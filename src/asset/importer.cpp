#include "importer.h"

#include <editor/project.h>
#include <core/fs_editor.h>
#include <asset/tracker.h>
#include <asset/importers/raw.h>
#include <utils/platform.h>

#ifdef ELM_PLATFORM_WINDOWS
#define DOT_FOLDER L"."
#else
#define DOT_FOLDER "."
#endif

QTimer* element::asset_importer::pending_import_timer = nullptr;
bool element::asset_importer::tracker_running = false;
element::asset_importer_node element::asset_importer::root_node;
std::unordered_map<std::string, element::asset_importer_callback> element::asset_importer::importers;
std::unordered_set<element::uuid> element::asset_importer::pending_to_import;

using namespace element;

void asset_importer::create_dir_nodes(const std::filesystem::path& path, asset_importer_node& node, std::vector<uuid>& import_pending) {
    bool is_dir = std::filesystem::is_directory(path);
    if (!is_dir) {
        std::string apath = get_fs_path_from_system(path);
        const uuid& id = fs_get_uuid_from_resource_path(apath);
        if (id.is_null()) {
            uuid new_id = fs_get_new_uuid();
            fs_resource_info info;
            info.path = std::move(apath);
            info.type = path.extension().string();
            if (info.type.length() > 0) info.type.erase(0, 1);
            fs_save_resource_info(new_id, std::move(info));
            import_pending.push_back(new_id);
        } else {
            std::filesystem::path bin_path = project::project_fs_path / id.str();
            if (std::filesystem::exists(bin_path)) {
                std::filesystem::file_time_type path_time = std::filesystem::last_write_time(path);
                std::filesystem::file_time_type bin_time = std::filesystem::last_write_time(bin_path);
                if (path_time > bin_time) import_pending.push_back(id);
            } else {
                import_pending.push_back(id);
            }
        }
    }
    asset_importer_node this_node;
    this_node.is_dir = is_dir;
    if (is_dir) {
        for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(path)) {
            create_dir_nodes(entry, this_node, import_pending);
        }
    }
    node.children[path.filename()] = this_node;
}

asset_importer_node& asset_importer::get_dir_nodes(const std::filesystem::path& path) {
    std::filesystem::path rpath = std::filesystem::relative(path, project::project_assets_path);
    asset_importer_node* node = &root_node;
    for (const std::filesystem::path::string_type part : rpath) {
        if (part == DOT_FOLDER) continue;
        node = &node->children[part];
        node->is_dir = true;
    }
    return *node;
}

void asset_importer::fix_dir_node_paths(const std::filesystem::path& old_path, const std::filesystem::path& new_path, const asset_importer_node& dir_node) {
    for (const auto& [name, node] : dir_node.children) {
        if (node.is_dir) {
            fix_dir_node_paths(old_path / name, new_path / name, node);
        } else {
            uuid id = fs_get_uuid_from_resource_path(get_fs_path_from_system(old_path / name));
            const fs_resource_info& old_info = fs_get_resource_info(id);
            fs_resource_info new_info;
            new_info.type = old_info.type;
            new_info.path = get_fs_path_from_system(new_path / name);
            fs_save_resource_info(id, std::move(new_info));
        }
    }
}

void asset_importer::delete_dir_nodes(const std::filesystem::path& path, const asset_importer_node& node) {
    for (const auto& [name, subnode] : node.children) {
        if (subnode.is_dir) {
            delete_dir_nodes(path / name, subnode);
        } else {
            uuid id = fs_get_uuid_from_resource_path(get_fs_path_from_system(path / name));
            fs_delete_resource_data(id);
            fs_delete_resource_info(id);
            pending_to_import.erase(id);
        }
    }
}

std::string asset_importer::get_fs_path_from_system(const std::filesystem::path& path) {
    return std::filesystem::relative(path, project::project_assets_path).generic_string();
}

void asset_importer::tracker_path_create(const std::filesystem::path& path, bool is_dir) {
    ELM_DEBUG("Create: is dir {0} path {1}", is_dir, path.string());
    if (is_dir) {
        std::filesystem::path dirpath = path;
        dirpath.remove_filename();
        std::vector<uuid> vimport;
        create_dir_nodes(path, get_dir_nodes(dirpath), vimport);
        pending_to_import.insert(vimport.begin(), vimport.end());
    } else {
        std::filesystem::path dirpath = path;
        dirpath.remove_filename();
        asset_importer_node& dirnode = get_dir_nodes(dirpath);
        asset_importer_node node;
        node.is_dir = false;
        dirnode.children[path.filename()] = std::move(node);
        uuid new_id = fs_get_new_uuid();
        fs_resource_info info;
        info.path = get_fs_path_from_system(path);
        info.type = path.extension().string();
        if (info.type.length() > 0) info.type.erase(0, 1);
        fs_save_resource_info(new_id, std::move(info));
        pending_to_import.insert(new_id);
    }
    pending_timer_reset();
}

void asset_importer::tracker_path_move(const std::filesystem::path &from, const std::filesystem::path &to) {
    ELM_DEBUG("Move: from {0} to {1}", from.string(), to.string());
    if (from == to) return;
    std::filesystem::path fromdir = from, todir = to;
    fromdir.remove_filename();
    todir.remove_filename();
    std::filesystem::path::string_type fromname = from.filename(), toname = to.filename();
    asset_importer_node& fromnode = get_dir_nodes(fromdir);
    asset_importer_node& tonode = get_dir_nodes(todir);
    tonode.children[toname] = std::move(fromnode.children[fromname]);
    fromnode.children.erase(fromname);
    if (tonode.children[toname].is_dir) {
        fix_dir_node_paths(from, to, tonode.children[toname]);
    } else {
        std::string respath = get_fs_path_from_system(from), resnewpath = get_fs_path_from_system(to);
        std::string fromext = from.extension().string(), toext = to.extension().string();
        uuid id = fs_get_uuid_from_resource_path(respath);
        if (fromext != toext) {
            pending_to_import.insert(id);
        }
        fs_resource_info newinfo;
        newinfo.path = std::move(resnewpath);
        newinfo.type = std::move(toext);
        if (newinfo.type.length() > 0) newinfo.type.erase(0, 1);
        fs_save_resource_info(id, std::move(newinfo));
    }
    pending_timer_reset();
}

void asset_importer::tracker_path_delete(const std::filesystem::path &path) {
    ELM_DEBUG("Delete: path {0}", path.string());
    std::filesystem::path dirpath = path;
    dirpath.remove_filename();
    asset_importer_node& dirnode = get_dir_nodes(dirpath);
    asset_importer_node& node = dirnode.children[path.filename()];
    if (node.is_dir) {
        delete_dir_nodes(path, node);
    } else {
        uuid id = fs_get_uuid_from_resource_path(get_fs_path_from_system(path));
        fs_delete_resource_data(id);
        fs_delete_resource_info(id);
        pending_to_import.erase(id);
    }
    dirnode.children.erase(path.filename());
    pending_timer_reset();
}

void asset_importer::tracker_path_modify(const std::filesystem::path &path) {
    ELM_DEBUG("Modify: file {0}", path.string());
    std::string apath = get_fs_path_from_system(path);
    const uuid& id = fs_get_uuid_from_resource_path(apath);
    if (!id.is_null()) pending_to_import.insert(id);
    pending_timer_reset();
}

void asset_importer::start() {
    if (tracker_running) {
        ELM_WARN("File system watcher already started.");
        return;
    }
    std::unordered_map<std::string, asset_importer_callback>& preregistered = get_preregistered_importers();
    importers.insert(preregistered.begin(), preregistered.end());
    ELM_DEBUG("Registered importers: {}", importers.size());
    for (const auto& [type, cbs] : importers) {
        ELM_DEBUG("    {}", type);
    }
    root_node.is_dir = true;
    ELM_INFO("Detecting removed assets...");
    std::unordered_map<uuid, fs_resource_info> fs_map = fs_get_map();
    for (const auto& [id, info] : fs_map) {
        if (!std::filesystem::exists(project::project_assets_path / info.path)) {
            ELM_DEBUG("{} was removed", info.path);
            fs_delete_resource_data(id);
            fs_delete_resource_info(id);
        }
    }
    ELM_INFO("Importing updated assets...");
    std::vector<uuid> import_pending;
    for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(project::project_assets_path)) {
        create_dir_nodes(entry, root_node, import_pending);
    }
    ELM_INFO("{} assets to import.", import_pending.size());
    fs_save_resources();
    import_all(import_pending.begin(), import_pending.end());
    ELM_INFO("Starting file system watcher...");
    asset_tracker::start();
    ELM_DEBUG("Watching path {0}", project::project_assets_path.string());
    tracker_running = true;
    pending_import_timer = new QTimer();
    pending_import_timer->setSingleShot(true);
    pending_import_timer->setTimerType(Qt::TimerType::CoarseTimer);
    pending_import_timer->callOnTimeout(pending_import);
}

void asset_importer::stop() {
    if (tracker_running) {
        ELM_INFO("Stopping file system watcher...");
        if (pending_import_timer->isActive()) {
            pending_import_timer->stop();
            pending_import();
        }
        delete pending_import_timer;
        asset_tracker::stop();
        root_node.children.clear();
        importers.clear();
        tracker_running = false;
    }
}

void asset_importer::register_importer(const std::string& type, asset_importer_callback importer) {
    importers.try_emplace(type, importer);
}

void asset_importer::register_importer(std::string&& type, asset_importer_callback importer) {
    importers.try_emplace(std::move(type), importer);
}

void asset_importer::reimport() {
    //TODO: cleara cache fs folder
    //TODO: import every file
}

void asset_importer::import(const uuid& id) {
    fs_resource_info info = fs_get_resource_info(id);
    auto it = importers.find(info.type);
    if (it != importers.end()) {
        ELM_INFO("Importing {0} ({1}).", info.path, id.str());
        it->second.import(id);
    } else {
        ELM_INFO("Raw importing {0} ({1}).", info.path, id.str());
        importers::raw_importer(id);
    }
}

void asset_importer::recreate_assets_dir() {
    ELM_WARN("Recreate asset dir");
}

void asset_importer::pending_timer_reset() {
    pending_import_timer->start(100);
}

void asset_importer::pending_import() {
    ELM_INFO("Importing all pending assets...");
    import_all(pending_to_import.begin(), pending_to_import.end());
    pending_to_import.clear();
    fs_save_resources();
}

std::unordered_map<std::string, asset_importer_callback>& asset_importer::get_preregistered_importers() {
    static std::unordered_map<std::string, asset_importer_callback> map;
    return map;
}

bool asset_importer::preregister_importer(std::string&& type, asset_importer_callback importer_cb) {
    get_preregistered_importers().try_emplace(std::move(type), importer_cb);
    return true;
}
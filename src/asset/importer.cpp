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

QTimer* element::__detail::__asset_importer_pending_import_timer = nullptr;
bool element::__detail::__asset_importer_tracker_running = false;
element::__detail::__asset_importer_node element::__detail::__asset_importer_root_node;
std::unordered_set<element::uuid> element::__detail::__asset_importer_pending_to_import;

using namespace element;

void __detail::__asset_importer_create_dir_nodes(const std::filesystem::path& path, __asset_importer_node& node, std::vector<uuid>& import_pending) {
    bool is_dir = std::filesystem::is_directory(path);
    if (!is_dir) {
        std::string apath = __asset_importer_get_fs_path_from_system(path);
        const uuid& id = fs::get_uuid_from_resource_path(apath);
        if (id.is_null()) {
            uuid new_id = fs::get_new_uuid();
            fs_resource_info info;
            info.path = std::move(apath);
            info.type = path.extension().string();
            if (info.type.length() > 0) info.type.erase(0, 1);
            fs::save_resource_info(new_id, std::move(info));
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
    __asset_importer_node this_node;
    this_node.is_dir = is_dir;
    if (is_dir) {
        for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(path)) {
            __asset_importer_create_dir_nodes(entry, this_node, import_pending);
        }
    }
    node.children[path.filename()] = this_node;
}

__detail::__asset_importer_node& __detail::__asset_importer_get_dir_nodes(const std::filesystem::path& path) {
    std::filesystem::path rpath = std::filesystem::relative(path, project::project_assets_path);
    __asset_importer_node* node = &__asset_importer_root_node;
    for (const std::filesystem::path::string_type part : rpath) {
        if (part == DOT_FOLDER) continue;
        node = &node->children[part];
        node->is_dir = true;
    }
    return *node;
}

void __detail::__asset_importer_fix_dir_node_paths(const std::filesystem::path& old_path, const std::filesystem::path& new_path, const __asset_importer_node& dir_node) {
    for (const auto& [name, node] : dir_node.children) {
        if (node.is_dir) {
            __asset_importer_fix_dir_node_paths(old_path / name, new_path / name, node);
        } else {
            uuid id = fs::get_uuid_from_resource_path(__asset_importer_get_fs_path_from_system(old_path / name));
            const fs_resource_info& old_info = fs::get_resource_info(id);
            fs_resource_info new_info;
            new_info.type = old_info.type;
            new_info.path = __asset_importer_get_fs_path_from_system(new_path / name);
            fs::save_resource_info(id, std::move(new_info));
        }
    }
}

void __detail::__asset_importer_delete_dir_nodes(const std::filesystem::path& path, const __asset_importer_node& node) {
    for (const auto& [name, subnode] : node.children) {
        if (subnode.is_dir) {
            __asset_importer_delete_dir_nodes(path / name, subnode);
        } else {
            uuid id = fs::get_uuid_from_resource_path(__asset_importer_get_fs_path_from_system(path / name));
            fs::delete_resource_data(id);
            fs::delete_resource_info(id);
            __asset_importer_pending_to_import.erase(id);
        }
    }
}

std::string __detail::__asset_importer_get_fs_path_from_system(const std::filesystem::path& path) {
    return std::filesystem::relative(path, project::project_assets_path).generic_string();
}

void __detail::__asset_importer_tracker_path_create(const std::filesystem::path& path, bool is_dir) {
    ELM_DEBUG("Create: is dir {0} path {1}", is_dir, path.string());
    if (is_dir) {
        std::filesystem::path dirpath = path;
        dirpath.remove_filename();
        std::vector<uuid> vimport;
        __asset_importer_create_dir_nodes(path, __asset_importer_get_dir_nodes(dirpath), vimport);
        __asset_importer_pending_to_import.insert(vimport.begin(), vimport.end());
    } else {
        std::filesystem::path dirpath = path;
        dirpath.remove_filename();
        __asset_importer_node& dirnode = __asset_importer_get_dir_nodes(dirpath);
        __asset_importer_node node;
        node.is_dir = false;
        dirnode.children[path.filename()] = std::move(node);
        uuid new_id = fs::get_new_uuid();
        fs_resource_info info;
        info.path = __asset_importer_get_fs_path_from_system(path);
        info.type = path.extension().string();
        if (info.type.length() > 0) info.type.erase(0, 1);
        fs::save_resource_info(new_id, std::move(info));
        __asset_importer_pending_to_import.insert(new_id);
    }
    __asset_importer_pending_timer_reset();
}

void __detail::__asset_importer_tracker_path_move(const std::filesystem::path &from, const std::filesystem::path &to) {
    ELM_DEBUG("Move: from {0} to {1}", from.string(), to.string());
    if (from == to) return;
    std::filesystem::path fromdir = from, todir = to;
    fromdir.remove_filename();
    todir.remove_filename();
    std::filesystem::path::string_type fromname = from.filename(), toname = to.filename();
    __asset_importer_node& fromnode = __asset_importer_get_dir_nodes(fromdir);
    __asset_importer_node& tonode = __asset_importer_get_dir_nodes(todir);
    tonode.children[toname] = std::move(fromnode.children[fromname]);
    fromnode.children.erase(fromname);
    if (tonode.children[toname].is_dir) {
        __asset_importer_fix_dir_node_paths(from, to, tonode.children[toname]);
    } else {
        std::string respath = __asset_importer_get_fs_path_from_system(from), resnewpath = __asset_importer_get_fs_path_from_system(to);
        std::string fromext = from.extension().string(), toext = to.extension().string();
        uuid id = fs::get_uuid_from_resource_path(respath);
        if (fromext != toext) {
            __asset_importer_pending_to_import.insert(id);
        }
        fs_resource_info newinfo;
        newinfo.path = std::move(resnewpath);
        newinfo.type = std::move(toext);
        if (newinfo.type.length() > 0) newinfo.type.erase(0, 1);
        fs::save_resource_info(id, std::move(newinfo));
    }
    __asset_importer_pending_timer_reset();
}

void __detail::__asset_importer_tracker_path_delete(const std::filesystem::path &path) {
    ELM_DEBUG("Delete: path {0}", path.string());
    std::filesystem::path dirpath = path;
    dirpath.remove_filename();
    __asset_importer_node& dirnode = __asset_importer_get_dir_nodes(dirpath);
    __asset_importer_node& node = dirnode.children[path.filename()];
    if (node.is_dir) {
        __asset_importer_delete_dir_nodes(path, node);
    } else {
        uuid id = fs::get_uuid_from_resource_path(__asset_importer_get_fs_path_from_system(path));
        fs::delete_resource_data(id);
        fs::delete_resource_info(id);
        __asset_importer_pending_to_import.erase(id);
    }
    dirnode.children.erase(path.filename());
    __asset_importer_pending_timer_reset();
}

void __detail::__asset_importer_tracker_path_modify(const std::filesystem::path &path) {
    ELM_DEBUG("Modify: file {0}", path.string());
    std::string apath = __asset_importer_get_fs_path_from_system(path);
    const uuid& id = fs::get_uuid_from_resource_path(apath);
    if (!id.is_null()) __asset_importer_pending_to_import.insert(id);
    __asset_importer_pending_timer_reset();
}

std::unordered_map<std::string, asset_importer::asset_importer_callback>& __detail::__asset_importer_get_importers() {
    static std::unordered_map<std::string, asset_importer::asset_importer_callback> map;
    return map;
}

void __detail::__asset_importer_pending_timer_reset() {
    __asset_importer_pending_import_timer->start(100);
}

void asset_importer::start() {
    if (__detail::__asset_importer_tracker_running) {
        ELM_WARN("File system watcher already started.");
        return;
    }
    auto& importers = __detail::__asset_importer_get_importers();
    ELM_DEBUG("Registered importers: {}", importers.size());
    for (const auto& [type, cbs] : importers) {
        ELM_DEBUG("    {}", type);
    }
    __detail::__asset_importer_root_node.is_dir = true;
    ELM_INFO("Detecting removed assets...");
    std::unordered_map<uuid, fs_resource_info> fs_map = fs::get_map();
    for (const auto& [id, info] : fs_map) {
        if (!std::filesystem::exists(project::project_assets_path / info.path)) {
            ELM_DEBUG("{} was removed", info.path);
            fs::delete_resource_data(id);
            fs::delete_resource_info(id);
        }
    }
    ELM_INFO("Importing updated assets...");
    std::vector<uuid> import_pending;
    for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(project::project_assets_path)) {
        __detail::__asset_importer_create_dir_nodes(entry, __detail::__asset_importer_root_node, import_pending);
    }
    ELM_INFO("{} assets to import.", import_pending.size());
    fs::save_resources();
    import_all(import_pending.begin(), import_pending.end());
    ELM_INFO("Starting file system watcher...");
    asset_tracker::start();
    ELM_DEBUG("Watching path {0}", project::project_assets_path.string());
    __detail::__asset_importer_tracker_running = true;
    __detail::__asset_importer_pending_import_timer = new QTimer();
    __detail::__asset_importer_pending_import_timer->setSingleShot(true);
    __detail::__asset_importer_pending_import_timer->setTimerType(Qt::TimerType::CoarseTimer);
    __detail::__asset_importer_pending_import_timer->callOnTimeout(pending_import);
}

void asset_importer::stop() {
    if (__detail::__asset_importer_tracker_running) {
        ELM_INFO("Stopping file system watcher...");
        if (__detail::__asset_importer_pending_import_timer->isActive()) {
            __detail::__asset_importer_pending_import_timer->stop();
            pending_import();
        }
        delete __detail::__asset_importer_pending_import_timer;
        asset_tracker::stop();
        __detail::__asset_importer_root_node.children.clear();
        __detail::__asset_importer_tracker_running = false;
    }
}

void asset_importer::register_importer(const std::string& type, asset_importer_callback importer) {
    __detail::__asset_importer_get_importers().try_emplace(type, importer);
}

void asset_importer::register_importer(std::string&& type, asset_importer_callback importer) {
    __detail::__asset_importer_get_importers().try_emplace(std::move(type), importer);
}

void asset_importer::unregister_importer(const std::string& type) {
    __detail::__asset_importer_get_importers().erase(type);
}

void asset_importer::reimport() {
    //TODO: cleara cache fs folder
    //TODO: import every file
}

void asset_importer::import(const uuid& id) {
    fs_resource_info info = fs::get_resource_info(id);
    auto it = __detail::__asset_importer_get_importers().find(info.type);
    if (it != __detail::__asset_importer_get_importers().end()) {
        ELM_INFO("Importing {0} ({1}).", info.path, id.str());
        it->second(id);
    } else {
        ELM_INFO("Raw importing {0} ({1}).", info.path, id.str());
        importers::raw_importer(id);
    }
}

void asset_importer::recreate_assets_dir() {
    ELM_WARN("Recreate asset dir");
}

void asset_importer::pending_import() {
    ELM_INFO("Importing all pending assets...");
    import_all(__detail::__asset_importer_pending_to_import.begin(), __detail::__asset_importer_pending_to_import.end());
    __detail::__asset_importer_pending_to_import.clear();
    fs::save_resources();
}
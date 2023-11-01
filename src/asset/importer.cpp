#include "importer.h"

#include <editor/project.h>
#include <core/fs_editor.h>
#include <asset/tracker.h>
#include <asset/importers/default_importer.h>
#include <utils/platform.h>
#include <utils/packed_set.h>

#ifdef ELM_PLATFORM_WINDOWS
#define DOT_FOLDER L"."
#else
#define DOT_FOLDER "."
#endif

struct asset_importer_node {
    bool is_dir;
    std::unordered_map<std::filesystem::path::string_type, asset_importer_node> children;
};

static QTimer* pending_import_timer = nullptr;
static bool tracker_running = false;
static asset_importer_node root_node;
static element::packed_set<element::uuid> pending_to_import;

using namespace element;

static void create_dir_nodes(const std::filesystem::path& path, bool is_dir, asset_importer_node& node, std::vector<uuid>& import_pending) {
    if (!is_dir) {
        std::string apath = asset_importer::get_fs_path_from_system(path);
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
    asset_importer_node this_node;
    this_node.is_dir = is_dir;
    if (is_dir) {
        for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(path)) {
            create_dir_nodes(entry, entry.is_directory(), this_node, import_pending);
        }
    }
    node.children[path.filename()] = std::move(this_node);
}

static void create_dir_nodes(const std::filesystem::path& path, asset_importer_node& node, std::vector<uuid>& import_pending) {
    bool is_dir = std::filesystem::is_directory(path);
    create_dir_nodes(path, is_dir, node, import_pending);
}

static asset_importer_node& get_dir_node(const std::filesystem::path& path) {
    std::filesystem::path rpath = std::filesystem::relative(path, project::project_assets_path);
    asset_importer_node* node = &root_node;
    for (const std::filesystem::path::string_type part : rpath) {
        if (part == DOT_FOLDER) continue;
        node = &node->children[part];
        node->is_dir = true;
    }
    return *node;
}

static void fix_dir_node_paths(const std::filesystem::path& old_path, const std::filesystem::path& new_path, const asset_importer_node& node) {
    if (node.is_dir) {
        for (const auto& [child_name, child_node] : node.children) {
            fix_dir_node_paths(old_path / child_name, new_path / child_name, child_node);
        }
    } else {
        uuid id = fs::get_uuid_from_resource_path(asset_importer::get_fs_path_from_system(old_path));
        fs_resource_info new_info;
        new_info.type = new_path.extension().string();
        new_info.path = asset_importer::get_fs_path_from_system(new_path);
        if (new_info.type.length() > 0) new_info.type.erase(0, 1);
        fs::save_resource_info(id, std::move(new_info));
        if (old_path.extension() != new_path.extension()) {
            pending_to_import.insert(id);
        }
    }
}

static void delete_dir_nodes(const std::filesystem::path& path, const asset_importer_node& node) {
    if (node.is_dir) {
        for (const auto& [child_name, child_node] : node.children) {
            delete_dir_nodes(path / child_name, child_node);
        }
    } else {
        uuid id = fs::get_uuid_from_resource_path(asset_importer::get_fs_path_from_system(path));
        events::asset_deleted event;
        event.id = id;
        event_manager::send_event(event);
        fs::delete_resource_data(id);
        fs::delete_resource_info(id);
        pending_to_import.erase(id);
    }
}

static std::unordered_map<std::string, asset_importer::asset_importer_callback>& get_importers() {
    static std::unordered_map<std::string, asset_importer::asset_importer_callback> map;
    return map;
}

static void pending_timer_reset() {
    pending_import_timer->start(100);
}

void asset_importer::start() {
    if (tracker_running) {
        ELM_WARN("File system watcher already started.");
        return;
    }
    auto& importers = get_importers();
    ELM_DEBUG("Registered importers: {}", importers.size());
    for (const auto& [type, cbs] : importers) {
        ELM_DEBUG("    {}", type);
    }
    root_node.is_dir = true;
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
        create_dir_nodes(entry, root_node, import_pending);
    }
    ELM_INFO("{} assets to import.", import_pending.size());
    fs::save_resources();
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
        tracker_running = false;
    }
}

void asset_importer::register_importer(const std::string& type, asset_importer_callback importer) {
    get_importers().try_emplace(type, importer);
}

void asset_importer::register_importer(std::string&& type, asset_importer_callback importer) {
    get_importers().try_emplace(std::move(type), importer);
}

void asset_importer::unregister_importer(const std::string& type) {
    get_importers().erase(type);
}

std::string asset_importer::get_fs_path_from_system(const std::filesystem::path& path) {
    return std::filesystem::relative(path, project::project_assets_path).generic_string();
}

void asset_importer::reimport() {
    //TODO: cleara cache fs folder
    //TODO: import every file
}

void asset_importer::import(const uuid& id) {
    import(id, false);
}

void asset_importer::import(const uuid& id, bool call_event) {
    fs_resource_info info = fs::get_resource_info(id);
    auto it = get_importers().find(info.type);
    if (it != get_importers().end()) {
        ELM_INFO("Importing {0} ({1}).", info.path, id.str());
        it->second(id);
    } else {
        ELM_INFO("Raw importing {0} ({1}).", info.path, id.str());
        importers::default_importer(id);
    }
    if (call_event) {
        events::asset_updated event;
        event.id = id;
        event_manager::send_event(event);
    }
}

void asset_importer::recreate_assets_dir() {
    ELM_WARN("Recreate asset dir");
}

void asset_importer::pending_import() {
    ELM_INFO("Importing all pending assets...");
    import_all(pending_to_import.begin(), pending_to_import.end());
    pending_to_import.clear();
    fs::save_resources();
}

void __detail::__asset_importer_tracker_path_create(const std::filesystem::path& path, bool is_dir) {
    ELM_DEBUG("Create: is dir {0} path {1}", is_dir, path.string());
    std::filesystem::path dirpath = path;
    dirpath.remove_filename();
    std::vector<uuid> vimport;
    create_dir_nodes(path, is_dir, get_dir_node(dirpath), vimport);
    pending_to_import.insert(vimport.begin(), vimport.end());
    pending_timer_reset();
}

void __detail::__asset_importer_tracker_path_move(const std::filesystem::path &from, const std::filesystem::path &to) {
    ELM_DEBUG("Move: from {0} to {1}", from.string(), to.string());
    if (from == to) return;
    std::filesystem::path fromdir = from, todir = to;
    fromdir.remove_filename();
    todir.remove_filename();
    std::filesystem::path::string_type fromname = from.filename(), toname = to.filename();
    asset_importer_node& fromnode = get_dir_node(fromdir);
    asset_importer_node& tonode = get_dir_node(todir);
    tonode.children[toname] = std::move(fromnode.children[fromname]);
    fromnode.children.erase(fromname);
    fix_dir_node_paths(from, to, tonode.children[toname]);
    pending_timer_reset();
}

void __detail::__asset_importer_tracker_path_delete(const std::filesystem::path &path) {
    ELM_DEBUG("Delete: path {0}", path.string());
    std::filesystem::path dirpath = path;
    dirpath.remove_filename();
    asset_importer_node& dirnode = get_dir_node(dirpath);
    asset_importer_node& node = dirnode.children[path.filename()];
    delete_dir_nodes(path, node);
    dirnode.children.erase(path.filename());
    pending_timer_reset();
}

void __detail::__asset_importer_tracker_path_modify(const std::filesystem::path &path) {
    ELM_DEBUG("Modify: file {0}", path.string());
    std::string apath = asset_importer::get_fs_path_from_system(path);
    const uuid& id = fs::get_uuid_from_resource_path(apath);
    if (!id.is_null()) pending_to_import.insert(id);
    pending_timer_reset();
}
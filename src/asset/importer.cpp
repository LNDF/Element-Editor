#include "importer.h"

#include <core/log.h>
#include <core/fs_editor.h>
#include <event/event.h>
#include <editor/project.h>
#include <asset/asset_events.h>
#include <asset/tracker.h>
#include <asset/dependencies.h>
#include <asset/importers/default_importer.h>
#include <utils/platform.h>
#include <utils/packed_set.h>
#include <future>
#include <vector>
#include <QTimer>

#ifdef ELM_PLATFORM_WINDOWS
#define DOT_FOLDER L"."
#else
#define DOT_FOLDER "."
#endif

struct asset_importer_node {
    bool is_dir;
    std::unordered_map<std::filesystem::path::string_type, asset_importer_node> children;
};

struct asset_importer_data {
    element::asset_importer::asset_importer_callback callback;
    std::uint32_t min_iteration;
};

static QTimer* pending_import_timer = nullptr;
static bool tracker_running = false;
static bool should_restart_timer = false;
static asset_importer_node root_node;
static element::packed_set<element::uuid> pending_to_import;

using namespace element;

static std::unordered_map<std::string, asset_importer_data>& get_importers() {
    static std::unordered_map<std::string, asset_importer_data> map;
    return map;
}

static void create_dir_nodes(const std::filesystem::path& path, bool is_dir, asset_importer_node& node) {
    if (!is_dir) {
        std::string apath = asset_importer::get_fs_path_from_system(path);
        const uuid& id = fs::get_uuid_from_resource_path(apath);
        if (id.is_null()) {
            uuid new_id;
            fs_resource_info info;
            info.path = apath;
            info.type = path.extension().string();
            if (info.type.length() > 0) info.type.erase(0, 1);
            fs::save_resource_info(new_id, std::move(info));
            __detail::__asset_importer_fix_dependents_import(new_id, apath);
            asset_importer::import(new_id);
        } else {
            fs_resource_info info = fs::get_resource_info(id);
            auto it = get_importers().find(info.type);
            if (it == get_importers().end() || it->second.callback != importers::null_importer) {
                std::filesystem::path bin_path = fs::get_resource_data_path(id);
                if (std::filesystem::exists(bin_path)) {
                    std::filesystem::file_time_type path_time = std::filesystem::last_write_time(path);
                    std::filesystem::file_time_type bin_time = std::filesystem::last_write_time(bin_path);
                    if (path_time > bin_time) asset_importer::import(id);
                } else {
                    asset_importer::import(id);
                }
            }
        }
    }
    asset_importer_node this_node;
    this_node.is_dir = is_dir;
    if (is_dir) {
        for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(path)) {
            create_dir_nodes(entry, entry.is_directory(), this_node);
        }
    }
    node.children[path.filename()] = std::move(this_node);
}

static void create_dir_nodes(const std::filesystem::path& path, asset_importer_node& node) {
    bool is_dir = std::filesystem::is_directory(path);
    create_dir_nodes(path, is_dir, node);
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
        std::string old_apath = asset_importer::get_fs_path_from_system(old_path);
        std::string new_apath = asset_importer::get_fs_path_from_system(new_path);
        fs_resource_info new_info;
        new_info.type = new_path.extension().string();
        new_info.path = new_apath;
        if (new_info.type.length() > 0) new_info.type.erase(0, 1);
        fs::save_resource_info(id, std::move(new_info));
        __detail::__asset_importer_fix_dependents_move(id, old_apath, new_apath);
        asset_importer::import(id);
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
        asset_importer::delete_dependency_data(id);
        pending_to_import.erase(id);
    }
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
    load_dependencies();
    events::asset_deleted delete_event;
    std::unordered_map<uuid, fs_resource_info> fs_map = fs::get_map();
    for (const auto& [id, info] : fs_map) {
        if (!std::filesystem::exists(project::project_assets_path / info.path)) {
            ELM_DEBUG("{} was removed", info.path);
            delete_event.id = id;
            event_manager::send_event(delete_event);
            fs::delete_resource_data(id);
            fs::delete_resource_info(id);
            delete_dependency_data(id);
        }
    }
    ELM_INFO("Importing updated assets...");
    for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(project::project_assets_path)) {
        create_dir_nodes(entry, root_node);
    }
    ELM_INFO("{} assets to import.", pending_to_import.size());
    import_pending_assets();
    ELM_INFO("Starting file system watcher...");
    asset_tracker::start();
    ELM_DEBUG("Watching path {0}", project::project_assets_path.string());
    tracker_running = true;
    pending_import_timer = new QTimer();
    pending_import_timer->setSingleShot(true);
    pending_import_timer->setTimerType(Qt::TimerType::CoarseTimer);
    pending_import_timer->callOnTimeout(import_pending_assets);
    should_restart_timer = true;
}

void asset_importer::stop() {
    if (tracker_running) {
        ELM_INFO("Stopping file system watcher...");
        if (pending_import_timer->isActive()) {
            pending_import_timer->stop();
            import_pending_assets();
        }
        should_restart_timer = false;
        delete pending_import_timer;
        asset_tracker::stop();
        root_node.children.clear();
        tracker_running = false;
    }
}

void asset_importer::register_importer(const std::string& type, asset_importer_callback importer, std::uint32_t min_iteration) {
    get_importers().try_emplace(type, importer, min_iteration);
}

void asset_importer::register_importer(std::string&& type, asset_importer_callback importer, std::uint32_t min_iteration) {
    get_importers().try_emplace(std::move(type), importer, min_iteration);
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
    pending_to_import.insert(id);
    if (should_restart_timer) pending_import_timer->start(100);
}

void asset_importer::recreate_assets_dir() {
    ELM_WARN("Recreate asset dir");
}

void asset_importer::import_pending_assets() {
    ELM_INFO("Starting to import assets...");
    packed_set<uuid> imported_assets;
    packed_set<uuid> currently_importing(std::move(pending_to_import));
    std::uint32_t iteration = 0;
    pending_to_import.clear();
    should_restart_timer = false;
    while (currently_importing.size() > 0) {
        events::asset_updated event;
        ELM_INFO("Importing... (iteration {0})", ++iteration);
        {
            std::vector<std::future<void>> futures;
            futures.reserve(currently_importing.size());
            for (const uuid& id : currently_importing) {
                fs_resource_info info = fs::get_resource_info(id);
                asset_importer_callback importer;
                auto it = get_importers().find(info.type);
                if (it != get_importers().end()) {
                    if (it->second.min_iteration > iteration) {
                        pending_to_import.insert(id);
                        continue;
                    }
                    ELM_INFO("Importing {0} ({1}).", info.path, id.str());
                    importer = it->second.callback;
                } else {
                    ELM_INFO("Importing {0} ({1}) with default importer.", info.path, id.str());
                    importer = importers::default_importer;
                }
                futures.push_back(std::async(std::launch::async, importer, id));
                imported_assets.insert(id);
            }
        }
        for (const auto& pending : pending_to_import) {
            currently_importing.erase(pending);
        }
        for (const uuid& id : currently_importing) {
            const auto& deps = get_dependents(id);
            pending_to_import.insert(deps.begin(), deps.end());
            event.id = id;
            event_manager::send_event(event);
        }
        currently_importing = std::move(pending_to_import);
        pending_to_import.clear();
        for (const auto& imported : imported_assets) {
            currently_importing.erase(imported);
        }
    }
    should_restart_timer = true;
    ELM_INFO("Finished importing {0} assets.", imported_assets.size());
    fs::save_resources();
    save_dependencies();
    events::assets_imported event;
    event_manager::send_event(event);
}

bool asset_importer::is_tracker_running() {
    return tracker_running;
}

void __detail::__asset_importer_tracker_path_create(const std::filesystem::path& path, bool is_dir) {
    ELM_DEBUG("Create: is dir {0} path {1}", is_dir, path.string());
    std::filesystem::path dirpath = path;
    dirpath.remove_filename();
    create_dir_nodes(path, is_dir, get_dir_node(dirpath));
    events::asset_file_created event;
    event.path = path;
    event.is_dir = is_dir;
    event_manager::send_event(event);
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
    events::asset_file_moved event;
    event.from = from;
    event.to = to;
    event_manager::send_event(event);
}

void __detail::__asset_importer_tracker_path_delete(const std::filesystem::path &path) {
    ELM_DEBUG("Delete: path {0}", path.string());
    std::filesystem::path dirpath = path;
    dirpath.remove_filename();
    asset_importer_node& dirnode = get_dir_node(dirpath);
    asset_importer_node& node = dirnode.children[path.filename()];
    delete_dir_nodes(path, node);
    fs::save_resources();
    asset_importer::save_dependencies();
    dirnode.children.erase(path.filename());
    events::asset_file_deleted event;
    event.path = path;
    event_manager::send_event(event);
}

void __detail::__asset_importer_tracker_path_modify(const std::filesystem::path &path) {
    ELM_DEBUG("Modify: file {0}", path.string());
    std::string apath = asset_importer::get_fs_path_from_system(path);
    const uuid& id = fs::get_uuid_from_resource_path(apath);
    if (!id.is_null()) asset_importer::import(id);
}
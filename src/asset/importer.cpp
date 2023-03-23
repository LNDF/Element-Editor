#include "importer.h"

#include <core/log.h>
#include <core/fs_editor.h>
#include <editor/project.h>
#include <asset/tracker.h>

bool element::asset_importer::tracker_running = false;
element::asset_importer_node element::asset_importer::root_node;

using namespace element;

void asset_importer::add_dir_nodes_and_import(const std::filesystem::path& path, asset_importer_node& node, std::vector<uuid>& import_pending) {
    bool is_dir = std::filesystem::is_directory(path);
    if (!is_dir) {
        std::string apath = get_fs_path_from_system(path);
        const uuid& id = fs_get_uuid_from_resource_path(apath);
        if (id.is_null()) {
            uuid new_id = fs_get_new_uuid();
            fs_resource_info info;
            info.path = std::move(apath);
            info.type = fs_resource_type::dummy; //TODO: detect resource type
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
    node.children[path.filename()] = this_node;
    if (is_dir) {
        for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(path)) {
            add_dir_nodes_and_import(entry, this_node, import_pending);
        }
    }
}

std::string asset_importer::get_fs_path_from_system(const std::filesystem::path& path) {
    return std::filesystem::relative(path, project::project_assets_path).string();
}

void asset_importer::tracker_path_create(const std::filesystem::path& path, bool is_dir) {
    ELM_DEBUG("Create: is dir {0} path {1}", is_dir, path.string());
}

void asset_importer::tracker_path_move(const std::filesystem::path &from, const std::filesystem::path &to) {
    ELM_DEBUG("Move: from {0} to {1}", from.string(), to.string());
}

void asset_importer::tracker_path_delete(const std::filesystem::path &path) {
    ELM_DEBUG("Delete: path {0}", path.string());
}

void asset_importer::tracker_path_modify(const std::filesystem::path &path) {
    ELM_DEBUG("Modify: file {0}", path.string());
}

void asset_importer::start() {
    if (tracker_running) {
        ELM_WARN("File system watcher already started.");
        return;
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
        add_dir_nodes_and_import(entry, root_node, import_pending);
    }
    ELM_INFO("{} assets to import.", import_pending.size());
    ELM_INFO("Starting file system watcher...");
    asset_tracker::start();
    ELM_DEBUG("Watching path {0}", project::project_assets_path.string());
    tracker_running = true;
    //TODO: add specialized functions...
}

void asset_importer::stop() {
    if (tracker_running) {
        ELM_INFO("Stopping file system watcher...");
        asset_tracker::stop();
        tracker_running = false;
    }
}

void asset_importer::reimport() {
    //TODO: cleara cache fs folder
    //TODO: import every file
}

void asset_importer::import(const std::string& path) {
    //TODO: delete item from cache fs folder
    //TODO import file
}

void asset_importer::recreate_assets_dir() {
    ELM_WARN("Recreate asset dir");
}

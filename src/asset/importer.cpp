#include "importer.h"

#include <core/log.h>
#include <editor/project.h>
#include <asset/asset_tracker.h>

std::unordered_map<element::uuid, std::filesystem::file_time_type> element::asset_importer::modified_tracaking;
bool element::asset_importer::tracker_running = false;

using namespace element;

void asset_importer::start() {
    if (tracker_running) {
        ELM_WARN("File system watcher already started.");
        return;
    }
    ELM_INFO("Starting file system watcher...");
    asset_tracker::start();
    ELM_DEBUG("Watching path {0}", project::project_assets_path.c_str());
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

void asset_importer::load() {
    //TODO: load "modified_tracking"
    //TODO: import files if they changed
    //TODO: save fs map
}

void asset_importer::save() {
    //TODO: save "modified_tracking"
}

void asset_importer::reimport() {
    //TODO: clear "modified_tracking"
    //TODO: clear cache fs folder
    //TODO: import every file and add to "modified_tracking"
}

void asset_importer::import(const std::string& path) {
    //TODO: delete item from cache fs folder
    //TODO import file
    //TODO: update item in "modified_tracking"
}
#include "importer.h"

#include <core/log.h>
#include <editor/project.h>
#include <QDir>
#include <QFileInfo>

std::unordered_map<int, int> element::asset_importer::import_handlers; //TODO
std::unordered_map<element::uuid, std::filesystem::file_time_type> element::asset_importer::modified_tracaking;
QFileSystemWatcher* element::asset_importer::watcher = nullptr;

using namespace element;

void asset_importer::qt_watcher_callback(const QString& path) {
    ELM_DEBUG("Update {0}", path.toStdString());
    QDir qdir(path);
    for (const QFileInfo& subdir : qdir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        watcher->addPath(subdir.absoluteFilePath());
    }
}

void asset_importer::start() {
    if (watcher != nullptr) {
        ELM_WARN("File system watcher already started.");
        return;
    }
    ELM_INFO("Starting file system watcher...");
    ELM_DEBUG("Watching path {0}", project::project_assets_path.c_str());
    watcher = new QFileSystemWatcher();
    watcher->addPath(project::project_assets_path.c_str());
    QDir qdir(project::project_assets_path.c_str());
    for (const QFileInfo& subdir : qdir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        watcher->addPath(subdir.absoluteFilePath());
    }
    QObject::connect(watcher, &QFileSystemWatcher::directoryChanged, qt_watcher_callback);
    //TODO: add specialized functions...
}

void asset_importer::stop() {
    if (watcher != nullptr) {
        ELM_INFO("Stopping file system watcher...");
        delete watcher;
        watcher = nullptr;
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
#pragma once

#include <unordered_map>
#include <filesystem>
#include <QFileSystemWatcher>

#include <utils/uuid.h>

namespace element {

    class asset_importer {
        private:
            static std::unordered_map<int, int> import_handlers; //TODO
            static std::unordered_map<uuid, std::filesystem::file_time_type> modified_tracaking;
            static QFileSystemWatcher* watcher;
            static void qt_watcher_callback(const QString& path);
        public:
            static void start();
            static void stop();
            static void load();
            static void save();
            static void reimport();
            static void import(const std::string& path);
    };

} // namespace element

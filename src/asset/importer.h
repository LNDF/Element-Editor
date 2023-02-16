#pragma once

#include <filesystem>
#include <unordered_map>
#include <filewatch/filewatch.h>
#include <utils/uuid.h>

namespace element {

    class asset_importer {
        private:
            using path_string = std::basic_string<std::filesystem::path::value_type>;
            static std::unordered_map<int, int> import_handlers; //TODO
            static std::unordered_map<uuid, std::filesystem::file_time_type> modified_tracaking;
            static filewatch::FileWatch<path_string>* watcher;
            static void watcher_callback(const path_string& path, const filewatch::Event change_type);
        public:
            static void start();
            static void stop();
            static void load();
            static void save();
            static void reimport();
            static void import(const std::string& path);
    };

} // namespace element

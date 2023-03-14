#pragma once

#include <filesystem>
#include <unordered_map>
#include <utils/uuid.h>
#include <utils/packed_map.h>

namespace element {

    struct asset_importer_node {
        bool is_dir;
        packed_map<std::string, asset_importer_node> children;
    };

    class asset_importer {
        private:
            static bool tracker_running;
            static asset_importer_node root_node;

        public:
            static void tracker_path_create(const std::filesystem::path& path, bool is_dir);
            static void tracker_path_move(const std::filesystem::path& from, const std::filesystem::path& to);
            static void tracker_path_delete(const std::filesystem::path& path);
            static void tracker_path_modify(const std::filesystem::path& path);
        
            static void start();
            static void stop();
            static void load();
            static void save();
            static void reimport();
            static void import(const std::string& path);
            static void recreate_assets_dir();
            
            static inline bool is_tracker_running() {return tracker_running;}
    };

} // namespace element

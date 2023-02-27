#pragma once

#include <filesystem>
#include <unordered_map>
#include <utils/uuid.h>

namespace element {

    class asset_importer {
        private:
            static std::unordered_map<uuid, std::filesystem::file_time_type> modified_tracaking;
            static bool tracker_running;

        public:
            static void tracker_path_create(const std::filesystem::path& path, bool is_dir);
            static void tracker_path_move(const std::filesystem::path& from, const std::filesystem::path& to, bool is_dir);
            static void tracker_path_delete(const std::filesystem::path& path, bool is_dir);
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

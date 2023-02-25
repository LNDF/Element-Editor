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
            static void start();
            static void stop();
            static void load();
            static void save();
            static void reimport();
            static void import(const std::string& path);
            
            static inline bool is_tracker_running() {return tracker_running;}
    };

} // namespace element

#pragma once

#include <utils/uuid.h>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <string>
#include <vector>

namespace element {
    namespace project {
        
        extern std::string name;
        extern std::string author;
        extern std::string version;
        extern uuid startup_scene;
        extern std::unordered_map<std::string, std::vector<std::string>> plugins;
        extern std::filesystem::path project_path;
        extern std::filesystem::path project_bin_path;
        extern std::filesystem::path project_filename;
        extern std::filesystem::path project_cache_path;
        extern std::filesystem::path project_metadata_path;
        extern std::filesystem::path project_assets_path;

        void open(const std::filesystem::path& path);
        bool exists();
        void mkdir();
        void load();
        void save();

    } // namespace project
} // namespace element

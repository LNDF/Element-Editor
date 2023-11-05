#pragma once

#include <string>
#include <filesystem>

namespace element {
    namespace project {
        
        extern std::string name;
        extern std::string author;
        extern std::string version;
        extern std::filesystem::path project_path;
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

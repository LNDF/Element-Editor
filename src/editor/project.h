#pragma once

#include <string>
#include <filesystem>

namespace element {
    
    class project {
        public:
            static std::string name;
            static std::string author;
            static std::string version;
            static std::filesystem::path project_path;
            static std::filesystem::path project_filename;
            static std::filesystem::path project_fs_path;
            static std::filesystem::path project_fs_fsmap;
            static std::filesystem::path project_metadata_path;
            static std::filesystem::path project_metadata_fsmap;
            static std::filesystem::path project_assets_path;

            static void open(const std::filesystem::path& path);
            static bool exists();
            static void mkdir();
            static void load();
            static void save();
    };

} // namespace element

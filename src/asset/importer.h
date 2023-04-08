#pragma once

#include <filesystem>
#include <core/fs.h>
#include <utils/packed_map.h>
#include <utils/uuid.h>

#define ELM_REGISTER_IMPORTER(type, import_fun) static bool __elm_register_importer_##type = element::asset_importer::preregister_importer(#type, element::asset_importer_callback{import_fun})

namespace element {

    struct asset_importer_node {
        bool is_dir;
        packed_map<std::string, asset_importer_node> children;
    };

    struct asset_importer_callback {
        std::function<void(const uuid&)> import;
    };

    class asset_importer {
        private:
            static bool tracker_running;
            static asset_importer_node root_node;
            static std::unordered_map<std::string, asset_importer_callback> importers;

            static void add_dir_nodes_and_import(const std::filesystem::path& path, asset_importer_node& node, std::vector<uuid>& import_pending);
            static std::string get_fs_path_from_system(const std::filesystem::path& path);

        public:
            static void tracker_path_create(const std::filesystem::path& path, bool is_dir);
            static void tracker_path_move(const std::filesystem::path& from, const std::filesystem::path& to);
            static void tracker_path_delete(const std::filesystem::path& path);
            static void tracker_path_modify(const std::filesystem::path& path);
        
            static void start();
            static void stop();
            static void register_importer(const std::string& type, asset_importer_callback importer);
            static void register_importer(std::string&& type, asset_importer_callback importer);
            static void reimport();
            static void import(const uuid& id);
            static void import_all(const std::vector<uuid>& all);
            static void recreate_assets_dir();
            
            static inline bool is_tracker_running() {return tracker_running;}

            static std::unordered_map<std::string, asset_importer_callback>& get_preregistered_importers();
            static bool preregister_importer(std::string&& type, asset_importer_callback importer_cb);
    };

} // namespace element

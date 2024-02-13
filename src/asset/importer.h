#pragma once

#include <asset/importers/null_importer.h>
#include <utils/pre_exec.h>
#include <utils/uuid.h>
#include <filesystem>

#define ELM_REGISTER_IMPORTER(type, import_fun, min_iteration) ELM_PRE_EXECUTE(element::asset_importer::register_importer, type, import_fun, min_iteration)
#define ELM_REGISTER_SKIP_IMPORT(type) ELM_REGISTER_IMPORTER(type, element::importers::null_importer, 1)

namespace element {
    
    namespace __detail {
        void __asset_importer_tracker_path_create(const std::filesystem::path& path, bool is_dir);
        void __asset_importer_tracker_path_move(const std::filesystem::path& from, const std::filesystem::path& to);
        void __asset_importer_tracker_path_delete(const std::filesystem::path& path);
        void __asset_importer_tracker_path_modify(const std::filesystem::path& path);
    } // namespace __detail

    namespace asset_importer {
        using asset_importer_callback = void(*)(const uuid&);

        void start();
        void stop();
        void register_importer(const std::string& type, asset_importer_callback importer, std::uint32_t min_iteration);
        void register_importer(std::string&& type, asset_importer_callback importer, std::uint32_t min_iteration);
        void unregister_importer(const std::string& type);
        std::string get_fs_path_from_system(const std::filesystem::path& path);
        std::filesystem::path get_system_path_from_fs(const std::string& path);
        void reimport();
        void import(const uuid& id);
        void recreate_assets_dir();

        void import_pending_assets();
        
        bool is_tracker_running();
        
        template<typename It>
        void import_all(It from, It to) {
            for (It i = from; i != to; i++) {
                import(*from);
            }
        }
    } // namespace asset_importer

} // namespace element

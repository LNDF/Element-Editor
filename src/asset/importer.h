#pragma once

#include <core/log.h>
#include <core/fs.h>
#include <event/event.h>
#include <asset/asset_events.h>
#include <utils/packed_map.h>
#include <utils/pre_exec.h>
#include <utils/uuid.h>

#include <filesystem>
#include <vector>
#include <future>
#include <unordered_map>
#include <unordered_set>

#include <QTimer>

#define ELM_REGISTER_IMPORTER(type, import_fun) ELM_PRE_EXECUTE(element::asset_importer::register_importer, type, import_fun)

namespace element {
    
    namespace asset_importer {
        using asset_importer_callback = void(*)(const uuid&);
    } // namespace asset_importer
    

    namespace __detail {
        struct __asset_importer_node {
            bool is_dir;
            std::unordered_map<std::filesystem::path::string_type, __asset_importer_node> children;
        };

        extern QTimer* __asset_importer_pending_import_timer;
        extern bool __asset_importer_tracker_running;
        extern __asset_importer_node __asset_importer_root_node;
        extern std::unordered_set<uuid> __asset_importer_pending_to_import;

        void __asset_importer_create_dir_nodes(const std::filesystem::path& path, __asset_importer_node& node, std::vector<uuid>& import_pending);
        __asset_importer_node& __asset_importer_get_dir_nodes(const std::filesystem::path& path);
        void __asset_importer_fix_dir_node_paths(const std::filesystem::path& old_path, const std::filesystem::path& new_path, const __asset_importer_node& dir_node);
        void __asset_importer_delete_dir_nodes(const std::filesystem::path& path, const __asset_importer_node& node);
        std::string __asset_importer_get_fs_path_from_system(const std::filesystem::path& path);

        void __asset_importer_tracker_path_create(const std::filesystem::path& path, bool is_dir);
        void __asset_importer_tracker_path_move(const std::filesystem::path& from, const std::filesystem::path& to);
        void __asset_importer_tracker_path_delete(const std::filesystem::path& path);
        void __asset_importer_tracker_path_modify(const std::filesystem::path& path);

        std::unordered_map<std::string, asset_importer::asset_importer_callback>& __asset_importer_get_importers();
        
        void __asset_importer_pending_timer_reset();
    } // namespace __detail

    namespace asset_importer {
        using asset_importer_callback = void(*)(const uuid&);

        void start();
        void stop();
        void register_importer(const std::string& type, asset_importer_callback importer);
        void register_importer(std::string&& type, asset_importer_callback importer);
        void unregister_importer(const std::string& type);
        void reimport();
        void import(const uuid& id);
        void import(const uuid& id, bool call_event);
        void recreate_assets_dir();
        
        void pending_import();
        
        inline bool is_tracker_running() {return __detail::__asset_importer_tracker_running;}
        
        template<typename It>
        void import_all(It from, It to) {
            ELM_INFO("Starting to import assets...");
            std::vector<std::future<void>> futures;
            for (; from != to; from++) {
                futures.push_back(std::async<void(const uuid&, bool)>(std::launch::async, asset_importer::import, *from, false));
            }
            events::asset_updated event;
            for (; from != to; from++) {
                event.id = *from;
                event_manager::send_event(event);
            }
            
        }
    } // namespace asset_importer

} // namespace element

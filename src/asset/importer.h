#pragma once

#include <core/log.h>
#include <event/event.h>
#include <asset/asset_events.h>
#include <asset/importers/null_importer.h>
#include <utils/pre_exec.h>
#include <utils/uuid.h>

#include <filesystem>
#include <future>

#include <QTimer>

#define ELM_REGISTER_IMPORTER(type, import_fun) ELM_PRE_EXECUTE(element::asset_importer::register_importer, type, import_fun)
#define ELM_REGISTER_SKIP_IMPORT(type) ELM_REGISTER_IMPORTER(type, element::importers::null_importer)

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
        void register_importer(const std::string& type, asset_importer_callback importer);
        void register_importer(std::string&& type, asset_importer_callback importer);
        void unregister_importer(const std::string& type);
        std::string get_fs_path_from_system(const std::filesystem::path& path);
        void reimport();
        void import(const uuid& id);
        void import(const uuid& id, bool call_event);
        void recreate_assets_dir();

        void pending_import();
        
        bool is_tracker_running();
        
        template<typename It>
        void import_all(It from, It to) {
            ELM_INFO("Starting to import assets...");
            {
                std::vector<std::future<void>> futures;
                for (It i = from; i != to; i++) {
                    futures.push_back(std::async<void(const uuid&, bool)>(std::launch::async, asset_importer::import, *i, false));
                }
            }
            events::asset_updated event;
            for (; from != to; from++) {
                event.id = *from;
                event_manager::send_event(event);
            }
            
        }
    } // namespace asset_importer

} // namespace element

#pragma once

#include <utils/uuid.h>
#include <filesystem>

namespace element {
    namespace events {
        
        struct asset_updated {
            uuid id;
        };

        struct asset_deleted {
            uuid id;
        };
        
        struct assets_imported {

        };

        struct asset_file_created {
            std::filesystem::path path;
            bool is_dir;
        };

        struct asset_file_moved {
            std::filesystem::path from;
            std::filesystem::path to;
        };

        struct asset_file_deleted {
            std::filesystem::path path;
        };

    } // namespace events
    
} // namespace element

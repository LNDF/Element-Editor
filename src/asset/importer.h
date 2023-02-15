#pragma once

#include <unordered_map>
#include <filesystem>
#include <QFileSystemWatcher>

#include <utils/uuid.h>

namespace element {
    


    class asset_importer {
        private:
            static std::unordered_map<int, int> import_handlers; //TODO
            static std::unordered_map<uuid, std::filesystem::file_time_type> modified_tracaking;
        public:
            static void load();
            static void save();
            static void reimport();
    }

} // namespace element

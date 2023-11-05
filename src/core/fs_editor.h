#pragma once

#include <core/fs.h>

#include <ostream>
#include <filesystem>

namespace element {

    namespace fs {
        std::unique_ptr<std::ostream> get_resource_ostream(const uuid& id);
        void save_resource_info(const uuid& id, const fs_resource_info& info);
        void save_resource_info(const uuid& id, fs_resource_info&& info);
        void delete_resource_info(const uuid& id);
        void delete_resource_data(const uuid& id);
        std::filesystem::path get_resource_data_path(const uuid& id);
        void save_resources();
        void save_bin_resources();
    } // namespace fs

} // namespace element

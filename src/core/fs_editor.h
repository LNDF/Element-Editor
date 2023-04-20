#pragma once

#include <core/fs.h>

#include <ostream>
#include <unordered_map>

namespace element {

    namespace fs {
        std::unique_ptr<std::ostream> get_resource_ostream(const uuid& id);
        void save_resource_info(const uuid& id, const fs_resource_info& info);
        void save_resource_info(const uuid& id, fs_resource_info&& info);
        void delete_resource_info(const uuid& id);
        void delete_resource_data(const uuid& id);
        void save_resources();
        void save_bin_resources();
        uuid get_new_uuid();
        const std::unordered_map<element::uuid, element::fs_resource_info>& get_map();
    } // namespace fs

} // namespace element

#pragma once

#include <core/fs.h>

#include <ostream>
#include <unordered_map>

namespace element {

    std::unique_ptr<std::ostream> fs_get_resource_ostream(const uuid& id);
    void fs_save_resource_info(const uuid& id, const fs_resource_info& info);
    void fs_save_resource_info(const uuid& id, fs_resource_info&& info);
    void fs_delete_resource_info(const uuid& id);
    void fs_save_resources();
    void fs_save_bin_resources();

} // namespace element

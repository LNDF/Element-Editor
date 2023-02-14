#include "fs_editor.h"

#include <fstream>
#include <stdexcept>
#include <utility>
#include <filesystem>

#include <core/log.h>
#include <editor/project.h>
#include <serialization/defs.h>
#include <serialization/fs.h>
#include <serialization/string.h>
#include <serialization/unordered_map.h>
#include <serialization/uuid.h>

static std::unordered_map<element::uuid, element::fs_resource_info> fs_map;
static std::unordered_map<std::string, element::uuid> fs_uuid_map;

namespace element {
    std::unique_ptr<std::istream> fs_get_resource(const uuid& id) {
        const fs_resource_info& info = fs_map[id];
        std::filesystem::path path = project::project_assets_path / info.path;
        path.make_preferred();
        return std::unique_ptr<std::istream>(new std::ifstream(path, std::ios::binary));
    }

    std::unique_ptr<std::ostream> fs_get_resource_ostream(const uuid& id) {
        const fs_resource_info& info = fs_map[id];
        std::filesystem::path path = project::project_assets_path / info.path;
        path.make_preferred();
        return std::unique_ptr<std::ostream>(new std::ofstream(path, std::ios::binary));
    }

    const fs_resource_info& fs_get_resource_info(const uuid& id) {
        return fs_map.at(id);
    }

    const uuid& fs_get_uuid_from_resource_path(const std::string& path) {
        return fs_uuid_map.at(path);
    }

    void fs_save_resource_info(const uuid& id, const fs_resource_info& info) {
        try {
            fs_uuid_map.erase(fs_map.at(id).path);
        } catch (const std::out_of_range& e) {}
        try {
            fs_map.erase(fs_uuid_map.at(info.path));
        } catch (const std::out_of_range& e) {}
        fs_map[id] = info;
        fs_uuid_map[info.path] = id;
    }

    void fs_save_resource_info(const uuid& id, fs_resource_info&& info) {
        try {
            fs_uuid_map.erase(fs_map.at(id).path);
        } catch (const std::out_of_range& e) {}
        try {
            fs_map.erase(fs_uuid_map.at(info.path));
        } catch (const std::out_of_range& e) {}
        fs_map[id] = std::move(info);
        fs_uuid_map[info.path] = id;
    }

    void fs_delete_resource_info(const uuid& id) {
        fs_uuid_map.erase(fs_map.at(id).path);
        fs_map.erase(id);
    }

    void fs_load_resources() {
        ELM_INFO("Loading FS map...");
        ELM_DEBUG("FS map path is {0}", project::project_metadata_fsmap.c_str());
        std::ifstream file(project::project_metadata_fsmap);
        if (file.fail()) {
            ELM_WARN("Couldn't open FS map");
            return;
        }
        text_deserializer deserialize = create_text_deserializer(file);
        deserialize(ELM_SERIALIZE_NVP("fs_map", fs_map));
        fs_uuid_map.reserve(fs_map.size());
        for (const auto& [id, info] : fs_map) {
            fs_uuid_map[info.path] = id;
        }
    }

    void fs_save_resources() {
        ELM_INFO("Saving FS map...");
        ELM_DEBUG("FS map path is {0}", project::project_metadata_fsmap.c_str());
        std::ofstream file(project::project_metadata_fsmap);
        text_serializer serialize = create_text_serializer(file);
        serialize(ELM_SERIALIZE_NVP("fs_map", fs_map));
    }

    void fs_save_bin_resources() {
        ELM_INFO("Saving FS map to binary...");
        ELM_DEBUG("FS map path is {0}", project::project_fs_fsmap.c_str());
        std::ofstream file(project::project_fs_fsmap, std::ios::binary);
        binary_serializer serialize = create_binary_serializer(file);
        serialize(ELM_SERIALIZE_NVP("fs_map", fs_map));
    }
} // namespace element

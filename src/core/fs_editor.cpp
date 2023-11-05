#include "fs_editor.h"

#include <fstream>
#include <stdexcept>
#include <utility>

#include <core/log.h>
#include <editor/project.h>
#include <serialization/defs.h>
#include <serialization/core/fs.h>
#include <serialization/string.h>
#include <serialization/unordered_map.h>
#include <serialization/utils/uuid.h>

static std::filesystem::path fs_path;
static std::filesystem::path fs_map_path;
static std::filesystem::path fs_map_bin_path;


static std::unordered_map<element::uuid, element::fs_resource_info> fs_map;
static std::unordered_map<std::string, element::uuid> fs_uuid_map;

using namespace element;

std::unique_ptr<std::istream> fs::get_resource(const uuid& id) {
    std::filesystem::path path = fs_path / id.str();
    path.make_preferred();
    return std::unique_ptr<std::istream>(new std::ifstream(path, std::ios::binary));
}

std::unique_ptr<std::ostream> fs::get_resource_ostream(const uuid& id) {
    std::filesystem::path path = fs_path / id.str();
    path.make_preferred();
    return std::unique_ptr<std::ostream>(new std::ofstream(path, std::ios::binary));
}

const fs_resource_info& fs::get_resource_info(const uuid& id) {
    return fs_map.at(id);
}

const uuid& fs::get_uuid_from_resource_path(const std::string& path) {
    auto it = fs_uuid_map.find(path);
    if (it == fs_uuid_map.end()) return uuid::null();
    return it->second;
}

void fs::save_resource_info(const uuid& id, const fs_resource_info& info) {
    auto fs_it = fs_map.find(id);
    if (fs_it != fs_map.end()) {
        fs_uuid_map.erase(fs_it->second.path);
    }
    auto id_it = fs_uuid_map.find(info.path);
    if (id_it != fs_uuid_map.end()) {
        fs_map.erase(id_it->second);
    }
    fs_uuid_map[info.path] = id;
    fs_map[id] = info;
}

void fs::save_resource_info(const uuid& id, fs_resource_info&& info) {
    auto fs_it = fs_map.find(id);
    if (fs_it != fs_map.end()) {
        fs_uuid_map.erase(fs_it->second.path);
    }
    auto id_it = fs_uuid_map.find(info.path);
    if (id_it != fs_uuid_map.end()) {
        fs_map.erase(id_it->second);
    }
    fs_uuid_map[info.path] = id;
    fs_map[id] = std::move(info);
}

void fs::delete_resource_info(const uuid& id) {
    fs_uuid_map.erase(fs_map.at(id).path);
    fs_map.erase(id);
}

void fs::delete_resource_data(const uuid& id) {
    std::filesystem::path path = fs_path / id.str();
    path.make_preferred();
    if (std::filesystem::exists(path)) std::filesystem::remove_all(path);
}

std::filesystem::path fs::get_resource_data_path(const uuid& id) {
    return fs_path / id.str();
}

void fs::load_resources() {
    ELM_INFO("Loading FS map...");
    fs_path = project::project_cache_path / "fs";
    fs_map_path = project::project_metadata_path / "fs_map.json";
    fs_map_bin_path = fs_path / "fs_map.bin";
    std::filesystem::create_directories(fs_path);
    ELM_DEBUG("FS map path is {0}", fs_map_path.string());
    std::ifstream file(fs_map_path);
    if (file.fail()) {
        ELM_WARN("Couldn't load FS map. Is this a new project?");
        return;
    }
    text_deserializer deserialize = create_text_deserializer(file);
    deserialize(ELM_SERIALIZE_NVP("fs_map", fs_map));
    fs_uuid_map.reserve(fs_map.size());
    for (const auto& [id, info] : fs_map) {
        fs_uuid_map[info.path] = id;
    }
}

void fs::save_resources() {
    ELM_INFO("Saving FS map...");
    std::ofstream file(fs_map_path);
    text_serializer serialize = create_text_serializer(file);
    serialize(ELM_SERIALIZE_NVP("fs_map", fs_map));
}

void fs::save_bin_resources() {
    ELM_INFO("Saving FS map to binary...");
    ELM_DEBUG("FS map path is {0}", fs_map_bin_path.string());
    std::ofstream file(fs_map_bin_path, std::ios::binary);
    binary_serializer serialize = create_binary_serializer(file);
    serialize(ELM_SERIALIZE_NVP("fs_map", fs_map));
}

const std::unordered_map<element::uuid, element::fs_resource_info>& fs::get_map() {
    return fs_map;
}
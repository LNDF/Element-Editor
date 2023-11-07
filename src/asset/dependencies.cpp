#include "dependencies.h"

#include <core/log.h>
#include <core/fs.h>
#include <editor/project.h>
#include <asset/importer.h>
#include <serialization/defs.h>
#include <serialization/serializers_editor.h>
#include <serialization/unordered_map.h>
#include <serialization/utils/packed_set.h>
#include <serialization/utils/uuid.h>
#include <serialization/string.h>
#include <fstream>
#include <unordered_map>

using namespace element;

static std::filesystem::path dependencies_path;
static packed_set<uuid> empty_id_set;
static packed_set<std::string> empty_path_set;

static std::unordered_map<uuid, packed_set<uuid>> id_dependencies_map;
static std::unordered_map<uuid, packed_set<std::string>> path_dependencies_map;
static std::unordered_map<uuid, packed_set<uuid>> dependents_map;

static void update_id_dependents(const uuid& id, const packed_set<uuid>& new_deps) {
    const packed_set<uuid>& old_deps = id_dependencies_map[id];
    for (const uuid& dep : old_deps) {
        if (!new_deps.contains(dep)) {
            dependents_map[dep].erase(id);
        }
    }
    for (const uuid& dep : new_deps) {
        if (!old_deps.contains(dep)) {
            dependents_map[dep].insert(id);
        }
    }
}

static void update_path_dependents(const uuid& id, const packed_set<std::string>& new_deps) {
    const packed_set<std::string>& old_deps = path_dependencies_map[id];
    for (const std::string& dep : old_deps) {
        if (!new_deps.contains(dep)) {
            const uuid& dep_id = fs::get_uuid_from_resource_path(dep);
            if (dep_id.is_null()) continue;
            dependents_map[dep_id].erase(id);
        }
    }
    for (const std::string& dep : new_deps) {
        if (!old_deps.contains(dep)) {
            const uuid& dep_id = fs::get_uuid_from_resource_path(dep);
            if (dep_id.is_null()) continue;
            dependents_map[dep_id].insert(id);
        }
    }
}

void __detail::__asset_importer_fix_dependents_import(const uuid& id, const std::string& path) {
    for (const auto& [dep_id, deps] : path_dependencies_map) {
        if (deps.contains(path)) {
            dependents_map[id].insert(dep_id);
        }
    }
}

void __detail::__asset_importer_fix_dependents_move(const uuid& id, const std::string& old, const std::string& path) {
    for (const auto& [dep_id, deps] : path_dependencies_map) {
        if (deps.contains(old)) {
            dependents_map[id].erase(dep_id);
        }
    }
    __asset_importer_fix_dependents_import(id, path);
}

void asset_importer::load_dependencies() {
    ELM_INFO("Loading import dependencies...");
    dependencies_path = project::project_metadata_path / "dependencies.json";
    ELM_DEBUG("Dependencies path is {0}", dependencies_path.string());
    std::ifstream file(dependencies_path);
    if (file.fail()) {
        ELM_WARN("Couldn't load dependencies. If this is not a new project, please reimport assets.");
        return;
    }
    text_deserializer deserialize = create_text_deserializer(file);
    deserialize(ELM_SERIALIZE_NVP("id_dependencies", id_dependencies_map));
    deserialize(ELM_SERIALIZE_NVP("path_dependencies", path_dependencies_map));
    for (const auto& [id, deps] : id_dependencies_map) {
        for (const uuid& dep : deps) {
            dependents_map[dep].insert(id);
        }
    }
    for (const auto& [id, deps] : path_dependencies_map) {
        for (const std::string& dep : deps) {
            const uuid& dep_id = fs::get_uuid_from_resource_path(dep);
            if (dep_id.is_null()) continue;
            dependents_map[dep_id].insert(id);
        }
    }
}

void asset_importer::save_dependencies() {
    ELM_INFO("Saving import dependencies...");
    std::ofstream file(dependencies_path, std::ios::binary);
    text_serializer serialize = create_text_serializer(file);
    serialize(ELM_SERIALIZE_NVP("id_dependencies", id_dependencies_map));
    serialize(ELM_SERIALIZE_NVP("path_dependencies", path_dependencies_map));
}


void asset_importer::delete_dependency_data(const uuid& id) {
    auto it = id_dependencies_map.find(id);
    if (it != id_dependencies_map.end()) {
        for (const uuid& dep : it->second) {
            dependents_map[dep].erase(id);
        }
        id_dependencies_map.erase(it);
    }
    auto path_it = path_dependencies_map.find(id);
    if (path_it != path_dependencies_map.end()) {
        for (const std::string& path : path_it->second) {
            const uuid& path_id = fs::get_uuid_from_resource_path(path);
            if (path_id.is_null()) continue;
            dependents_map[path_id].erase(id);
        }
        path_dependencies_map.erase(path_it);
    }
    it = dependents_map.find(id);
    if (it != dependents_map.end()) {
        packed_set<uuid> dependents_to_remove;
        for (const uuid& dep : it->second) {
            if (!id_dependencies_map[dep].contains(id)) {
                dependents_to_remove.insert(dep);
            }
            asset_importer::import(dep);
        }
        it->second.erase(dependents_to_remove.begin(), dependents_to_remove.end());
    }
}

const packed_set<uuid>& asset_importer::get_id_dependencies(const uuid& id) {
    auto it = id_dependencies_map.find(id);
    if (it == id_dependencies_map.end()) return empty_id_set;
    return it->second;
}

const packed_set<std::string>& asset_importer::get_path_dependencies(const uuid& id) {
    auto it = path_dependencies_map.find(id);
    if (it == path_dependencies_map.end()) return empty_path_set;
    return it->second;
}

const packed_set<uuid>& asset_importer::get_dependents(const uuid& id) {
    auto it = dependents_map.find(id);
    if (it == dependents_map.end()) return empty_id_set;
    return it->second;
}

void asset_importer::set_id_dependencies(const uuid& id, const packed_set<uuid>& deps) {
    update_id_dependents(id, deps);
    if (deps.empty()) {
        id_dependencies_map.erase(id);
    } else {
        id_dependencies_map[id] = deps;
    }
}

void asset_importer::set_id_dependencies(const uuid& id, packed_set<uuid>&& deps) {
    update_id_dependents(id, deps);
    if (deps.empty()) {
        id_dependencies_map.erase(id);
    } else {
        id_dependencies_map[id] = std::move(deps);
    }
}

void asset_importer::set_path_dependencies(const uuid& id, const packed_set<std::string>& deps) {
    update_path_dependents(id, deps);
    if (deps.empty()) {
        path_dependencies_map.erase(id);
    } else {
        path_dependencies_map[id] = deps;
    }
}

void asset_importer::set_path_dependencies(const uuid& id, packed_set<std::string>&& deps) {
    update_path_dependents(id, deps);
    if (deps.empty()) {
        path_dependencies_map.erase(id);
    } else {
        path_dependencies_map[id] = std::move(deps);
    }
}
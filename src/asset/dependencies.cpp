#include "dependencies.h"

#include <core/log.h>
#include <editor/project.h>
#include <asset/importer.h>
#include <serialization/defs.h>
#include <serialization/unordered_map.h>
#include <serialization/utils/packed_set.h>
#include <serialization/utils/uuid.h>
#include <fstream>
#include <unordered_map>

using namespace element;

static packed_set<uuid> empty_set;

static std::unordered_map<uuid, packed_set<uuid>> dependencies_map;
static std::unordered_map<uuid, packed_set<uuid>> dependents_map;

static void update_dependents(const uuid& id, const packed_set<uuid>& new_deps) {
    const packed_set<uuid>& old_deps = dependencies_map[id];
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

void asset_importer::load_dependencies() {
    ELM_INFO("Loading import dependencies...");
    ELM_DEBUG("Dependencies path is {0}", project::project_metadata_dependencies.string());
    std::ifstream file(project::project_metadata_dependencies);
    if (file.fail()) {
        ELM_WARN("Couldn't load dependencies. If this is not a new project, please reimport assets.");
        return;
    }
    text_deserializer deserialize = create_text_deserializer(file);
    deserialize(ELM_SERIALIZE_NVP("dependencies", dependencies_map));
    for (const auto& [id, deps] : dependencies_map) {
        for (const uuid& dep : deps) {
            dependents_map[dep].insert(id);
        }
    }
}

void asset_importer::save_dependencies() {
    ELM_INFO("Saving import dependencies...");
    ELM_DEBUG("Dependencies path is {0}", project::project_metadata_dependencies.string());
    std::ofstream file(project::project_metadata_dependencies, std::ios::binary);
    text_serializer serialize = create_text_serializer(file);
    serialize(ELM_SERIALIZE_NVP("dependencies", dependencies_map));
}


void asset_importer::delete_dependency_data(const uuid& id) {
    auto it = dependencies_map.find(id);
    if (it != dependencies_map.end()) {
        for (const uuid& dep : it->second) {
            dependents_map[dep].erase(id);
        }
        dependencies_map.erase(it);
    }
    it = dependents_map.find(id);
    if (it != dependents_map.end()) {
        for (const uuid& dep : it->second) {
            auto& cont = dependencies_map[dep];
            cont.erase(id);
            if (cont.empty()) dependencies_map.erase(dep);
            asset_importer::import(dep);
        }
        dependents_map.erase(it);
    }
}

const packed_set<uuid>& asset_importer::get_dependencies(const uuid& id) {
    auto it = dependencies_map.find(id);
    if (it == dependencies_map.end()) return empty_set;
    return it->second;
}

const packed_set<uuid>& asset_importer::get_dependents(const uuid& id) {
    auto it = dependents_map.find(id);
    if (it == dependents_map.end()) return empty_set;
    return it->second;
}

void asset_importer::set_dependencies(const uuid& id, const packed_set<uuid>& deps) {
    if (deps.empty()) {
        delete_dependency_data(id);
    } else {
        update_dependents(id, deps);
        dependencies_map[id] = deps;
    }
}

void asset_importer::set_dependencies(const uuid& id, packed_set<uuid>&& deps) {
    if (deps.empty()) {
        delete_dependency_data(id);
    } else {
        update_dependents(id, deps);
        dependencies_map[id] = std::move(deps);
    }
}
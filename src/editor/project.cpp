#include "project.h"

#include <fstream>

#include <core/log.h>
#include <editor/project_events.h>
#include <plugins/editor.h>
#include <serialization/serializers_editor.h>
#include <serialization/defs.h>
#include <serialization/utils/uuid.h>
#include <serialization/unordered_map.h>
#include <serialization/vector.h>
#include <event/event.h>

using namespace element;
    
std::string project::name;
std::string project::author;
std::string project::version;
uuid project::startup_scene;
std::unordered_map<std::string, std::vector<std::string>> project::plugins;
std::filesystem::path project::project_path;
std::filesystem::path project::project_bin_path;
std::filesystem::path project::project_filename;
std::filesystem::path project::project_cache_path;
std::filesystem::path project::project_metadata_path;
std::filesystem::path project::project_assets_path;

void project::open(const std::filesystem::path& path) {
    project_path = std::filesystem::absolute(path);
    project_path.make_preferred();
    project_filename = project_path / "project.json";
    project_bin_path = project_path / "bin";
    project_cache_path = project_path / "cache";
    project_metadata_path = project_path / "meta";
    project_assets_path = project_path / "assets";
    events::project_opened event;
    event_manager::send_event(event);
}

bool project::exists() {
    return std::filesystem::exists(project_filename);
}

void project::mkdir() {
    std::filesystem::create_directories(project_path);
    std::filesystem::create_directories(project_cache_path);
    std::filesystem::create_directories(project_metadata_path);
    std::filesystem::create_directories(project_assets_path);
    std::filesystem::create_directories(project_bin_path);
    for (const auto& platform : plugins::platforms) {
        std::filesystem::create_directories(project_bin_path / platform);
    }
}

void project::load() {
    ELM_INFO("Loading project {0}...", project_path.string());
    {
        std::ifstream file(project_filename);
        text_deserializer deserialize = create_text_deserializer(file);
        deserialize(ELM_SERIALIZE_NVP("name",          name),
                    ELM_SERIALIZE_NVP("author",        author),
                    ELM_SERIALIZE_NVP("version",       version),
                    ELM_SERIALIZE_NVP("startup_scene", startup_scene),
                    ELM_SERIALIZE_NVP("plugins",       plugins));
    }
    ELM_INFO("Loaded project {0} version {1} by {2}", name, version, author);
}

void project::save() {
    ELM_INFO("Saving project {0}...", project_path.string());
    project::mkdir();
    {
        std::ofstream file(project_filename);
        text_serializer serialize = create_text_serializer(file);
        serialize(ELM_SERIALIZE_NVP("name",          name),
                  ELM_SERIALIZE_NVP("author",        author),
                  ELM_SERIALIZE_NVP("version",       version),
                  ELM_SERIALIZE_NVP("startup_scene", startup_scene),
                  ELM_SERIALIZE_NVP("plugins",       plugins));
    }
    events::project_saved event;
    event_manager::send_event(event);
}
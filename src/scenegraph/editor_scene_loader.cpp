#include "editor_scene_loader.h"

#include <core/fs.h>
#include <core/log.h>
#include <editor/project.h>
#include <scenegraph/scene_manager.h>
#include <scenegraph/editor_camera_node_storage.h>
#include <render/render.h>
#include <serialization/defs.h>
#include <serialization/serializers_editor.h>
#include <serialization/scenegraph.h>
#include <fstream>
#include <filesystem>

using namespace element;

static void open_and_import(const uuid& id, const std::string& path) {
    std::filesystem::path sys_path = project::project_assets_path / path;
    std::ifstream stream(sys_path);
    if (stream.fail()) {
        ELM_WARN("Couldn't open scene {0} {{1}}", sys_path.string(), id.str());
        return;
    }
    if (!scenegraph::close_scene()) return;
    scenegraph::scene s;
    text_deserializer deserialize = create_text_deserializer(stream);
    deserialize(ELM_SERIALIZE_NVP("scene", s));
    scenegraph::import_scene(id, std::move(s));
    render::get_screen_scene_renderer()->select_scene(id, scenegraph::get_editor_camera());
}

void scenegraph::open_scene(const uuid& id) {
    fs_resource_info info = fs::get_resource_info(id);
    if (info.path.empty()) {
        ELM_WARN("Couldn't get path for scene with UUID {0}", id.str());
        return;
    }
    open_and_import(id, info.path);
}

void scenegraph::open_scene(const std::string& path) {
    const uuid& id = fs::get_uuid_from_resource_path(path);
    if (id.is_null()) {
        ELM_WARN("Couldn't get UUID for scene with path {0}", path);
        return;
    }
    open_and_import(id, path);
}

bool scenegraph::close_scene() {
    //TODO: saving and stuff
    render::get_screen_scene_renderer()->select_scene(uuid::null());
    scenegraph::destroy_all_scenes();
    return true;
}
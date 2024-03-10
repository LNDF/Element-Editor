#include "editor_scene_loader.h"

#include <core/fs.h>
#include <core/log.h>
#include <asset/loaders/scene.h>
#include <editor/editor.h>
#include <scenegraph/scene_manager.h>
#include <scenegraph/editor_camera_node_storage.h>
#include <render/render.h>
#include <serialization/defs.h>
#include <serialization/serializers_editor.h>
#include <ui/element_editor.h>
#include <fstream>
#include <filesystem>

using namespace element;

static scenegraph::scene* current_scene = nullptr;

static void open_and_import(const uuid& id, const std::string& path) {
    std::optional<scenegraph::scene> s = asset_loader::scene_text_load(path);
    if (!s.has_value()) {
        ELM_WARN("Couldn't open scene {0} {{1}}", path, id.str());
        return;
    }
    ELM_INFO("Loading scene {} in editor...", path);
    if (!scenegraph::close_scene()) return;
    current_scene = scenegraph::import_scene(id, std::move(s.value()));
    editor::main_window->load_scene();
    if (render::get_screen_scene_renderer() == nullptr) return;
    render::get_screen_scene_renderer()->select_scene(id, scenegraph::get_editor_camera());
    render::render_screen();
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
    if (current_scene == nullptr) return true;
    //TODO: saving and stuff
    ELM_INFO("Unloading editor scene...");
    current_scene = nullptr;
    editor::main_window->unload_scene();
    render::get_screen_scene_renderer()->select_scene(uuid::null());
    scenegraph::destroy_all_scenes();
    render::render_screen_safe();
    return true;
}

void scenegraph::save_scene() {
    if (current_scene == nullptr) return;
    ELM_INFO("Saving current scene...");
    const fs_resource_info& info = fs::get_resource_info(current_scene->get_id());
    asset_loader::scene_text_save(info.path, *current_scene);
}

scenegraph::scene* scenegraph::get_current_scene() {
    return current_scene;
}
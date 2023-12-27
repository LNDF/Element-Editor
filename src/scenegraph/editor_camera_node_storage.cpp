#include "editor_camera_node_storage.h"

using namespace element;

static scenegraph::editor_camera_node_storage camera_storage_instance;

scenegraph::editor_camera_node_storage::editor_camera_node_storage() {}

scenegraph::editor_camera_node_storage::~editor_camera_node_storage() {
    if (!editor_cam.get_id().is_null()) {
        cleanup_node(editor_cam);
        delete_node_storage_mapping(editor_cam.get_id());
    }
}

void scenegraph::editor_camera_node_storage::delete_node(const uuid& id) {}
void scenegraph::editor_camera_node_storage::emplace_node(const uuid& id, const std::string& name, node_ref& parent) {}
void scenegraph::editor_camera_node_storage::emplace_node(const uuid& id, std::string&& name, node_ref& parent) {}
void scenegraph::editor_camera_node_storage::emplace_root(const uuid& id) {}

scenegraph::node* scenegraph::editor_camera_node_storage::get_node_ptr(const uuid& id) {
    if (id.is_null() || editor_cam.get_id() != id) return nullptr;
    return &editor_cam;
}

std::type_index scenegraph::editor_camera_node_storage::get_type_index() const {
    return std::type_index(typeid(camera_node));
}

void scenegraph::editor_camera_node_storage::init_scene(scenegraph::scene* owner_scene) {}

scenegraph::camera_node_ref& scenegraph::editor_camera_node_storage::get_editor_camera() {
    if (editor_cam.get_id().is_null()) {
        uuid id;
        update_node_storage_mapping(id, this);
        editor_cam = camera_node(id, "Editor camera", nullptr);
        cache_number++;
        setup_node(editor_cam);
        editor_cam_ref = camera_node_ref(id);
    }
    return editor_cam_ref;
}

scenegraph::camera_node_ref& scenegraph::get_editor_camera() {
    return camera_storage_instance.get_editor_camera();
}
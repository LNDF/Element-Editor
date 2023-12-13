#include <core/fs.h>
#include <asset/asset_events.h>
#include <event/event.h>
#include <render/mesh_manager.h>

static void reload_mesh_common(const element::uuid& id) {
    element::fs_resource_info info = element::fs::get_resource_info(id);
    if (info.type == "mesh") element::render::gpu_mesh_manager::reload_resource(id);
}

static bool reload_mesh_after_inport(element::events::asset_updated& event) {
    reload_mesh_common(event.id);
    return true;
}

static bool reload_mesh_after_delete(element::events::asset_deleted& event) {
    reload_mesh_common(event.id);
    return true;
}

ELM_REGISTER_EVENT_CALLBACK(element::events::asset_updated, reload_mesh_after_inport, element::event_callback_priority::highest)
ELM_REGISTER_EVENT_CALLBACK(element::events::asset_deleted, reload_mesh_after_delete, element::event_callback_priority::highest)
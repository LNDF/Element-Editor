#include <core/fs.h>
#include <asset/asset_events.h>
#include <event/event.h>
#include <render/material.h>

static void destroy_material_common(const element::uuid& id) {
    element::fs_resource_info info = element::fs::get_resource_info(id);
    if (info.type == "material") element::render::destroy_material(id);
}

static bool destroy_material_after_inport(element::events::asset_updated& event) {
    destroy_material_common(event.id);
    return true;
}

static bool destroy_material_after_delete(element::events::asset_deleted& event) {
    destroy_material_common(event.id);
    return true;
}

ELM_REGISTER_EVENT_CALLBACK(element::events::asset_updated, destroy_material_after_inport, element::event_callback_priority::highest)
ELM_REGISTER_EVENT_CALLBACK(element::events::asset_deleted, destroy_material_after_delete, element::event_callback_priority::highest)
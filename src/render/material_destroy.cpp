#include <core/fs.h>
#include <asset/asset_events.h>
#include <event/event.h>
#include <render/material.h>

static void destroy_material_common(const element::uuid& id) {
    element::fs_resource_info info = element::fs::get_resource_info(id);
    if (info.type == "material") {
        std::uint32_t references = 0;
        element::render::gpu_material* gpu_mat = element::render::get_gpu_material(id);
        if (gpu_mat != nullptr) {
            references = gpu_mat->get_references();
        }
        element::render::destroy_material(id);
        if (references > 0) {
            element::render::get_or_create_gpu_material(id)->__set_references(references);
        }
    }
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
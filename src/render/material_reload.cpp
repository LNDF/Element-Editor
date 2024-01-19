#include <core/fs.h>
#include <asset/asset_events.h>
#include <event/event.h>
#include <render/material.h>
#include <render/material_manager.h>
#include <render/scene_data_manager.h>

static void scene_render_material_disable(const element::uuid& material, const element::uuid& pipeline) {
    for (auto& [data_id, data] : element::render::get_scene_render_data_map()) {
        data.disable_material(material, pipeline);
    }
}

static void scene_render_material_enable(const element::uuid& material, const element::uuid& pipeline) {
    for (auto& [data_id, data] : element::render::get_scene_render_data_map()) {
        data.enable_material(material, pipeline);
    }
}

static void scene_render_material_pipeline_update(const element::uuid& material, const element::uuid& old_pipeline, const element::uuid& new_pipeline) {
    for (auto& [data_id, data] : element::render::get_scene_render_data_map()) {
        data.update_material(material, old_pipeline, new_pipeline);
    }
}

static bool reload_material_after_inport(element::events::asset_updated& event) {
    element::fs_resource_info info = element::fs::get_resource_info(event.id);
    if (info.type == "material") {
        element::render::material* old_mat = element::material_manager::get(event.id, false);
        element::uuid old_pipeline = element::uuid::null();
        bool has_gpu_mat = false;
        if (old_mat != nullptr) {
            has_gpu_mat = element::render::get_gpu_material(event.id) != nullptr;
            old_pipeline = old_mat->get_pipeline_id();
        }
        element::render::destroy_material(event.id);
        element::render::material* new_mat = element::material_manager::get(event.id);
        element::uuid new_pipeline = element::uuid::null();
        if (new_mat != nullptr) {
            new_pipeline = new_mat->get_pipeline_id();
        }
        if (old_mat != nullptr && new_mat != nullptr) {
            if (old_pipeline != new_pipeline) {
                scene_render_material_pipeline_update(event.id, old_pipeline, new_pipeline);
            }
            if (has_gpu_mat) element::render::get_or_create_gpu_material(event.id);
        } else if (old_mat == nullptr && new_mat != nullptr) {
            scene_render_material_enable(event.id, new_pipeline);
        } else if (old_mat != nullptr && new_mat == nullptr) {
            scene_render_material_disable(event.id, old_pipeline);
        }

    }
    return true;
}

static bool destroy_material_after_delete(element::events::asset_deleted& event) {
    element::fs_resource_info info = element::fs::get_resource_info(event.id);
    if (info.type == "material") {
        element::render::material* old_mat = element::material_manager::get(event.id, false);
        element::uuid old_pipeline = element::uuid::null();
        if (old_mat != nullptr) {
            old_pipeline = old_mat->get_pipeline_id();
        }
        element::render::destroy_material(event.id);
        if (old_mat != nullptr) {
            scene_render_material_disable(event.id, old_pipeline);
        }
    }
    return true;
}

ELM_REGISTER_EVENT_CALLBACK(element::events::asset_updated, reload_material_after_inport, element::event_callback_priority::highest)
ELM_REGISTER_EVENT_CALLBACK(element::events::asset_deleted, destroy_material_after_delete, element::event_callback_priority::highest)
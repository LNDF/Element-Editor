#include <core/fs.h>
#include <asset/asset_events.h>
#include <event/event.h>
#include <render/pipeline.h>

static void destroy_pipeline_common(const element::uuid& id) {
    element::fs_resource_info info = element::fs::get_resource_info(id);
    if (info.type == "pipeline") element::render::destroy_pipeline(id);
}

static bool destroy_pipeline_after_import(element::events::asset_updated& event) {
    destroy_pipeline_common(event.id);
    return true;
}

static bool destroy_pipeline_after_delete(element::events::asset_deleted& event) {
    destroy_pipeline_common(event.id);
    return true;
}

ELM_REGISTER_EVENT_CALLBACK(element::events::asset_updated, destroy_pipeline_after_import, element::event_callback_priority::highest)
ELM_REGISTER_EVENT_CALLBACK(element::events::asset_deleted, destroy_pipeline_after_delete, element::event_callback_priority::highest)
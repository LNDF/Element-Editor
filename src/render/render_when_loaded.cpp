#include <asset/asset_events.h>
#include <event/event.h>
#include <editor/editor.h>
#include <resource/async_loader_events.h>
#include <render/render.h>

using namespace element;

static bool render_on_resource_load(events::resource_end_loading& event) {
    editor::execute_in_editor_thread(render::render_screen_safe);
    return true;
}

static bool render_on_assets_imported(events::assets_imported& event) {
    render::render_screen_safe();
    return true;
}

ELM_REGISTER_EVENT_CALLBACK(events::resource_end_loading, render_on_resource_load, event_callback_priority::highest)
ELM_REGISTER_EVENT_CALLBACK(events::assets_imported, render_on_assets_imported, event_callback_priority::highest)
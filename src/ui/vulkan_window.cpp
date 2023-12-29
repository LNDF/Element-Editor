#include "vulkan_window.h"

#include <editor/editor.h>
#include <core/log.h>
#include <event/event.h>
#include <editor/editor.h>
#include <resource/async_loader_events.h>
#include <graphics/vulkan_swapchain.h>
#include <render/render.h>

#include <QResizeEvent>

using namespace element;

qt_vulkan_window::qt_vulkan_window(QWindow* parent) : QWindow(parent) {
    create();
    surface = vulkan::create_surface_from_qt(this);
    if (!vulkan::device_initialized) {
        vulkan::init_device(surface);
        render::init_renderer();
    }
}

qt_vulkan_window::~qt_vulkan_window() {
    if (swapchain_created) vulkan::destroy_swapchain(swapchain);
    vulkan::instance.destroySurfaceKHR(surface, nullptr, vulkan::dld);
    swapchain_created = false;
}

void qt_vulkan_window::resizeEvent(QResizeEvent* ev) {
    if (ev->size() == ev->oldSize()) return;
    ELM_DEBUG("Editor renderer resized to {0}x{1}", ev->size().width(), ev->size().height());
    if (swapchain_created) {
        render::unselect_swapchain();
        vulkan::destroy_swapchain(swapchain);
    }
    vulkan::swapchain_creation_info info = vulkan::query_swapchain_info(surface, ev->size().width(), ev->size().height());
    swapchain = vulkan::create_swapchain(info);
    render::select_swapchain(swapchain);
    swapchain_created = true;
    render::render_screen_safe();
}

static bool render_on_resource_load(events::end_loading& event) {
    editor::execute_in_editor_thread(render::render_screen_safe);
    return true;
}

ELM_REGISTER_EVENT_CALLBACK(events::end_loading, render_on_resource_load, event_callback_priority::highest)
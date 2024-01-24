#include "vulkan_window.h"

#include <editor/editor.h>
#include <core/log.h>
#include <render/vulkan_swapchain.h>
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
#include <scenegraph/editor_scene_loader.h>
void qt_vulkan_window::resizeEvent(QResizeEvent* ev) {
    if (ev->size() == ev->oldSize()) return;
    ELM_DEBUG("Editor renderer resized to {0}x{1}", ev->size().width(), ev->size().height());
    bool first_time = !swapchain_created;
    if (swapchain_created) {
        render::unselect_swapchain();
        vulkan::destroy_swapchain(swapchain);
    }
    vulkan::swapchain_creation_info info = vulkan::query_swapchain_info(surface, ev->size().width(), ev->size().height());
    swapchain = vulkan::create_swapchain(info);
    render::select_swapchain(swapchain);
    swapchain_created = true;
    if (first_time) {
        scenegraph::open_scene("test.scene");
    }
    render();
}

void qt_vulkan_window::render() {
    render::render_screen_safe();
}
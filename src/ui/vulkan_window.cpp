#include "vulkan_window.h"

#include <editor/editor.h>
#include <core/log.h>
#include <graphics/vulkan_swapchain.h>
#include <render/render.h>

#include <QResizeEvent>

using namespace element;

qt_vulkan_window::qt_vulkan_window(QWindow* parent) : QWindow(parent) {
    create();
    surface = vulkan::create_surface_from_qt(this);
    if (vulkan::device == nullptr) {
        vulkan::init_device(surface);
        render::init_renderer();
    }
}

qt_vulkan_window::~qt_vulkan_window() {
    if (swapchain_created) vulkan::destroy_swapchain(swapchain);
    vulkan::instance.destroySurfaceKHR(surface, nullptr, vulkan::dld);
    swapchain_created = false;
}

void qt_vulkan_window::resizeEvent(QResizeEvent *ev) {
    ELM_DEBUG("Editor renderer resized to {0}x{1}", ev->size().width(), ev->size().height());
    if (swapchain_created) vulkan::destroy_swapchain(swapchain);
    vulkan::swapchain_creation_info info = vulkan::query_swapchain_info(surface, this->width(), this->height());
    swapchain = vulkan::create_swapchain(info);
    swapchain_created = true;
}
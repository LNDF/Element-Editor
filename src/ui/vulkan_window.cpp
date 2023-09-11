#include "vulkan_window.h"

#include <editor/editor.h>
#include <core/log.h>
#include <graphics/vulkan_swapchain.h>

#include <QResizeEvent>

using namespace element;

qt_vulkan_window::qt_vulkan_window(QWindow* parent) : QWindow(parent) {
    create();
    surface = vulkan_create_surface_from_qt(this);
    if (!vulkan::is_device_created()) {
        vulkan::init_device(surface);
    }
}

qt_vulkan_window::~qt_vulkan_window() {
    if (swapchain != nullptr) if (swapchain != nullptr) vulkan::get_device().destroySwapchainKHR(swapchain);
    vulkan::get_instance().destroySurfaceKHR(surface, nullptr, vulkan::get_dld());
}

void qt_vulkan_window::resizeEvent(QResizeEvent *ev) {
    ELM_DEBUG("Editor renderer resized to {0}x{1}", ev->size().width(), ev->size().height());
    if (swapchain != nullptr) if (swapchain != nullptr) vulkan::get_device().destroySwapchainKHR(swapchain);
    vulkan::swapchain_creation_info info = vulkan::query_swapchain_info(surface, this->width(), this->height());
    swapchain = vulkan::create_swapchain(info);
}
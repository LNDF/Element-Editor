#include "vulkan_window.h"

#include <editor/editor.h>

using namespace element;

qt_vulkan_window::qt_vulkan_window(QWindow* parent) : QWindow(parent) {
    this->setSurfaceType(QSurface::VulkanSurface);
    
}

qt_vulkan_window::~qt_vulkan_window() {
    vulkan::get_instance().destroySurfaceKHR(surface);
}

void qt_vulkan_window::check_and_init() {
    if (!is_initialized) {
        is_initialized = true;
        this->setVulkanInstance(vulkan_qt::get_vulkan_instance());
        surface = QVulkanInstance::surfaceForWindow(this);
        if (!vulkan::is_device_created()) {
            vulkan::init_device(surface);
        }
    }
}

void qt_vulkan_window::exposeEvent(QExposeEvent* event) {
    if (isExposed()) {
        check_and_init();
    }
}

void qt_vulkan_window::resizeEvent(QResizeEvent* event) {

}
#include "vulkan_window.h"

#include <editor/editor.h>

using namespace element;

qt_vulkan_window::qt_vulkan_window(QWindow* parent) : QWindow(parent) {
    create();
    surface = vulkan_create_surface_from_qt(this);
    if (!vulkan::is_device_created()) {
        vulkan::init_device(surface);
    }
}

qt_vulkan_window::~qt_vulkan_window() {
    vulkan::get_instance().destroySurfaceKHR(surface, nullptr, vulkan::get_dld());
}
#pragma once

#include <graphics/vulkan.h>
#include <QWindow>

namespace element {

    vk::SurfaceKHR vulkan_create_surface_from_qt(QWindow* window);

} // namespace element

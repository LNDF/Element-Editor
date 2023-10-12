#pragma once

#include <graphics/vulkan.h>

#include <QWindow>

namespace element {
    namespace vulkan {

        vk::SurfaceKHR create_surface_from_qt(QWindow* window);

    } // namespace vulkan
} // namespace element

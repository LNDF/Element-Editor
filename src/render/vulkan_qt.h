#pragma once

#include <render/vulkan.h>

#include <QWindow>

namespace element {
    namespace vulkan {

        void vulkan_init_qt_extensions();
        vk::SurfaceKHR create_surface_from_qt(QWindow* window);

    } // namespace vulkan
} // namespace element

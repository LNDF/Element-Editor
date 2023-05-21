#pragma once

#include <QWindow>
#include <render/vulkan_qt.h>

namespace element {
    
    class qt_vulkan_window : public QWindow {
        Q_OBJECT
        private:
            vk::SurfaceKHR surface;
        public:
            qt_vulkan_window(QWindow* parent = nullptr);
            virtual ~qt_vulkan_window();

    };

} // namespace element

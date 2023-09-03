#pragma once

#include <QWindow>
#include <graphics/vulkan_qt.h>

namespace element {
    
    class qt_vulkan_window : public QWindow {
        Q_OBJECT
        private:
            vk::SurfaceKHR surface;
            vk::SwapchainKHR swapchain = nullptr;
        public:
            qt_vulkan_window(QWindow* parent = nullptr);
            virtual ~qt_vulkan_window();

            void resizeEvent(QResizeEvent *ev) override;
    };

} // namespace element

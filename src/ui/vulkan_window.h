#pragma once

#include <QWindow>
#include <graphics/vulkan_qt.h>
#include <graphics/vulkan_swapchain.h>

namespace element {
    
    class qt_vulkan_window : public QWindow {
        Q_OBJECT
        private:
            bool swapchain_created = false;
            vk::SurfaceKHR surface;
            vulkan::swapchain_info swapchain;
        protected:
            void resizeEvent(QResizeEvent *ev) override;
        public:
            qt_vulkan_window(QWindow* parent = nullptr);
            virtual ~qt_vulkan_window();disc
    };

} // namespace element

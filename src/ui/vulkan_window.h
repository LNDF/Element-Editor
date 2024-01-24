#pragma once

#include <QWindow>
#include <render/vulkan_qt.h>
#include <render/vulkan_swapchain.h>

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
            virtual ~qt_vulkan_window();
        public slots:
            void render();
    };

} // namespace element

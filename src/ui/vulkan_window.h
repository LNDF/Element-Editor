#pragma once

#include <QWindow>
#include <render/vulkan_qt.h>
#include <render/vulkan_swapchain.h>

namespace element {
    
    enum qt_vulkan_window_drag {
        none,
        rotate,
        translate
    };

    class qt_vulkan_window : public QWindow {
        Q_OBJECT
        private:
            bool swapchain_created = false;
            vk::SurfaceKHR surface;
            vulkan::swapchain_info swapchain;
            qt_vulkan_window_drag drag_status;
            QPoint pre_drag_pos;
        protected:
            void resizeEvent(QResizeEvent *ev) override;
            void mouseMoveEvent(QMouseEvent *ev) override;
            void mousePressEvent(QMouseEvent *ev) override;
            void mouseReleaseEvent(QMouseEvent *ev) override;
        public:
            qt_vulkan_window(QWindow* parent = nullptr);
            virtual ~qt_vulkan_window();
        public slots:
            void render();
    };

} // namespace element

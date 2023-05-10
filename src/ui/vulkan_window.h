#pragma once

#include <QWindow>
#include <render/vulkan_qt.h>

namespace element {
    
    class qt_vulkan_window : public QWindow {
        Q_OBJECT
        private:
            bool is_initialized = false;
            vk::SurfaceKHR surface;

            void check_and_init();
            void exposeEvent(QExposeEvent *event) override;
            void resizeEvent(QResizeEvent *event) override;

        public:
            qt_vulkan_window(QWindow* parent = nullptr);
            virtual ~qt_vulkan_window();

            inline vk::SurfaceKHR get_surface() {return surface;}
    };

} // namespace element

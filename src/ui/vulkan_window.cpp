#include "vulkan_window.h"

#include <editor/editor.h>
#include <core/log.h>
#include <render/vulkan_swapchain.h>
#include <render/render.h>
#include <scenegraph/editor_camera_node_storage.h>
#include <glm/glm.hpp>

#include <QResizeEvent>

using namespace element;

qt_vulkan_window::qt_vulkan_window(QWindow* parent) : QWindow(parent), drag_status(qt_vulkan_window_drag::none) {
    create();
    surface = vulkan::create_surface_from_qt(this);
    if (!vulkan::device_initialized) {
        vulkan::init_device(surface);
        render::init_renderer();
    }
}

qt_vulkan_window::~qt_vulkan_window() {
    if (swapchain_created) vulkan::destroy_swapchain(swapchain);
    vulkan::instance.destroySurfaceKHR(surface, nullptr, vulkan::dld);
    swapchain_created = false;
}
#include <scenegraph/editor_scene_loader.h>
void qt_vulkan_window::resizeEvent(QResizeEvent* ev) {
    if (ev->size() == ev->oldSize()) return;
    ELM_DEBUG("Editor renderer resized to {0}x{1}", ev->size().width(), ev->size().height());
    bool first_time = !swapchain_created;
    if (swapchain_created) {
        render::unselect_swapchain();
        vulkan::destroy_swapchain(swapchain);
    }
    vulkan::swapchain_creation_info info = vulkan::query_swapchain_info(surface, ev->size().width(), ev->size().height());
    swapchain = vulkan::create_swapchain(info);
    render::select_swapchain(swapchain);
    swapchain_created = true;
    if (first_time) {
        scenegraph::open_scene("test.scene");
    }
    render();
}

void qt_vulkan_window::mouseMoveEvent(QMouseEvent* ev) {
    if (drag_status != qt_vulkan_window_drag::none) {
        QPoint pos = QCursor::pos();
        QPoint center = mapToGlobal(QPoint(width() / 2, height() / 2));
        if (pos == center) return;
        QCursor::setPos(center);
        scenegraph::transform& trans = scenegraph::get_editor_camera()->get_transform();
        float dx = (float) (pos.x() - center.x()) / -width();
        float dy = (float) (pos.y() - center.y()) / height();
        if (drag_status == qt_vulkan_window_drag::translate) {
            glm::vec3 pos = trans.get_position();
            pos += dx * trans.get_right();
            pos += dy * trans.get_up();
            trans.set_position(pos);
        } else if (drag_status == qt_vulkan_window_drag::rotate) {
            trans.rotate_axis(trans.get_right(), dy);
            glm::vec3 up(0, 1, 0);
            if (trans.get_up().y > 0) up *= -1;
            trans.rotate_axis(up, dx);
        }
        render();
    }
}

void qt_vulkan_window::mousePressEvent(QMouseEvent* ev) {
    if (drag_status == qt_vulkan_window_drag::none) {
        if (ev->button() == Qt::RightButton) {
            drag_status = qt_vulkan_window_drag::rotate;
            pre_drag_pos = QCursor::pos();
            setCursor(Qt::BlankCursor);
            QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));
        } else if (ev->button() == Qt::MiddleButton) {
            drag_status = qt_vulkan_window_drag::translate;
            pre_drag_pos = QCursor::pos();
            setCursor(Qt::BlankCursor);
            QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));
        }
    }
}

void qt_vulkan_window::mouseReleaseEvent(QMouseEvent* ev) {
    if ((drag_status == qt_vulkan_window_drag::translate && ev->button() == Qt::MiddleButton) ||
        (drag_status == qt_vulkan_window_drag::rotate && ev->button() == Qt::RightButton)) {
        drag_status = qt_vulkan_window_drag::none;
        QCursor::setPos(pre_drag_pos);
        setCursor(Qt::ArrowCursor);
    }
}

void qt_vulkan_window::render() {
    render::render_screen_safe();
}
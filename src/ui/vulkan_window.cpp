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
    QString platform = QGuiApplication::platformName();
    can_set_cursor = !platform.startsWith("wayland");
    if (!can_set_cursor) {
        ELM_WARN("This platform ({}) doesn't support moving the mouse cursor", platform.toStdString());
    }
}

void qt_vulkan_window::center_cursor() {
    if (can_set_cursor) {
        drag_last_pos = mapToGlobal(QPoint(width() / 2, height() / 2));
        QCursor::setPos(drag_last_pos);
    } else {
        drag_last_pos = QCursor::pos();
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
        if (pos == drag_last_pos) return;
        scenegraph::transform& trans = scenegraph::get_editor_camera()->get_transform();
        float dx = (float) (pos.x() - drag_last_pos.x()) / -width();
        float dy = (float) (pos.y() - drag_last_pos.y()) / height();
        center_cursor();
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
            if (can_set_cursor) setCursor(Qt::BlankCursor);
            center_cursor();
        } else if (ev->button() == Qt::MiddleButton) {
            drag_status = qt_vulkan_window_drag::translate;
            pre_drag_pos = QCursor::pos();
            if (can_set_cursor) setCursor(Qt::BlankCursor);
            center_cursor();
        }
    }
}

void qt_vulkan_window::mouseReleaseEvent(QMouseEvent* ev) {
    if ((drag_status == qt_vulkan_window_drag::translate && ev->button() == Qt::MiddleButton) ||
        (drag_status == qt_vulkan_window_drag::rotate && ev->button() == Qt::RightButton)) {
        drag_status = qt_vulkan_window_drag::none;
        if (can_set_cursor) QCursor::setPos(pre_drag_pos);
        setCursor(Qt::ArrowCursor);
    }
}

void qt_vulkan_window::render() {
    render::render_screen_safe();
}
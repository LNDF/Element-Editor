#include "element_editor.h"

#include <QCloseEvent>
#include <core/log.h>
#include <core/engine.h>
#include <event/core_events.h>
#include <event/event.h>
#include <ui/vulkan_window.h>

using namespace element::ui;

element_editor::element_editor() {
    setupUi(this);
    game_window = new qt_vulkan_window();
    game_window_container = QWidget::createWindowContainer(game_window);
    render_canvas->layout()->addWidget(game_window_container);
    
}

element_editor::~element_editor() {
    delete game_window_container;
}

void element_editor::closeEvent(QCloseEvent* event) {
    event->ignore();
    events::close close_event;
    if (event_manager::send_event(close_event)) {
        event->accept();
    }
}
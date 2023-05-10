#include "element_editor.h"

#include <core/log.h>
#include <core/engine.h>
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
    delete game_window;
}
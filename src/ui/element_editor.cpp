#include "element_editor.h"

#include <core/log.h>
#include <core/engine.h>
#include <render/display.h>
#include <render/qt_display.h>
#include <render/opengl_qt/qt_element_gl_widget.h>

using namespace element::ui;

element_editor::element_editor() {
    setupUi(this);
    QWidget* game_widget = nullptr; //Ownership transfered
    if (engine::settings.renderer == opengl) {
        ELM_DEBUG("Creating Qt widgets for OpenGL...");
        game_widget = new qt_element_gl_widget();
    } else {
        ELM_ERROR("No rendering API specified. Skipping render widget creation.");
    }
    render_canvas->layout()->addWidget(game_widget);
    qt_display_set_game_widget(game_widget);
    // create display by default
    create_display();
}

element_editor::~element_editor() {
    
}
#include "qt_opengl_context.h"

#include <core/log.h>
#include <core/engine.h>
#include <render/qt_display.h>

#include <QOpenGLWidget>

using namespace element;

qt_opengl_rendering_context::~qt_opengl_rendering_context() {}

void qt_opengl_rendering_context::swap_buffers() {
    //Don't do anything. Let Qt swap buffers.
}

void qt_opengl_rendering_context::make_current() {
    QWidget* game_widget = qt_display_get_game_widget();
    if (game_widget == nullptr) {
        ELM_WARN("Tried to make context current bat the game Qt Widget is null");
        return;
    }
    if (engine::settings.renderer != OPENGL) {
        ELM_WARN("Tried to make context current but OpenGL is not set as the rendering API.");
        return;
    }
    QOpenGLWidget* gl_game_widget = (QOpenGLWidget*) game_widget;
    gl_game_widget->makeCurrent();
}
#include "qt_element_gl_widget.h"

#include <render/opengl/loader.h>

using namespace element;

void qt_element_gl_widget::paintGL() {
    //TODO: call renderer
}

void qt_element_gl_widget::initializeGL() {
    opengl_load();
}

void qt_element_gl_widget::resizeGL(int w, int h) {
    //TODO: resize???
}
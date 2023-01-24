#include "element_editor.h"

using namespace element::ui;

element_editor::element_editor() : render_opengl(nullptr) {
    setupUi(this);
    //for now, only OpenGL
    render_opengl = new QOpenGLWidget(render_canvas);
    render_canvas->layout()->addWidget(render_opengl);
}

element_editor::~element_editor() {
    if (render_opengl != nullptr) delete render_opengl;
}
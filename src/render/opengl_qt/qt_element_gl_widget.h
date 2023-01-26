#pragma once

#include <QOpenGLWidget>

namespace element {
    class qt_element_gl_widget : public QOpenGLWidget {
        protected:
            void paintGL() final override;
            void initializeGL() final override;
            void resizeGL(int w, int h) final override;
    };
} // namespace element

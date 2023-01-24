#pragma once

#include <QMainWindow>
#include <QCloseEvent>
#include <QOpenGLWidget>

#include "ui_element_editor.h"

namespace element {
    namespace ui {
        
        class element_editor : public QMainWindow, private Ui::element_editor {
            Q_OBJECT
            public:
                element_editor();
                virtual ~element_editor();
            public slots:

            private:
                QOpenGLWidget* render_opengl;
        };

    } // namespace ui
} // namespace element

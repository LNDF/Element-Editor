#pragma once

#include <QMainWindow>
#include <QCloseEvent>
#include <QOpenGLWidget>
#include <event/event.h>
#include <event/core_events.h>

#include "ui_element_editor.h"

namespace element {
    namespace ui {
        
        class element_editor : public QMainWindow, private Ui::element_editor {
            Q_OBJECT
            public:
                element_editor();
                virtual ~element_editor();
                virtual void closeEvent(QCloseEvent* event);
            public slots:

            private:
                QOpenGLWidget* render_opengl;
        };

    } // namespace ui
} // namespace element

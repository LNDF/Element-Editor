#pragma once

#include <QMainWindow>

#include "ui_element_editor.h"

namespace element {
    namespace ui {
        
        class element_editor : public QMainWindow, private Ui::element_editor {
            Q_OBJECT
            public:
                element_editor();
            public slots:
                
        };

    } // namespace ui
} // namespace element

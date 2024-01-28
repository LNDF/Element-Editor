#pragma once

#include <render/vulkan.h>

#include <QMainWindow>

#include "ui_element_editor.h"

namespace element {
    namespace ui {
        
        class element_editor : public QMainWindow, private Ui::element_editor {
            Q_OBJECT
            private:
                QWindow* game_window;
                QWidget* game_window_container;
            protected:
                void closeEvent(QCloseEvent* event) override;
            public:
                element_editor();
                virtual ~element_editor();

                void load_scene();

                inline QWindow* get_game_window() {return game_window;}
                inline QWidget* get_game_window_container() {return game_window_container;}
        };

    } // namespace ui
} // namespace element

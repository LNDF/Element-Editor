#pragma once

#include <QApplication>
#include <ui/element_editor.h>

namespace element {
    namespace editor {
        extern QApplication* qt_app;
        extern ui::element_editor* main_window;

        void execute_in_editor_thread(std::function<void()> call);
        void run_editor(QApplication* app);
    }
} // namespace element

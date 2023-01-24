#pragma once

#include <QApplication>
#include <ui/element_editor.h>

namespace element {
    class editor {
        public:
            static void run_editor(char* exe_name);
            inline static QApplication* get_qt_app() {return qt_app;}
            inline static ui::element_editor* get_main_window() {return main_window;}
        private:
            static QApplication* qt_app;
            static ui::element_editor* main_window;
    };
} // namespace element

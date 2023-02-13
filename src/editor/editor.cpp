#include "editor.h"

#include <core/log.h>

using namespace element;

QApplication* editor::qt_app = nullptr;
ui::element_editor* editor::main_window = nullptr;

void editor::run_editor(char* exe_name) {
    int argc = 1;
    qt_app = new QApplication(argc, &exe_name);
    main_window = new ui::element_editor();
    main_window->show();
    ELM_DEBUG("Executing editor Qt application. Qt is taking control of the program.");
    qt_app->exec();
    delete main_window;
    delete qt_app;
    main_window = nullptr;
    qt_app = nullptr;
}
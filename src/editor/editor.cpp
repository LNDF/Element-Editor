#include "editor.h"

#include <QMetaObject>
#include <core/log.h>
#include <asset/importer.h>

using namespace element;

QApplication* editor::qt_app = nullptr;
ui::element_editor* editor::main_window = nullptr;

void editor::execute_in_editor_thread(std::function<void()> call) {
    QMetaObject::invokeMethod(main_window, call, Qt::QueuedConnection);
}

void editor::run_editor(char* exe_name) {
    int argc = 1;
    qt_app = new QApplication(argc, &exe_name);
    main_window = new ui::element_editor();
    main_window->show();
    asset_importer::start();
    asset_importer::load();
    ELM_DEBUG("Executing editor Qt application. Qt is taking control of the program.");
    qt_app->exec();
    asset_importer::save();
    asset_importer::stop();
    delete main_window;
    delete qt_app;
    main_window = nullptr;
    qt_app = nullptr;
}
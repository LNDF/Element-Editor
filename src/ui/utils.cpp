#include "utils.h"

#include <editor/editor.h>
#include <editor/project.h>
#include <QCoreApplication>
#include <QMessageBox>

using namespace element;
using namespace element::ui;

bool element::ui::file_check_exists_ui(const std::string &path) {
    if (std::filesystem::exists(project::project_assets_path / path)) {
        QMessageBox box(editor::main_window);
        box.setWindowTitle(QCoreApplication::translate("element-editor", "Can't create file or directory"));
        box.setText(QCoreApplication::translate("element-editor", "The file or directory aleady exists."));
        box.setIcon(QMessageBox::Critical);
        box.setStandardButtons(QMessageBox::Ok);
        box.exec();
        return true;
    }
    return false;
}
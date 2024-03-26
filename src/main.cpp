#include <QApplication>

#include <core/engine.h>
#include <core/log.h>
#include <editor/editor.h>
#include <editor/project.h>
#include <render/vulkan_qt.h>
#include <ui/project_properties.h>

using namespace element;

int main(int argc, char** argv) {
    ELM_SETUP_LOG;
    ELM_SET_LOG_LEVEL(ELM_LOG_LEVEL_DEBUG);
    int args = 1;
    QApplication* qt_app = new QApplication(args, argv);
    //TODO: get from program arguments...
    project::open("testing/project1");
    if (!project::exists()) {
        ELM_INFO("Creating new project...");
        project::name = "";
        project::author = "";
        project::version = "";
        ui::project_properties dialog;
        if (dialog.exec() == QDialog::Rejected) {
            ELM_INFO("User cancelled project creation. Exiting...");
            return 0;
        }
    } else {
        project::mkdir();
        project::load();
    }
    engine::settings.app_name = "Element editor";
    engine::settings.app_version = ELM_EDITOR_VERSION;
    vulkan::vulkan_init_qt_extensions();
    engine::setup();
    ELM_INFO("This is the editor. Running editor...");
    editor::run_editor(qt_app);
    engine::cleanup();
    delete qt_app;
    return 0;
}
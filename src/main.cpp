#include <QApplication>

#include <core/engine.h>
#include <core/log.h>
#include <editor/editor.h>
#include <editor/project.h>

using namespace element;

int main(int argc, char** argv) {
    ELM_INIT_LOG;
    ELM_SET_LOG_LEVEL(ELM_LOG_LEVEL_TRACE);
    int args = 1;
    QApplication* qt_app = new QApplication(args, argv);
    //TODO: get from program arguments...
    project::open("testing/project");
    if (!project::exists()) {
        ELM_INFO("Creating new project...");
        //TODO: use qt dialog...
        project::name = "test project";
        project::author = "test author";
        project::version = "1.0.0";
        project::save();
    } else {
        project::mkdir();
        project::load();
    }
    engine::settings.app_name = "Element editor";
    engine::settings.app_version = ELM_EDITOR_VERSION;
    engine::setup_engine();
    ELM_INFO("This is the editor. Running editor...");
    editor::run_editor(qt_app);
    engine::cleanup_engine();
    delete qt_app;
    return 0;
}
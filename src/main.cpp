#include <core/engine.h>
#include <core/log.h>
#include <editor/editor.h>
#include <editor/project.h>

using namespace element;

int main(int argc, char** argv) {
    ELM_INIT_LOG;
    ELM_SET_LOG_LEVEL(ELM_LOG_LEVEL_TRACE);
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
    engine_settings settings;
    settings.app_name = "Element editor";
    settings.app_version = ELM_EDITOR_VERSION;
    engine::setup_engine(settings);
    ELM_INFO("This is the editor. Running editor...");
    editor::run_editor(argv[0]);
    engine::cleanup_engine();
    return 0;
}
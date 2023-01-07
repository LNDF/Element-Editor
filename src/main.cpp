#include <core/engine.h>
#include <core/log.h>
#include <editor/editor.h>

using namespace element;

int main(int argc, char** argv) {
    engine_settings settings;
    settings.app_name = "Element editor";
    settings.app_version = ELM_EDITOR_VERSION;
    engine::setup_engine(settings);
    ELM_INFO("This is the editor. Running editor...");
    editor::run_editor(argv[0]);
    engine::cleanup_engine();
    return 0;
}
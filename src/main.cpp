#include <core/application.h>
#include <core/log.h>
#include <event/event.h>
#include <event/core_events.h>
#include <ui/impl_ui_qt.h>

using namespace element;

int main(int argc, char** argv) {
    ui::qt_set_argv_0(argv[0]);
    application_settings settings;
    settings.app_name = "Element editor";
    settings.app_version = ELM_EDITOR_VERSION;
    application::setup_engine(settings);
    application::start();
    application::cleanup_engine();
    return 0;
}
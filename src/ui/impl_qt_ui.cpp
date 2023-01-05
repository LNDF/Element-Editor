//this is a stub

#include "impl_ui_qt.h"

#include <QApplication>
#include <ui/ui.h>
#include <ui/element_editor.h>
#include <core/log.h>
#include <event/event.h>
#include <event/core_events.h>

static QApplication* qt_app;
static char* argv_0;
static int qt_argc = 1;
static element::ui::element_editor* main_window;

using namespace element;

static bool qt_app_process_events(const events::update& ) {
    qt_app->processEvents();
    return true;
}

void ui::qt_set_argv_0(char* argv0) {
    argv_0 = argv0;
}

void ui::init_backend() {
    ELM_DEBUG("Init Qt backend, version {0}...", QT_VERSION_STR);
    qt_app = new QApplication(qt_argc, &argv_0);
    event_manager::register_listener<events::update>(qt_app_process_events);
}

void ui::cleanup_backend() {
    ELM_DEBUG("Stopping Qt backend...");
    delete qt_app;
}

void ui::start_ui() {
    ELM_DEBUG("Showing main window...");
    main_window = new element_editor();
    main_window->show();
}

void ui::stop_ui() {
    ELM_DEBUG("Cleanup main window");
    delete main_window;
}

int ui::get_surface_width() {
    ELM_WARN("STUBBED");
    return 0;
}

int ui::get_surface_height() {
    ELM_WARN("STUBBED");
    return 0;
}

void ui::set_surface_size(int width, int height) {
    ELM_WARN("STUBBED");
}

void ui::set_pointer_lock(bool lock) {
    ELM_WARN("STUBBED");
}

bool ui::is_pointer_locked() {
    ELM_WARN("STUBBED");
    return false;
}

void ui::set_vsync(bool lock) {
    ELM_WARN("STUBBED");
}

bool ui::is_vsync_enabled() {
    ELM_WARN("STUBBED");
    return false;
}
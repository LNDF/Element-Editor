#include <ui/ui.h>
#include <core/log.h>

//SEMI STUB

using namespace element;

void ui::init_backend() {
    //nothing to do...
}

void ui::cleanup_backend() {
    //nothing to do...
}

void ui::start_ui() {
    //nothing to do...
}

void ui::stop_ui() {
    //nothing to do...
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
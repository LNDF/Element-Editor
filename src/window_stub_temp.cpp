//this is a stub

#include <window/window.h>
#include <core/log.h>

static std::string empty = "";

using namespace element;

void window::init_backend() {
    ELM_WARN("STUBBED");
}

void window::cleanup_backend() {
    ELM_WARN("STUBBED");
}

void window::create_window(const std::string& title, int width, int height, int x, int y) {
    ELM_WARN("STUBBED");
}

void window::close_window() {
    ELM_WARN("STUBBED");
}

void window::set_title(const std::string& title) {
    ELM_WARN("STUBBED");
}

const std::string& window::get_title() {
    ELM_WARN("STUBBED");
    return empty;
}

int window::get_width() {
    ELM_WARN("STUBBED");
    return 0;
}

int window::get_height() {
    ELM_WARN("STUBBED");
    return 0;
}

int window::get_x_position() {
    ELM_WARN("STUBBED");
    return 0;
}

int window::get_y_position() {
    ELM_WARN("STUBBED");
    return 0;
}

void window::set_size(int width, int height) {
    ELM_WARN("STUBBED");
}

void window::move_window(int x, int y) {
    ELM_WARN("STUBBED");
}

void window::set_pointer_lock(bool lock) {
    ELM_WARN("STUBBED");
}

bool window::is_pointer_locked() {
    ELM_WARN("STUBBED");
    return false;
}

void window::set_vsync(bool lock) {
    ELM_WARN("STUBBED");
}

bool window::is_vsync_enabled() {
    ELM_WARN("STUBBED");
    return false;
}
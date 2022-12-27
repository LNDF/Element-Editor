//this is a stub

#include <window/window.h>
#include <core/log.h>

static std::string empty = "";

using namespace element::window;

void init_backend() {
    ELM_WARN("STUBBED");
}

void cleanup_backend() {
    ELM_WARN("STUBBED");
}

void create_window(const std::string& title, int width, int height, int x, int y) {
    ELM_WARN("STUBBED");
}

void close_window() {
    ELM_WARN("STUBBED");
}

void set_title(const std::string& title) {
    ELM_WARN("STUBBED");
}

const std::string& get_title() {
    ELM_WARN("STUBBED");
    return empty;
}

int get_width() {
    ELM_WARN("STUBBED");
    return 0;
}

int get_height() {
    ELM_WARN("STUBBED");
    return 0;
}

int get_x_position() {
    ELM_WARN("STUBBED");
    return 0;
}

int get_y_position() {
    ELM_WARN("STUBBED");
    return 0;
}

void set_size(int width, int height) {
    ELM_WARN("STUBBED");
}

void move_window(int x, int y) {
    ELM_WARN("STUBBED");
}

void set_pointer_lock(bool lock) {
    ELM_WARN("STUBBED");
}

bool is_pointer_locked() {
    ELM_WARN("STUBBED");
    return false;
}

void set_vsync(bool lock) {
    ELM_WARN("STUBBED");
}

bool is_vsync_enabled() {
    ELM_WARN("STUBBED");
    return false;
}
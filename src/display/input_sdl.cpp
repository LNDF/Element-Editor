#include <display/input.h>

#include <SDL.h>

using namespace element;

std::pair<int, int> display::get_cursor_position() {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return {x, y};
}

void display::grab_cursor() {
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void display::release_cursor() {
    SDL_SetRelativeMouseMode(SDL_FALSE);
}

void display::hide_cursor() {
    SDL_ShowCursor(SDL_DISABLE);
}

void display::show_cursor() {
    SDL_ShowCursor(SDL_ENABLE);
}
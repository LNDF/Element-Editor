#include <display/events.h>

#include <core/core_events.h>
#include <event/event.h>
#include <render/render.h>
#include <SDL2/SDL.h>

using namespace element;

static void process_mouse_down(const SDL_MouseButtonEvent& event) {
    events::mouse_down mouse_down_event;
    mouse_down_event.button = static_cast<display::mouse_button>(event.button);
    event_manager::send_event(mouse_down_event);
}

static void process_mouse_up(const SDL_MouseButtonEvent& event) {
    events::mouse_up mouse_up_event;
    mouse_up_event.button = static_cast<display::mouse_button>(event.button);
    event_manager::send_event(mouse_up_event);
}

static void process_key_down(const SDL_KeyboardEvent& event) {
    events::key_down key_down_event;
    key_down_event.key = static_cast<display::key>(event.keysym.sym);
    event_manager::send_event(key_down_event);
}

static void process_key_up(const SDL_KeyboardEvent& event) {
    events::key_up key_up_event;
    key_up_event.key = static_cast<display::key>(event.keysym.sym);
    event_manager::send_event(key_up_event);
}

static void process_mouse_move(const SDL_MouseMotionEvent& event) {
    events::mouse_move mouse_move_event;
    mouse_move_event.x = event.x;
    mouse_move_event.y = event.y;
    event_manager::send_event(mouse_move_event);
}

static void process_window_resize(const SDL_WindowEvent& event) {
    events::window_resize window_resize_event;
    window_resize_event.width = event.data1;
    window_resize_event.height = event.data2;
    event_manager::send_event(window_resize_event);
}

static bool poll(events::update& update) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                events::close close_event;
                event_manager::send_event(close_event);
                break;
            case SDL_MOUSEBUTTONDOWN:
                process_mouse_down(event.button);
                break;
            case SDL_MOUSEBUTTONUP:
                process_mouse_up(event.button);
                break;
            case SDL_KEYDOWN:
                process_key_down(event.key);
                break;
            case SDL_KEYUP:
                process_key_up(event.key);
                break;
            case SDL_MOUSEMOTION:
                process_mouse_move(event.motion);
                break;
            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                        process_window_resize(event.window);
                        break;
                }
                break;
        }
    }
    render::render_screen();
    return true;
}

ELM_REGISTER_EVENT_CALLBACK(events::update, poll, event_callback_priority::highest);
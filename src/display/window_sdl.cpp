#include <display/window.h>

#include <core/log.h>
#include <render/render.h>
#include <render/vulkan.h>
#include <render/vulkan_swapchain.h>
#include <SDL.h>
#include <SDL_vulkan.h>

using namespace element;

static SDL_Window* window = nullptr;
static vk::SurfaceKHR surface;
static vulkan::swapchain_info swapchain;

void display::open_window() {
    ELM_DEBUG("Opening SDL window...");
    window = SDL_CreateWindow("Element", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN);
    VkSurfaceKHR tmp_surface;
    if (!SDL_Vulkan_CreateSurface(window, vulkan::instance, &tmp_surface)) {
        ELM_CRITICAL("Failed to create Vulkan surface: {0}", SDL_GetError());
        return;
    }
    surface = tmp_surface;
    if (!vulkan::device_initialized) {
        vulkan::init_device(surface);
        render::init_renderer();
    }
    vulkan::swapchain_creation_info info = vulkan::query_swapchain_info(surface, 800, 600);
    swapchain = vulkan::create_swapchain(info);
    render::select_swapchain(swapchain);
}

void display::close_window() {
    ELM_DEBUG("Closing SDL window...");
    render::unselect_swapchain();
    vulkan::destroy_swapchain(swapchain);
    vulkan::instance.destroySurfaceKHR(surface, nullptr, vulkan::dld);
    SDL_DestroyWindow(window);
}

void display::set_window_title(const std::string& title) {
    SDL_SetWindowTitle(window, title.c_str());
}

void display::set_window_size(int width, int height) {
    SDL_SetWindowSize(window, width, height);
}

void display::set_window_position(int x, int y) {
    SDL_SetWindowPosition(window, x, y);
}

void display::set_window_mode(window_mode mode) {
    switch (mode) {
        case window_mode::windowed:
            SDL_SetWindowFullscreen(window, 0);
            break;
        case window_mode::fullscreen:
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
            break;
        case window_mode::borderless:
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            break;
    }
}

void display::set_window_resizable(bool resizable) {
    SDL_SetWindowResizable(window, resizable ? SDL_TRUE : SDL_FALSE);
}
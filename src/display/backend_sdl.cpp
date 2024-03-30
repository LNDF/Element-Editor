#include <display/backend.h>

#include <core/log.h>
#include <SDL.h>
#include <SDL_vulkan.h>
#include <render/vulkan.h>

static const char** instance_extensions;
static std::uint32_t instance_extension_count;

using namespace element;

void display::init_backend() {
    ELM_INFO("Initializing SDL...");
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Vulkan_LoadLibrary(nullptr);
    SDL_Vulkan_GetInstanceExtensions(nullptr, &instance_extension_count, nullptr);
    instance_extensions = new const char*[instance_extension_count];
    SDL_Vulkan_GetInstanceExtensions(nullptr, &instance_extension_count, instance_extensions);
    vulkan::add_instance_extensions(instance_extensions, instance_extension_count);
}

void display::cleanup_backend() {
    ELM_INFO("Cleaning up SDL...");
    delete[] instance_extensions;
    SDL_Quit();
}
#include <render/vulkan.h>
#include <core/log.h>
#include <QGuiApplication> 
#include <vector>
#include <string>

static void editor_required_vulkan_extensions(std::vector<const char*>& extensions) {
    std::string qpa = QGuiApplication::platformName().toStdString();
    extensions.push_back("VK_KHR_surface");
    ELM_DEBUG("Qt QPA is {}", qpa);
    if (qpa == "windows") {
        ELM_DEBUG("Extensions are VK_KHR_surface and VK_KHR_win32_surface");
        extensions.push_back("VK_KHR_win32_surface");
    } else if (qpa == "wayland") {
        ELM_DEBUG("Extensions are VK_KHR_surface and VK_KHR_wayland_surface");
        extensions.push_back("VK_KHR_wayland_surface");
    } else if (qpa == "xcb") {
        ELM_DEBUG("Extensions are VK_KHR_surface and VK_KHR_xcb_surface");
        extensions.push_back("VK_KHR_xcb_surface");
    } else {
        ELM_WARN("Unsupported QPA");
    }
}

ELM_REGISTER_VULKAN_REQUIRED_EXTENSION_HOOK(editor_required_vulkan_extensions);
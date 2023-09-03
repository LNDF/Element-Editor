#include "vulkan_qt.h"

#include <core/log.h>
#include <utils/platform.h>
#include <QGuiApplication> 
#include <vector>
#include <string>
#include <stdexcept>
#include <qpa/qplatformnativeinterface.h>

#if defined(ELM_PLATFORM_WINDOWS)
#include <windows.h>
#include <vulkan/vulkan_win32.h>
#elif defined(ELM_PLATFORM_LINUX)
#include <X11/Xlib.h>
#include <vulkan/vulkan_wayland.h>
#include <vulkan/vulkan_xlib.h>
struct wl_surface;
#endif

using namespace element;

static void editor_required_vulkan_extensions(std::vector<const char*>& extensions) {
    std::string qpa = QGuiApplication::platformName().toStdString();
    extensions.push_back("VK_KHR_surface");
    ELM_DEBUG("Qt QPA is {}", qpa);
    if (qpa == "windows") {
        ELM_DEBUG("Extensions are VK_KHR_surface and VK_KHR_win32_surface");
        extensions.push_back("VK_KHR_win32_surface");
    } else if (qpa == "wayland" || qpa == "wayland-egl") {
        ELM_DEBUG("Extensions are VK_KHR_surface and VK_KHR_wayland_surface");
        extensions.push_back("VK_KHR_wayland_surface");
    } else if (qpa == "xcb") {
        ELM_DEBUG("Extensions are VK_KHR_surface and VK_KHR_xlib_surface");
        extensions.push_back("VK_KHR_xlib_surface");
    } else {
        ELM_WARN("Unsupported QPA");
    }
}

ELM_REGISTER_VULKAN_REQUIRED_EXTENSION_HOOK(editor_required_vulkan_extensions);

vk::SurfaceKHR element::vulkan_create_surface_from_qt(QWindow* window) {
    VkSurfaceKHR c_surface = nullptr;
    std::string qpa = QGuiApplication::platformName().toStdString();
#if defined(ELM_PLATFORM_WINDOWS)
    if (qpa == "windows") {
        ELM_DEBUG("Creating win32 surface...");
        VkWin32SurfaceCreateInfoKHR create_info;
        create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        create_info.pNext = nullptr;
        create_info.flags = 0;
        create_info.hinstance = nullptr;
        create_info.hwnd = (HWND) window->winId();
        const PFN_vkCreateWin32SurfaceKHR func = (PFN_vkCreateWin32SurfaceKHR) vulkan::get_dld().vkGetInstanceProcAddr(vulkan::get_instance(), "vkCreateWin32SurfaceKHR");
        if (!func) {
            throw new std::runtime_error("Couldn't get address of vkCreateWin32SurfaceKHR");
        } else {
            VkResult r = func(vulkan::get_instance(), &create_info, nullptr, &c_surface);
            if (r != VK_SUCCESS) vk::detail::throwResultException((vk::Result) r, "::vkCreateWin32SurfaceKHR");
        }
    }
#elif defined(ELM_PLATFORM_LINUX)
    QPlatformNativeInterface* qpni = QGuiApplication::platformNativeInterface();
    if (qpa == "xcb") {
        ELM_DEBUG("Creating X11 xlib surface...");
        VkXlibSurfaceCreateInfoKHR create_info;
        create_info.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
        create_info.pNext = nullptr;
        create_info.flags = 0;
        create_info.dpy = (Display*) qpni->nativeResourceForWindow("display", window);
        create_info.window = (Window) window->winId();
        const PFN_vkCreateXlibSurfaceKHR func = (PFN_vkCreateXlibSurfaceKHR) vulkan::get_dld().vkGetInstanceProcAddr(vulkan::get_instance(), "vkCreateXlibSurfaceKHR");
        if (!func) {
            throw new std::runtime_error("Couldn't get address of vkCreateXlibSurfaceKHR");
        } else {
            VkResult r = func(vulkan::get_instance(), &create_info, nullptr, &c_surface);
            if (r != VK_SUCCESS) vk::detail::throwResultException((vk::Result) r, "::vkCreateXlibSurfaceKHR");
        }
    } else if (qpa == "wayland" || qpa == "wayland-egl") {
        ELM_DEBUG("Creating Wayland surface...");
        VkWaylandSurfaceCreateInfoKHR create_info;
        create_info.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
        create_info.pNext = nullptr;
        create_info.flags = 0;
        create_info.display = (wl_display*) qpni->nativeResourceForWindow("display", window);
        create_info.surface = (wl_surface*) qpni->nativeResourceForWindow("surface", window);
        const PFN_vkCreateWaylandSurfaceKHR func = (PFN_vkCreateWaylandSurfaceKHR)vulkan::get_dld().vkGetInstanceProcAddr(vulkan::get_instance(), "vkCreateWaylandSurfaceKHR");
        if (!func) {
            throw new std::runtime_error("Couldn't get address of vkCreateWaylandSurfaceKHR");
        } else {
            VkResult r = func(vulkan::get_instance(), &create_info, nullptr, &c_surface);
            if (r != VK_SUCCESS) vk::detail::throwResultException((vk::Result) r, "::vkCreateWaylandSurfaceKHR");
        }
    }
#endif
    if (c_surface == nullptr) {
        ELM_WARN("Returning null surface...");
    }
    return vk::SurfaceKHR(c_surface);
}
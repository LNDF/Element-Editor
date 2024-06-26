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

void element::vulkan::vulkan_init_qt_extensions() {
    std::string qpa = QGuiApplication::platformName().toStdString();
    std::uint32_t count;
    const char* extensions[] = {"VK_KHR_surface", nullptr};
    ELM_DEBUG("Qt QPA is {}", qpa);
    if (qpa == "windows") {
        ELM_DEBUG("Extensions are VK_KHR_surface and VK_KHR_win32_surface");
        extensions[1] = "VK_KHR_win32_surface";
    } else if (qpa == "wayland" || qpa == "wayland-egl") {
        ELM_DEBUG("Extensions are VK_KHR_surface and VK_KHR_wayland_surface");
        extensions[1] = "VK_KHR_wayland_surface";
    } else if (qpa == "xcb") {
        ELM_DEBUG("Extensions are VK_KHR_surface and VK_KHR_xlib_surface");
        extensions[1] = "VK_KHR_xlib_surface";
    } else {
        ELM_WARN("Unsupported QPA");
    }
    count = extensions[1] == nullptr ? 1 : 2;
    vulkan::add_instance_extensions(extensions, count);
}

vk::SurfaceKHR element::vulkan::create_surface_from_qt(QWindow* window) {
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
        const PFN_vkCreateWin32SurfaceKHR func = (PFN_vkCreateWin32SurfaceKHR) dld.vkGetInstanceProcAddr(instance, "vkCreateWin32SurfaceKHR");
        if (!func) {
            throw new std::runtime_error("Couldn't get address of vkCreateWin32SurfaceKHR");
        } else {
            VkResult r = func(instance, &create_info, nullptr, &c_surface);
            if (r != VK_SUCCESS) if (r != VK_SUCCESS) throw std::runtime_error("::vkCreateWin32SurfaceKHR");
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
        const PFN_vkCreateXlibSurfaceKHR func = (PFN_vkCreateXlibSurfaceKHR) dld.vkGetInstanceProcAddr(instance, "vkCreateXlibSurfaceKHR");
        if (!func) {
            throw new std::runtime_error("Couldn't get address of vkCreateXlibSurfaceKHR");
        } else {
            VkResult r = func(instance, &create_info, nullptr, &c_surface);
            if (r != VK_SUCCESS) throw std::runtime_error("::vkCreateXlibSurfaceKHR");
        }
    } else if (qpa == "wayland" || qpa == "wayland-egl") {
        ELM_DEBUG("Creating Wayland surface...");
        VkWaylandSurfaceCreateInfoKHR create_info;
        create_info.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
        create_info.pNext = nullptr;
        create_info.flags = 0;
        create_info.display = (wl_display*) qpni->nativeResourceForWindow("display", window);
        create_info.surface = (wl_surface*) qpni->nativeResourceForWindow("surface", window);
        const PFN_vkCreateWaylandSurfaceKHR func = (PFN_vkCreateWaylandSurfaceKHR) dld.vkGetInstanceProcAddr(instance, "vkCreateWaylandSurfaceKHR");
        if (!func) {
            throw new std::runtime_error("Couldn't get address of vkCreateWaylandSurfaceKHR");
        } else {
            VkResult r = func(instance, &create_info, nullptr, &c_surface);
            if (r != VK_SUCCESS) if (r != VK_SUCCESS) throw std::runtime_error("::vkCreateWaylandSurfaceKHR");
        }
    }
#endif
    if (c_surface == nullptr) {
        ELM_WARN("Returning null surface...");
    }
    return vk::SurfaceKHR(c_surface);
}
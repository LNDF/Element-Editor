#include <utils/platform.h>

#ifdef ELM_PLATFORM_WINDOWS
#include "tracker.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <editor/project.h>
#include <editor/editor.h>
#include <asset/importer.h>
#include <core/log.h>

#define WATCHER_FLAGS FILE_NOTIFY_CHANGE_SECURITY |	FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_LAST_ACCESS | FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_FILE_NAME

static HANDLE asset_dir = nullptr;
static HANDLE stop_event = nullptr;
static bool watcher_running = false;

static std::string get_last_error_str() {
    DWORD err = GetLastError();
    if (err == 0) return std::string();
    LPSTR buff = NULL;
    DWORD size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR) &buff, 0, NULL);
    std::string ret(buff, size);
    LocalFree(buff);
    return ret;
}

static bool is_path_directory(const std::filesystem::path& path) {
    DWORD attr = GetFileAttributesW(path.c_str());
    if (attr == INVALID_FILE_ATTRIBUTES) {
        ELM_ERROR("GetFileAttributesW(): error {0}: {1}", GetLastError(), get_last_error_str());
    }
    return attr & FILE_ATTRIBUTE_DIRECTORY;
}

static void watcher_win32_process_event(FILE_NOTIFY_INFORMATION* event) {
    std::filesystem::path path = element::project::project_assets_path / event->FileName;
    ELM_WARN("is dir {0}", is_path_directory(path));
    switch (event->Action) {
        case FILE_ACTION_ADDED:
            ELM_WARN("added path {0}", path.string());
            break;
        case FILE_ACTION_REMOVED:
            ELM_WARN("remove path {0}", path.string());
            break;
        case FILE_ACTION_MODIFIED:
            ELM_WARN("modify path {0}", path.string());
            break;
        case FILE_ACTION_RENAMED_OLD_NAME:
            ELM_WARN("rename old path {0}", path.string());
            break;;
        case FILE_ACTION_RENAMED_NEW_NAME:
            ELM_WARN("rename new path {0}", path.string());
            break;
        
        default:
            break;
    }
}

static void watcher_win32_thread() {
    BYTE buffer[4096 * 8] = {0}, *ptr;
    FILE_NOTIFY_INFORMATION* event;
    bool cleanup_io = false;
    DWORD resultret = 0, waitret = 0, next = 0;
    OVERLAPPED overlapped{0};
    overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    HANDLE waits[2] = {stop_event, overlapped.hEvent};
    while (watcher_running) {
        ReadDirectoryChangesW(asset_dir, buffer, 4096 * 8, TRUE, WATCHER_FLAGS, &resultret, &overlapped, NULL);
        cleanup_io = true;
        waitret = WaitForMultipleObjects(2, waits, FALSE, INFINITE);
        switch (waitret) {
            case WAIT_OBJECT_0:
                continue;
            case WAIT_OBJECT_0 + 1:
                if (!GetOverlappedResult(asset_dir, &overlapped, &resultret, TRUE)) {
                    ELM_ERROR("GetOverlappedResult(): error {0}: {1}", GetLastError(), get_last_error_str());
                    continue;;
                }
                cleanup_io = false;
                if (resultret == 0) continue; //overflow??
                next = 0;
                ptr = buffer;
                do {
                    ptr += next;
                    event = (FILE_NOTIFY_INFORMATION*) ptr;
                    watcher_win32_process_event(event);
                    next = event->NextEntryOffset;
                } while (next != 0);
                memset(buffer, 0, 4096 * 8);
                break;
            default:
                break;
        }
    }
    if (cleanup_io) {
        CancelIo(asset_dir);
        GetOverlappedResult(asset_dir, &overlapped, &resultret, TRUE);
    }
    CloseHandle(overlapped.hEvent);
}

namespace element {
    
    std::thread asset_tracker::watcher_thread;

    void asset_tracker::start() {
        ELM_INFO("Starting asset tracker...");
        stop_event = CreateEvent(NULL, FALSE, FALSE, NULL);
        if (!stop_event) {
            ELM_ERROR("CreateEvent(): error {0}: {1}", GetLastError(), get_last_error_str());
            return;
        }
        asset_dir = CreateFileW(project::project_assets_path.c_str(), FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL);
        if (asset_dir == INVALID_HANDLE_VALUE) {
            ELM_ERROR("CreateFileW(): error {0}: {1}", GetLastError(), get_last_error_str());
            CloseHandle(stop_event);
            return;
        }
        watcher_running = true;
        watcher_thread = std::thread(watcher_win32_thread);
        ELM_INFO("Started asset tracker");
    }

    void asset_tracker::stop() {
        ELM_INFO("Stopping asset tracker...");
        watcher_running = false;
        SetEvent(stop_event);
        watcher_thread.join();
        CloseHandle(asset_dir);
        CloseHandle(stop_event);
        ELM_INFO("Stopped asset tracker");
    }

} // namespace element

#endif
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

std::filesystem::path last_remove;
std::filesystem::path last_old_rename;
bool last_remove_enabled = false;
bool last_old_rename_enabled = false;

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
    bool consumed = false;
    switch (event->Action) {
        case FILE_ACTION_ADDED:
            if (last_remove_enabled) {
                element::editor::execute_in_editor_thread([=]() {
                    element::asset_importer::tracker_path_move(last_remove, path);
                });
                consumed = true;
            }
            break;
        case FILE_ACTION_RENAMED_NEW_NAME:
            if (last_old_rename_enabled) {
                element::editor::execute_in_editor_thread([=]() {
                    element::asset_importer::tracker_path_move(last_old_rename, path);
                });
                consumed = true;
            }
            break;
    }
    if (!consumed)  {
        if (last_remove_enabled) {
            element::editor::execute_in_editor_thread([=]() {
                element::asset_importer::tracker_path_delete(path);
            });
        }
    }
    last_remove_enabled = false;
    last_old_rename_enabled = false;
    if (consumed) return;
    switch (event->Action) {
        case FILE_ACTION_ADDED:
            element::editor::execute_in_editor_thread([=]() {
                element::asset_importer::tracker_path_create(path, is_path_directory(path));
            });
            break;
        case FILE_ACTION_REMOVED:
            last_remove_enabled = true;
            last_remove = path;
            break;
        case FILE_ACTION_RENAMED_OLD_NAME:
            last_old_rename_enabled = true;
            last_old_rename = path;
            break;
        case FILE_ACTION_MODIFIED:
            if (!is_path_directory(path)) {
                element::editor::execute_in_editor_thread([=]() {
                    element::asset_importer::tracker_path_modify(path);
                });
            }
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
        waitret = WaitForMultipleObjects(2, waits, FALSE, (last_old_rename_enabled || last_remove_enabled) ? 50 : INFINITE);
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
            case WAIT_TIMEOUT:
                last_old_rename_enabled = false;
                if (last_remove_enabled) {
                    element::editor::execute_in_editor_thread([=]() {
                        element::asset_importer::tracker_path_delete(last_remove);
                    });
                    last_remove_enabled = false;
                }
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
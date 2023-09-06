#include <utils/platform.h>

#ifdef ELM_PLATFORM_LINUX
#include "tracker.h"

#include <sys/inotify.h>
#include <poll.h>
#include <unistd.h>
#include <memory.h>

#include <unordered_map>
#include <unordered_set>
#include <editor/project.h>
#include <editor/editor.h>
#include <asset/importer.h>
#include <core/log.h>
#include <errno.h>

#define IN_FLAGS IN_CREATE | IN_DELETE | IN_MOVE | IN_MOVE_SELF | IN_DELETE_SELF | IN_MODIFY

static int inotify_fd;
static int asset_wd;
static int pipe_wfd, pipe_rfd;
static bool watcher_running = false;
static std::unordered_map<int, const std::filesystem::path*> wd_path_map;
static std::unordered_map<std::filesystem::path, int> path_wd_map;
static std::unordered_map<int, std::pair<int, std::unordered_set<int>>> wdset_map;
static std::unordered_map<uint32_t, std::pair<bool, std::filesystem::path>> cookie_move_data_map;

static void bind_wd_path(int wd, const std::filesystem::path& path) {
    auto pit = path_wd_map.insert({path, wd}).first;
    const std::filesystem::path* pptr = std::addressof(pit->first);
    wd_path_map.insert({wd, pptr});
}

static int wd_from_path(const std::filesystem::path& path) {
    return path_wd_map[path];
}

static const std::filesystem::path& path_from_wd(int wd) {
    return *wd_path_map[wd];
}

static void unbind_wd_path(int wd) {
    auto pit = wd_path_map.find(wd);
    if (pit != wd_path_map.end()) {
        path_wd_map.erase(*pit->second);
        wd_path_map.erase(pit);
    }
}

static void watcher_inotify_add_watchers(const std::filesystem::path& path, int wd, int parent_wd) {
    std::pair<int, std::unordered_set<int>>& wdset = wdset_map[wd];
    wdset_map[parent_wd].second.insert(wd);
    wdset.first = parent_wd;
    for (const std::filesystem::directory_entry& subdir : std::filesystem::directory_iterator(path)) {
        const std::filesystem::path& subpath = subdir.path();
        int swd = inotify_add_watch(inotify_fd, subpath.c_str(), IN_FLAGS | IN_ONLYDIR);
        if (swd == -1) {
            if (errno == ENOTDIR) continue;
            ELM_ERROR("inotify_add_watch(): path {0}, error {1}: {2}", subpath.string(), errno, strerror(errno));
        }
        bind_wd_path(swd, subpath);
        watcher_inotify_add_watchers(subpath, swd, wd);
    }
}

static void watcher_inotify_rm_watches(int wd, bool move) {
    auto it = wdset_map.find(wd);
    if (it == wdset_map.end()) return;
    for (int swd : it->second.second) {
        watcher_inotify_rm_watches(swd, move);
    }
    wdset_map.erase(it);
    unbind_wd_path(wd);
    if (move) inotify_rm_watch(inotify_fd, wd);
    
}

inline void watcher_process_delete(const std::pair<bool, std::filesystem::path>& data) {
    if (data.first) {
        int wd = wd_from_path(data.second);
        wdset_map[wdset_map[wd].first].second.erase(wd);
        watcher_inotify_rm_watches(wd, true);
    }
    element::editor::execute_in_editor_thread([=]() {
        element::__detail::__asset_importer_tracker_path_delete(data.second);
    });
}

static void watcher_inotify_process_event(inotify_event* event) {
    if (event->len == 0) return;
    std::filesystem::path path = path_from_wd(event->wd) / event->name;
    if (event->mask & IN_MOVED_TO && !cookie_move_data_map.empty()) {
        auto it = cookie_move_data_map.find(event->cookie);
        if (it != cookie_move_data_map.end()) {
            std::filesystem::path& from = it->second.second;
            element::editor::execute_in_editor_thread([=]() {
                element::__detail::__asset_importer_tracker_path_move(from, path);
            });
            cookie_move_data_map.erase(it);
            return;
        }
    }
    if (event->mask & (IN_CREATE | IN_MOVED_TO)) {
        if (event->mask & IN_ISDIR) {
            int nwd = inotify_add_watch(inotify_fd, path.c_str(), IN_FLAGS);
            if (nwd == -1) {
                ELM_ERROR("inotify_add_watch(): path {0} error {1}: {2}", path.string(), errno, strerror(errno));
                return;
            }
            bind_wd_path(nwd, path);
            watcher_inotify_add_watchers(path, nwd, event->wd);
        }
        element::editor::execute_in_editor_thread([=]() {
            element::__detail::__asset_importer_tracker_path_create(path, event->mask & IN_ISDIR);
        });
    } else if (event->mask & IN_DELETE) {
        if (event->mask & IN_ISDIR) {
            watcher_inotify_rm_watches(wd_from_path(path), false);
        }
        element::editor::execute_in_editor_thread([=]() {
            element::__detail::__asset_importer_tracker_path_delete(path);
        });
    } else if (event->mask & IN_MOVED_FROM) {
        cookie_move_data_map.try_emplace(event->cookie, event->mask & IN_ISDIR, std::move(path));
    } else if (event->mask & (IN_DELETE_SELF | IN_MOVE_SELF)) {
        if (event->wd == asset_wd) {
            element::editor::execute_in_editor_thread([=]() {
                element::asset_importer::recreate_assets_dir();
            });
        }
    } else if (event->mask & IN_MODIFY) {
        element::editor::execute_in_editor_thread([=]() {
            element::__detail::__asset_importer_tracker_path_modify(path);
        });
    }
}

static void watcher_inotify_thread() {
    pollfd fds[2];
    inotify_event* event;
    int pollret, readret;
    uint8_t buffer[4096 * 8] = {0}, *ptr;
    fds[0].fd = pipe_rfd;
    fds[0].events = POLLIN;
    fds[1].fd = inotify_fd;
    fds[1].events = POLLIN;
    while (watcher_running) {
        pollret = poll(fds, 2, cookie_move_data_map.empty() ? -1 : 50);
        if (pollret == -1) {
            if (pollret == EINTR) continue;
            ELM_ERROR("poll()): error {0}: {1}", errno, strerror(errno));
            continue;
        } else if (pollret == 0) {
            for (const auto& [cookie, data] : cookie_move_data_map) {
                watcher_process_delete(data);
            }
            cookie_move_data_map.clear();
        } else {
            if (fds[0].revents & POLLIN) {
                read(fds[0].fd, buffer, 4096 * 8);
            }
            if (fds[1].revents & POLLIN) {
                readret = read(fds[1].fd, buffer, 4096 * 8);
                if (readret == -1) {
                    ELM_ERROR("read()): error {0}: {1}", errno, strerror(errno));
                    continue;
                }
                for (ptr = buffer; ptr < buffer + readret; ptr += sizeof(inotify_event) + event->len) {
                    event = (inotify_event*) ptr;
                    watcher_inotify_process_event(event);
                }
            }
        }
    }
}

namespace element {
    
    std::thread asset_tracker::watcher_thread;

    void asset_tracker::start() {
        int pipe_fds[2];
        ELM_INFO("Starting asset tracker...");
        inotify_fd = inotify_init();
        ELM_DEBUG("inotify fd {0}", inotify_fd);
        if (inotify_fd == -1) {
            ELM_ERROR("inotify_init(): error {0}: {1}", errno, strerror(errno));
            return;
        }
        asset_wd = inotify_add_watch(inotify_fd, project::project_assets_path.c_str(), IN_FLAGS);
        ELM_DEBUG("asset wd {0}", asset_wd);
        if (asset_wd == -1) {
            ELM_ERROR("inotify_add_watch(): path {0} error {1}: {2}", project::project_assets_path.string(), errno, strerror(errno));
            close(inotify_fd);
            return;
        }
        watcher_inotify_add_watchers(project::project_assets_path, asset_wd, -1);
        bind_wd_path(asset_wd, project::project_assets_path);
        if (pipe(pipe_fds) == -1) {
            ELM_ERROR("pipe(): error {0}: {1}", errno, strerror(errno));
            close(inotify_fd);
            close(asset_wd);
            return;
        }
        pipe_rfd = pipe_fds[0];
        pipe_wfd = pipe_fds[1];
        ELM_DEBUG("pipe rfd {0}, pipe wfd {1}", pipe_rfd, pipe_wfd);
        watcher_running = true;
        asset_tracker::watcher_thread = std::thread(watcher_inotify_thread);
        ELM_INFO("Started asset tracker");
    }

    void asset_tracker::stop() {
        ELM_INFO("Stopping asset tracker...");
        watcher_running = false;
        write(pipe_wfd, &inotify_fd, 1);
        watcher_thread.join();
        wd_path_map.clear();
        close(inotify_fd);
        ELM_INFO("Stopped asset tracker");
    }

} // namespace element

#endif
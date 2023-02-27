#pragma once

#include <thread>

namespace element {
    
    class asset_tracker {
        private:
            static std::thread watcher_thread;
        public:
            inline static const std::thread& get_watcher_thread() {return watcher_thread;}
            static void start();
            static void stop();
    };

} // namespace element

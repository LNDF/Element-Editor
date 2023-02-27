#include <utils/platform.h>

#ifdef ELM_PLATFORM_WINDOWS
#include "tracker.h"

namespace element {
    
    std::thread asset_tracker::watcher_thread;

    void asset_tracker::start() {
        
    }

    void asset_tracker::stop() {
        
    }

} // namespace element

#endif
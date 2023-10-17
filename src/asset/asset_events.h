#pragma once

#include <utils/uuid.h>

namespace element {
    namespace events {
        
        struct asset_updated {
            uuid id;
        };

        struct asset_deleted {
            uuid id;
        };
        
        

    } // namespace events
    
} // namespace element

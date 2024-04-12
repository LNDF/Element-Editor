#pragma once

#include <array>

namespace element {
    namespace plugins {
        
        extern std::array<const char*, 2> platforms;

        void load_plugins();

    } // namespace plugins    
} // namespace element
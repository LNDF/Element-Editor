#pragma once

#include <utils/uuid.h>

namespace element {
    namespace scenegraph {
        
        void open_scene(const uuid& id);
        void open_scene(const std::string& path);
        bool close_scene();

    } // namespace scenegraph
} // namespace element

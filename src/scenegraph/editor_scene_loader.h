#pragma once

#include <utils/uuid.h>

namespace element {
    namespace scenegraph {

        class scene;
        
        void open_scene(const uuid& id);
        void open_scene(const std::string& path);
        bool close_scene();
        void save_scene();

        scene* get_current_scene();

    } // namespace scenegraph
} // namespace element

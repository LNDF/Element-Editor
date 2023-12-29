#pragma once

#include <scenegraph/scene.h>
#include <serialization/scenegraph.h>
#include <resource/editor_loader.h>

namespace element {
    namespace asset_loader {

        ELM_MAKE_EDITOR_ASSET_FUNCTIONS(scene, scenegraph::scene, "scene")
        
    } // namespace asset_loader
} // namespace element

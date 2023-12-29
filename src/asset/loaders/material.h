#pragma once

#include <render/material.h>
#include <resource/editor_loader.h>
#include <serialization/render/material.h>

namespace element {
    namespace asset_loaders {
        
        ELM_MAKE_EDITOR_ASSET_FUNCTIONS(material, render::material, "material")

    } // namespace render
} // namespace element

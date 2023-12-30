#pragma once

#include <render/pipeline.h>
#include <resource/editor_loader.h>
#include <serialization/render/pipeline.h>

namespace element {
    namespace asset_loaders {
        
        ELM_MAKE_EDITOR_ASSET_FUNCTIONS(pipeline, render::pipeline_data, "pipeline")

    } // namespace render
} // namespace element

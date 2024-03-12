#include <asset/creators/asset_creators_info.h>
#include <asset/loaders/pipeline.h>
#include <string>

static void asset_create_pipeline(const std::string& path) {
    element::render::pipeline_data pipeline;
    element::asset_loaders::pipeline_text_save(path, pipeline);
}

ELM_REGISTER_ASSET_CREATOR("pipeline", "Pipeline", asset_create_pipeline)
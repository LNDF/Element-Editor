#include <asset/creators/asset_creators_info.h>
#include <asset/loaders/pipeline.h>
#include <utils/uuid.h>
#include <string>

static void asset_create_pipeline(const std::string& path) {
    element::render::pipeline_data pipeline;
    pipeline.vert_id = element::uuid::null();
    pipeline.frag_id = element::uuid::null();
    element::asset_loaders::pipeline_text_save(path, pipeline);
}

ELM_REGISTER_ASSET_CREATOR("pipeline", "Pipeline", asset_create_pipeline)
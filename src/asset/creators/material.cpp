#include <asset/creators/asset_creators_info.h>
#include <asset/loaders/material.h>
#include <utils/uuid.h>
#include <string>

static void asset_create_material(const std::string& path) {
    element::render::material material;
    material.set_pipeline_id(element::uuid::null());
    element::asset_loaders::material_text_save(path, material);
}

ELM_REGISTER_ASSET_CREATOR("material", "Material", asset_create_material)
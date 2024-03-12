#include <asset/creators/asset_creators_info.h>
#include <asset/loaders/material.h>
#include <string>

static void asset_create_material(const std::string& path) {
    element::render::material material;
    element::asset_loaders::material_text_save(path, material);
}

ELM_REGISTER_ASSET_CREATOR("material", "Material", asset_create_material)
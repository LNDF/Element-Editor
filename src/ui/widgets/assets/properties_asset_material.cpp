#include "properties_asset_material.h"

#include <core/fs.h>
#include <asset/loaders/material.h>

using namespace element::ui;

properties_asset_material::properties_asset_material(const uuid& id, QWidget* parent) : properties_asset(parent), id(id) {
    material = new properties_asset_material_form(&data, this);
    material->setObjectName("material");
    layout()->addWidget(material);
    connect(material, SIGNAL(values_changed()), this, SLOT(load_material_properties()), Qt::ConnectionType::DirectConnection);
    connect(material, SIGNAL(values_changed()), this, SLOT(enable_save()), Qt::ConnectionType::DirectConnection);
}

void properties_asset_material::load_material_properties() {
    //TODO
}

void properties_asset_material::save_values() {
    const fs_resource_info& info = fs::get_resource_info(id);
    if (info.path.empty()) return;
    asset_loaders::material_text_save(info.path, data);
    properties_asset::save_values();
}

void properties_asset_material::load_values() {
    material_loaded = false;
    const fs_resource_info& info = fs::get_resource_info(id);
    if (info.path.empty()) return;
    auto asset = asset_loaders::material_text_load(info.path);
    if (asset == std::nullopt) return;
    data = *asset;
    material->load_values();
    properties_asset::load_values();
    material_loaded = true;
}

ELM_REGISTER_ASSET_PROPERTIES_CONTAINER("material", properties_asset_material);
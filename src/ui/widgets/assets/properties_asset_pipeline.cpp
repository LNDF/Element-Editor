#include "properties_asset_pipeline.h"

#include <core/fs.h>
#include <asset/loaders/pipeline.h>

using namespace element::ui;

properties_asset_pipeline::properties_asset_pipeline(const uuid& id, QWidget* parent) : properties_asset(parent), id(id) {
    pipeline = new properties_asset_pipeline_form(&data, this);
    pipeline->setObjectName("pipeline");
    layout()->addWidget(pipeline);
    connect(pipeline, SIGNAL(values_changed()), this, SLOT(enable_save()), Qt::ConnectionType::DirectConnection);
}

void properties_asset_pipeline::save_values() {
    const fs_resource_info& info = fs::get_resource_info(id);
    if (info.path.empty()) return;
    asset_loaders::pipeline_text_save(info.path, data);
    properties_asset::save_values();
}

void properties_asset_pipeline::load_values() {
    const fs_resource_info& info = fs::get_resource_info(id);
    if (info.path.empty()) return;
    auto asset = asset_loaders::pipeline_text_load(info.path);
    if (asset == std::nullopt) return;
    data = *asset;
    pipeline->load_values();
    properties_asset::load_values();
}

ELM_REGISTER_ASSET_PROPERTIES_CONTAINER("pipeline", properties_asset_pipeline);
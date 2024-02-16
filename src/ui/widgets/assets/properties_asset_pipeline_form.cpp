#include "properties_asset_pipeline_form.h"

#include <QCoreApplication>

using namespace element::ui;

properties_asset_pipeline_form::properties_asset_pipeline_form(render::pipeline_data* data, QWidget* parent) : properties_standard_form(parent), data(data) {
    setTitle(QCoreApplication::translate("element-editor", "Pipeline"));
    vert_input = new asset_input("vert", this);
    vert_input->setObjectName("vert_input");
    frag_input = new asset_input("frag", this);
    frag_input->setObjectName("frag_input");
    backface_culling_input = new QCheckBox(this);
    backface_culling_input->setObjectName("backface_culling_input");
    frontface_input = new QComboBox(this);
    frontface_input->setObjectName("frontface_input");
    transparent_input = new QCheckBox(this);
    transparent_input->setObjectName("transparent_input");
    frontface_input->addItem(QCoreApplication::translate("element-editor", "Backface"));
    frontface_input->addItem(QCoreApplication::translate("element-editor", "Frontface"));
    add_property(QCoreApplication::translate("element-editor", "Vertex shader"), vert_input);
    add_property(QCoreApplication::translate("element-editor", "Fragment shader"), frag_input);
    add_property(QCoreApplication::translate("element-editor", "Backface culling"), backface_culling_input);
    add_property(QCoreApplication::translate("element-editor", "Frontface"), frontface_input);
    add_property(QCoreApplication::translate("element-editor", "Transparent"), transparent_input);
    load_values();
    connect(vert_input, SIGNAL(value_changed(uuid)), this, SLOT(set_vert(uuid)), Qt::ConnectionType::DirectConnection);
    connect(frag_input, SIGNAL(value_changed(uuid)), this, SLOT(set_frag(uuid)), Qt::ConnectionType::DirectConnection);
    connect(backface_culling_input, SIGNAL(stateChanged(int)), this, SLOT(set_backface_culling(int)), Qt::ConnectionType::DirectConnection);
    connect(frontface_input, SIGNAL(currentIndexChanged(int)), this, SLOT(set_frontface(int)), Qt::ConnectionType::DirectConnection);
    connect(transparent_input, SIGNAL(stateChanged(int)), this, SLOT(set_transparent(int)), Qt::ConnectionType::DirectConnection);
    connect(vert_input, SIGNAL(value_changed(uuid)), this, SIGNAL(values_changed()), Qt::ConnectionType::DirectConnection);
    connect(frag_input, SIGNAL(value_changed(uuid)), this, SIGNAL(values_changed()), Qt::ConnectionType::DirectConnection);
    connect(backface_culling_input, SIGNAL(stateChanged(int)), this, SIGNAL(values_changed()), Qt::ConnectionType::DirectConnection);
    connect(frontface_input, SIGNAL(currentIndexChanged(int)), this, SIGNAL(values_changed()), Qt::ConnectionType::DirectConnection);
    connect(transparent_input, SIGNAL(stateChanged(int)), this, SIGNAL(values_changed()), Qt::ConnectionType::DirectConnection);
}

void properties_asset_pipeline_form::set_vert(uuid id) {
    data->vert_id = id;
}

void properties_asset_pipeline_form::set_frag(uuid id) {
    data->frag_id = id;
}

void properties_asset_pipeline_form::set_backface_culling(int cull) {
    data->backface_culling = cull == Qt::Checked;
}

void properties_asset_pipeline_form::set_frontface(int face) {
    data->frontface = (render::culling_face) face;
}

void properties_asset_pipeline_form::set_transparent(int transparent) {
    data->transparent = transparent == Qt::Checked;
}

void properties_asset_pipeline_form::load_values() {
    vert_input->set_value(data->vert_id);
    frag_input->set_value(data->frag_id);
    backface_culling_input->setChecked(data->backface_culling);
    frontface_input->setCurrentIndex(data->frontface);
    transparent_input->setChecked(data->transparent);
}
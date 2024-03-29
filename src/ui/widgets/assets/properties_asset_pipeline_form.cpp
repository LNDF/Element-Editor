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
    connect(vert_input, SIGNAL(value_changed(uuid)), this, SIGNAL(values_changed()), Qt::ConnectionType::DirectConnection);
    connect(frag_input, SIGNAL(value_changed(uuid)), this, SIGNAL(values_changed()), Qt::ConnectionType::DirectConnection);
    connect(backface_culling_input, SIGNAL(stateChanged(int)), this, SIGNAL(values_changed()), Qt::ConnectionType::DirectConnection);
    connect(frontface_input, SIGNAL(currentIndexChanged(int)), this, SIGNAL(values_changed()), Qt::ConnectionType::DirectConnection);
    connect(transparent_input, SIGNAL(stateChanged(int)), this, SIGNAL(values_changed()), Qt::ConnectionType::DirectConnection);
}

void properties_asset_pipeline_form::save_values() {
    data->vert_id = vert_input->value();
    data->frag_id = frag_input->value();
    data->backface_culling = backface_culling_input->isChecked();
    data->frontface = (render::culling_face) frontface_input->currentIndex();
    data->transparent = transparent_input->isChecked();
}

void properties_asset_pipeline_form::load_values() {
    vert_input->set_value(data->vert_id);
    frag_input->set_value(data->frag_id);
    backface_culling_input->setChecked(data->backface_culling);
    frontface_input->setCurrentIndex(data->frontface);
    transparent_input->setChecked(data->transparent);
}
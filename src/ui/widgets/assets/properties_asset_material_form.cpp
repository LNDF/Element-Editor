#include "properties_asset_material_form.h"

#include <QCoreApplication>

using namespace element::ui;

properties_asset_material_form::properties_asset_material_form(render::material* data, QWidget* parent) : properties_standard_form(parent), data(data) {
    setTitle(QCoreApplication::translate("element-editor", "Material"));
    pipeline_input = new asset_input("pipeline", this);
    pipeline_input->setObjectName("pipeline_input");
    add_property(QCoreApplication::translate("element-editor", "Pipeline"), pipeline_input);
    connect(pipeline_input, SIGNAL(value_changed(uuid)), this, SIGNAL(values_changed()), Qt::ConnectionType::DirectConnection);
}

void properties_asset_material_form::load_values() {
    pipeline_input->set_value(data->get_pipeline_id());
}
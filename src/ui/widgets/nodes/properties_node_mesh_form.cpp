#include "properties_node_mesh_form.h"

#include <scenegraph/nodes/mesh_node.h>
#include <QCoreApplication>

using namespace element::ui;

properties_node_mesh_form::properties_node_mesh_form(const scenegraph::mesh_node_ref& node, QWidget* parent) : properties_standard_form(parent), node(node) {
    setTitle(QCoreApplication::translate("element-editor", "Mesh"));
    material_input = new asset_input("material", this);
    material_input->setObjectName("material_input");
    mesh_input = new asset_input("mesh", this);
    mesh_input->setObjectName("mesh_input");
    add_property(QCoreApplication::translate("element-editor", "Mesh"), mesh_input);
    add_property(QCoreApplication::translate("element-editor", "Material"), material_input);
    load_values();
    connect(material_input, SIGNAL(value_changed(uuid)), SLOT(set_material(uuid)), Qt::ConnectionType::DirectConnection);
    connect(mesh_input, SIGNAL(value_changed(uuid)), SLOT(set_mesh(uuid)), Qt::ConnectionType::DirectConnection);
    connect(material_input, SIGNAL(value_changed(uuid)), SIGNAL(values_changed()), Qt::ConnectionType::DirectConnection);
    connect(mesh_input, SIGNAL(value_changed(uuid)), SIGNAL(values_changed()), Qt::ConnectionType::DirectConnection);
}

void properties_node_mesh_form::set_material(uuid material) {
    if (node.exists()) {
        node->set_material(material);
    }
}

void properties_node_mesh_form::set_mesh(uuid mesh) {
    if (node.exists()) {
        node->set_mesh(mesh);
    }
}

void properties_node_mesh_form::load_values() {
    if (node.exists()) {
        mesh_input->set_value(node->get_mesh());
        material_input->set_value(node->get_material());
    }
}
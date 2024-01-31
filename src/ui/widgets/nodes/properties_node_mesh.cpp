#include "properties_node_mesh.h"

#include <scenegraph/nodes/mesh_node.h>

using namespace element::ui;

properties_node_mesh::properties_node_mesh(const scenegraph::node_ref& node, QWidget* parent) : properties_node(node, parent) {
    mesh = new properties_node_mesh_form(node, this);
    mesh->setObjectName("mesh");
    layout()->addWidget(mesh);
    connect(mesh, SIGNAL(values_changed()), this, SIGNAL(values_changed()), Qt::ConnectionType::DirectConnection);
}

void properties_node_mesh::load_values() {
    properties_node::load_values();
    mesh->load_values();
}

ELM_REGISTER_NODE_PROPERTIES_CONTAINER(element::scenegraph::mesh_node, element::ui::properties_node_mesh)
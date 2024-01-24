#include "properties_node.h"

using namespace element::ui;

properties_node::properties_node(const scenegraph::node_ref& node, QWidget* parent) : properties_container(parent) {
    transform = new properties_transform(node, this);
    connect(transform, SIGNAL(values_changed()), this, SIGNAL(values_changed()), Qt::ConnectionType::DirectConnection);
    load_values();
}

void properties_node::load_values() {
    transform->load_values();
}
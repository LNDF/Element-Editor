#include "properties_node.h"

using namespace element::ui;

std::unordered_map<std::type_index, properties_node_container_factory>& element::__detail::__ui_get_node_properties_container_map() {
    static std::unordered_map<std::type_index, properties_node_container_factory> map;
    return map;
}

properties_node_container_factory element::ui::get_node_properties_container_factory(std::type_index type) {
    return element::__detail::__ui_get_node_properties_container_map().at(type);
}

properties_node::properties_node(const scenegraph::node_ref& node, QWidget* parent) : properties_container(parent) {
    transform = new properties_transform_form(node, this);
    transform->setObjectName("transform");
    layout()->addWidget(transform);
    connect(transform, SIGNAL(values_changed()), this, SIGNAL(values_changed()), Qt::ConnectionType::DirectConnection);
    
}

void properties_node::load_values() {
    transform->load_values();
}

ELM_REGISTER_NODE_PROPERTIES_CONTAINER(element::scenegraph::node, element::ui::properties_node)
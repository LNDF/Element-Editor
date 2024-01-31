#include "properties_node_camera.h"

#include <scenegraph/nodes/camera_node.h>

using namespace element::ui;

properties_node_camera::properties_node_camera(const scenegraph::node_ref& node, QWidget* parent) : properties_node(node, parent) {
    camera = new properties_node_camera_form(node, this);
    camera->setObjectName("camera");
    layout()->addWidget(camera);
    connect(camera, SIGNAL(values_changed()), this, SIGNAL(values_changed()), Qt::ConnectionType::DirectConnection);
}

void properties_node_camera::load_values() {
    properties_node::load_values();
    camera->load_values();
}

ELM_REGISTER_NODE_PROPERTIES_CONTAINER(element::scenegraph::camera_node, element::ui::properties_node_camera)
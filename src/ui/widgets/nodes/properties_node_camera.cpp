#include "properties_node_camera.h"

using namespace element::ui;

properties_node_camera::properties_node_camera(const scenegraph::camera_node_ref& node, QWidget* parent) : properties_node(node.get_id(), parent) {
    camera = new properties_node_camera_form(node, this);
    camera->setObjectName("camera");
    connect(camera, SIGNAL(values_changed()), this, SIGNAL(values_changed()), Qt::ConnectionType::DirectConnection);
}

void properties_node_camera::load_values() {
    properties_node::load_values();
    camera->load_values();
}
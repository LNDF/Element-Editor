#include "properties_node_camera_form.h"

#include <scenegraph/nodes/camera_node.h>
#include <QCoreApplication>

using namespace element::ui;

properties_node_camera_form::properties_node_camera_form(const scenegraph::camera_node_ref& node, QWidget* parent) : properties_standard_form(parent) {
    setTitle(QCoreApplication::translate("element-editor", "Camera"));
    near_plane_input = new real_input(this);
    far_plane_input = new real_input(this);
    fov_input = new real_input(this);
    near_plane_input->setObjectName("near_plane");
    far_plane_input->setObjectName("far_plane");
    fov_input->setObjectName("fov");
    add_property(QCoreApplication::translate("element-editor", "Near plane"), near_plane_input);
    add_property(QCoreApplication::translate("element-editor", "Far plane"), far_plane_input);
    add_property(QCoreApplication::translate("element-editor", "Field of view"), fov_input);
    load_values();
    connect(near_plane_input, SIGNAL(valueChanged(double)), SLOT(set_near_plane(double)), Qt::ConnectionType::DirectConnection);
    connect(far_plane_input, SIGNAL(valueChanged(double)), SLOT(set_far_plane(double)), Qt::ConnectionType::DirectConnection);
    connect(fov_input, SIGNAL(valueChanged(double)), SLOT(set_fov(double)), Qt::ConnectionType::DirectConnection);
    connect(near_plane_input, SIGNAL(valueChanged(double)), SIGNAL(values_changed()), Qt::ConnectionType::DirectConnection);
    connect(far_plane_input, SIGNAL(valueChanged(double)), SIGNAL(values_changed()), Qt::ConnectionType::DirectConnection);
    connect(fov_input, SIGNAL(valueChanged(double)), SIGNAL(values_changed()), Qt::ConnectionType::DirectConnection);
}

void properties_node_camera_form::set_near_plane(double near_plane) {
    if (node.exists()) node->set_near_plane((float) near_plane);
}

void properties_node_camera_form::set_far_plane(double far_plane) {
    if (node.exists()) node->set_far_plane((float) far_plane);
}

void properties_node_camera_form::set_fov(double fov) {
    if (node.exists()) node->set_fov((float) fov);
}

void properties_node_camera_form::load_values() {
    if (node.exists()) {
        near_plane_input->setValue(node->get_near_plane());
        far_plane_input->setValue(node->get_far_plane());
        fov_input->setValue(node->get_fov());
    }
}
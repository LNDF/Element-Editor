#include "properties_transform_form.h"

#include <scenegraph/node.h>
#include <scenegraph/transform.h>
#include <glm/glm.hpp>
#include <QCoreApplication>

using namespace element::ui;

properties_transform_form::properties_transform_form(const scenegraph::node_ref& node, QWidget* parent) : properties_standard_form(parent), node(node) {
    setTitle(QCoreApplication::translate("element-editor", "Transform"));
    position_input = new vec3_input(this);
    rotation_input = new vec3_input(this);
    scale_input = new vec3_input(this);
    position_input->setObjectName("position");
    rotation_input->setObjectName("rotation");
    scale_input->setObjectName("scale");
    add_property(QCoreApplication::translate("element-editor", "Position"), position_input);
    add_property(QCoreApplication::translate("element-editor", "Rotation"), rotation_input);
    add_property(QCoreApplication::translate("element-editor", "Scale"), scale_input);
    load_values();
    connect(position_input, SIGNAL(values_changed(glm::vec3)), this, SLOT(set_position(glm::vec3)), Qt::ConnectionType::DirectConnection);
    connect(rotation_input, SIGNAL(values_changed(glm::vec3)), this, SLOT(set_rotation(glm::vec3)), Qt::ConnectionType::DirectConnection);
    connect(scale_input, SIGNAL(values_changed(glm::vec3)), this, SLOT(set_scale(glm::vec3)), Qt::ConnectionType::DirectConnection);
    connect(position_input, SIGNAL(values_changed(glm::vec3)), this, SIGNAL(values_changed()), Qt::ConnectionType::DirectConnection);
    connect(rotation_input, SIGNAL(values_changed(glm::vec3)), this, SIGNAL(values_changed()), Qt::ConnectionType::DirectConnection);
    connect(scale_input, SIGNAL(values_changed(glm::vec3)), this, SIGNAL(values_changed()), Qt::ConnectionType::DirectConnection);
}

void properties_transform_form::set_position(glm::vec3 position) {
    if (node.exists()) node->get_transform().set_position(position);
}

void properties_transform_form::set_rotation(glm::vec3 rotation) {
    if (node.exists()) node->get_transform().set_euler_rotation(glm::radians(rotation));
}

void properties_transform_form::set_scale(glm::vec3 scale) {
    if (node.exists()) node->get_transform().set_scale(scale);
}

void properties_transform_form::load_values() {
    if (node.exists()) {
        position_input->set_value(node->get_transform().get_position());
        rotation_input->set_value(glm::degrees(node->get_transform().get_euler_angles()));
        scale_input->set_value(node->get_transform().get_scale());
    }
}
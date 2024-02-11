#include "properties_node_mesh_form.h"

#include <scenegraph/nodes/mesh_node.h>
#include <QCoreApplication>

using namespace element::ui;

properties_node_mesh_form::properties_node_mesh_form(const scenegraph::mesh_node_ref& node, QWidget* parent) : properties_standard_form(parent), node(node) {
    setTitle(QCoreApplication::translate("element-editor", "Mesh"));
    add_property(QCoreApplication::translate("element-editor", "Mesh"), new QLabel("TODO"));
    add_property(QCoreApplication::translate("element-editor", "Material"), new QLabel("TODO"));
    load_values();
}

void properties_node_mesh_form::load_values() {
    if (node.exists()) {
        
    }
}
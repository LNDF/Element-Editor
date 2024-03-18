#include "properties_scene.h"

#include <scenegraph/editor_scene_loader.h>
#include <scenegraph/scene.h>
#include <QCoreApplication>

using namespace element::ui;

properties_scene::properties_scene(const uuid& scene, QWidget* parent) : properties_container(parent), scene(scene) {
    scenegraph::scene* current = scenegraph::get_current_scene();
    if (current == nullptr || current->get_id() != scene) {
        load_scene_button = new QPushButton(QCoreApplication::translate("element-editor", "Load scene"), this);
        general_properties = nullptr;
        layout()->addWidget(load_scene_button);
        connect(load_scene_button, SIGNAL(clicked()), this, SLOT(load_scene()), Qt::ConnectionType::DirectConnection);
        return;
    }
    general_properties = new properties_scene_form(scene, this);
    general_properties->setObjectName("general_properties");
    layout()->addWidget(general_properties);
    connect(general_properties, SIGNAL(values_changed()), this, SIGNAL(values_changed()), Qt::ConnectionType::DirectConnection);
}

void properties_scene::load_values() {
    if (general_properties != nullptr) {
        general_properties->load_values();
    }
}

void properties_scene::load_scene() {
    scenegraph::open_scene(scene);
}
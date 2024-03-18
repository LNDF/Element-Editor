#include "properties_scene_form.h"

#include <QCoreApplication>

using namespace element::ui;

properties_scene_form::properties_scene_form(const uuid& scene, QWidget* parent) : properties_standard_form(parent) {
    setTitle(QCoreApplication::translate("element-editor", "Scene Properties"));
    add_property(QCoreApplication::translate("element-editor", "Default camera"), new QWidget(this));
    load_values();
}

void properties_scene_form::load_values() {
    //TODO: Load values
}
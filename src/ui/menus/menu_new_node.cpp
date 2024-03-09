#include "menu_new_node.h"

#include <scenegraph/scene.h>
#include <scenegraph/node_type_info.h>
#include <scenegraph/editor_scene_loader.h>
#include <QCoreApplication>

using namespace element::ui;

menu_new_node_action::menu_new_node_action(std::type_index type, QObject* parent) : QAction(parent), type(type) {
    retranslate();
}

void menu_new_node_action::retranslate() {
    const scenegraph::node_type_info& info = scenegraph::get_node_type_info(type);
    setText(QCoreApplication::translate("element-editor", info.name.c_str()));
}

void menu_new_node_action::create_node(bool checked) {
    emit trigger(type);
}

menu_new_node::menu_new_node(QWidget* parent) : menu_new_node(nullptr, parent) {}

menu_new_node::menu_new_node(const scenegraph::node_ref& parent_node, QWidget* parent) : QMenu(parent), parent_node(parent_node) {
    for (const auto& [index, info] : scenegraph::get_node_type_info_map()) {
        menu_new_node_action* action = new menu_new_node_action(index, this);
        addAction(action);
        actions.push_back(action);
    }
}

void menu_new_node::create_node(std::type_index type, const scenegraph::node_ref& parent) {
    //TODO: open dialog here
}

void menu_new_node::retranslate() {
    for (const auto& action : actions) {
        action->retranslate();
    }
}

void menu_new_node::action_triggered(std::type_index type) {
    if (parent_node == nullptr) {
        create_node(type, scenegraph::get_current_scene()->get_root_node());
    } else {
        create_node(type, parent_node);
    }
}
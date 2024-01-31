#include "element_editor.h"

#include <QCloseEvent>
#include <core/log.h>
#include <core/engine.h>
#include <core/core_events.h>
#include <event/event.h>
#include <scenegraph/node.h>
#include <ui/models/model_scenegraph_tree.h>
#include <ui/widgets/properties_node.h>
#include <ui/vulkan_window.h>

using namespace element::ui;

element_editor::element_editor() {
    setupUi(this);
    game_window = new qt_vulkan_window();
    game_window_container = QWidget::createWindowContainer(game_window);
    render_canvas->layout()->addWidget(game_window_container);

}

element_editor::~element_editor() {
    delete game_window_container;
    if (scene_tree_model != nullptr) delete scene_tree_model;
}

void element_editor::load_properties_container() {
    current_properties_container->load_values();
    properties_scroll->setWidget(current_properties_container);
    connect(current_properties_container, SIGNAL(values_changed()), game_window, SLOT(render()), Qt::ConnectionType::DirectConnection);
    current_properties_container->show();
}

void element_editor::closeEvent(QCloseEvent* event) {
    event->ignore();
    events::close close_event;
    if (event_manager::send_event(close_event)) {
        event->accept();
    }
}

void element_editor::load_scene() {
    bool first_time = true;
    if (scene_tree_model != nullptr) {
        delete scene_tree_model;
        first_time = false;
    }
    scene_tree_model = new model_scenegraph_tree();
    scene_tree->setModel(scene_tree_model);
    if (first_time) {
        connect(scene_tree->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(node_selected(const QModelIndex&)), Qt::ConnectionType::DirectConnection);
    }
    if (current_properties_container != nullptr) {
        connect(scene_tree_model, SIGNAL(rowsMoved(const QModelIndex&, int, int, const QModelIndex&, int)), current_properties_container, SLOT(load_values()), Qt::ConnectionType::DirectConnection);
    }
}
#include <scenegraph/node.h>
void element_editor::node_selected(const QModelIndex& index) {
    if (scene_tree_model == nullptr) return;
    const scenegraph::node_ref& ref = scene_tree_model->ref_from_index(index);
    if (ref == nullptr) return;
    auto factory = get_node_properties_container_factory(std::type_index(typeid(*ref.get_node())));
    current_properties_container = factory(ref, nullptr);
    load_properties_container();
    if (scene_tree_model != nullptr) {
        connect(scene_tree_model, SIGNAL(rowsMoved(const QModelIndex&, int, int, const QModelIndex&, int)), current_properties_container, SLOT(load_values()), Qt::ConnectionType::DirectConnection);
    }
}
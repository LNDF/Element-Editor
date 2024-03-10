#include "element_editor.h"

#include <QCloseEvent>
#include <asset/asset_events.h>
#include <core/fs.h>
#include <core/log.h>
#include <core/engine.h>
#include <core/core_events.h>
#include <event/event.h>
#include <scenegraph/node.h>
#include <scenegraph/editor_scene_loader.h>
#include <ui/widgets/properties_asset.h>
#include <ui/widgets/properties_node.h>
#include <ui/vulkan_window.h>

using namespace element::ui;

element_editor::element_editor() {
    setupUi(this);
    game_window = new qt_vulkan_window();
    game_window_container = QWidget::createWindowContainer(game_window);
    render_canvas->layout()->addWidget(game_window_container);
    assets_tree_model = new model_assets_tree();
    filesystem_tree->setModel(assets_tree_model);
    connect(assets_tree_model->sourceModel(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&, const QList<int>&)), this, SLOT(properties_load_values()), Qt::ConnectionType::DirectConnection);
    connect(filesystem_tree, SIGNAL(activated(const QModelIndex&)), this, SLOT(asset_select(const QModelIndex&)), Qt::ConnectionType::DirectConnection);
    connect(scene_tree, SIGNAL(activated(const QModelIndex&)), this, SLOT(node_select(const QModelIndex&)), Qt::ConnectionType::DirectConnection);
    auto reload_on_import_event = [this](events::assets_imported& event) {
        this->properties_load_values();
        return true;
    };
    reload_on_import = event_manager::register_event_callback<events::assets_imported>(reload_on_import_event, event_callback_priority::highest);
}

element_editor::~element_editor() {
    event_manager::unregister_event_callback<events::assets_imported>(reload_on_import, event_callback_priority::highest);
    delete game_window_container;
    if (scene_tree_model != nullptr) delete scene_tree_model;
    delete assets_tree_model;
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
    if (scene_tree_model != nullptr) {
        delete scene_tree_model;
    }
    scene_tree_model = new model_scenegraph_tree();
    scene_tree->setModel(scene_tree_model);
    connect(scene_tree_model, SIGNAL(rowsMoved(const QModelIndex&, int, int, const QModelIndex&, int)), this, SLOT(properties_load_values()), Qt::ConnectionType::DirectConnection);
    action_save->setEnabled(true);
    action_reload->setEnabled(true);
    action_preview_current_scene->setEnabled(true);
    action_close->setEnabled(true);
    menu_new_node->setEnabled(true);
}

void element_editor::unload_scene() {
    if (scene_tree_model != nullptr) {
        delete scene_tree_model;
    }
    scene_tree->setModel(nullptr);
    action_save->setEnabled(false);
    action_reload->setEnabled(false);
    action_preview_current_scene->setEnabled(false);
    action_close->setEnabled(false);
    menu_new_node->setEnabled(false);
}

void element_editor::node_select(const QModelIndex& index) {
    if (scene_tree_model == nullptr) return;
    const scenegraph::node_ref& ref = scene_tree_model->ref_from_index(index);
    if (ref == nullptr) return;
    auto factory = get_node_properties_container_factory(std::type_index(typeid(*ref.get_node())));
    current_properties_container = factory(ref, nullptr);
    load_properties_container();
}

void element_editor::asset_select(const QModelIndex& index) {
    const uuid& id = assets_tree_model->id_from_index(index);
    const fs_resource_info& info = fs::get_resource_info(id);
    auto factory = get_asset_properties_container_factory(info.type);
    if (factory == nullptr) return;
    current_properties_container = factory(id, nullptr);
    load_properties_container();
}

void element_editor::properties_load_values() {
    if (current_properties_container != nullptr) {
        current_properties_container->load_values();
    }
}

void element_editor::file_save() {
    scenegraph::save_scene();
}

void element_editor::file_reload() {
    scenegraph::open_scene(scenegraph::get_current_scene()->get_id());
}

void element_editor::file_close() {
    scenegraph::close_scene();
}

void element_editor::file_preview() {
    //TODO: preview
}

void element_editor::file_preview_current_scene() {
    //TODO: preview current scene
}

void element_editor::file_properties() {
    //TODO: open properties
}

void element_editor::file_exit() {
    close();
}

void element_editor::tools_import_3d_model() {
    //TODO: import 3d model dialog
}

void element_editor::help_about() {
    //TODO: about
}

void element_editor::help_about_qt() {
    //TODO: about qt
}
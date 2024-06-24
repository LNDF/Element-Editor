#include "element_editor.h"

#include <asset/importer.h>
#include <asset/asset_events.h>
#include <core/fs.h>
#include <core/log.h>
#include <core/engine.h>
#include <core/core_events.h>
#include <editor/preview_launcher.h>
#include <editor/project.h>
#include <event/event.h>
#include <plugins/editor.h>
#include <scenegraph/node.h>
#include <scenegraph/scene_events.h>
#include <scenegraph/editor_scene_loader.h>
#include <ui/import_3d_model.h>
#include <ui/menus/menu_new_asset.h>
#include <ui/menus/menu_new_node.h>
#include <ui/project_properties.h>
#include <ui/widgets/properties_asset.h>
#include <ui/widgets/properties_node.h>
#include <ui/widgets/properties_scene.h>
#include <QCloseEvent>
#include <filesystem>

using namespace element::ui;

element_editor::element_editor() {
    setupUi(this);
    game_window = new qt_vulkan_window();
    game_window_container = QWidget::createWindowContainer(game_window);
    render_canvas->layout()->addWidget(game_window_container);
    assets_tree_model = new model_assets_tree();
    assets_tree->setModel(assets_tree_model);
    connect(assets_tree_model->sourceModel(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&, const QList<int>&)), this, SLOT(properties_load_values()), Qt::ConnectionType::DirectConnection);
    connect(assets_tree, SIGNAL(activated(const QModelIndex&)), this, SLOT(asset_select(const QModelIndex&)), Qt::ConnectionType::DirectConnection);
    connect(assets_tree, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(context_assets_tree(const QPoint&)), Qt::ConnectionType::DirectConnection);
    connect(scene_tree, SIGNAL(activated(const QModelIndex&)), this, SLOT(node_select(const QModelIndex&)), Qt::ConnectionType::DirectConnection);
    connect(scene_tree, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(context_scene_tree(const QPoint&)), Qt::ConnectionType::DirectConnection);
    auto reload_on_import_event = [this](events::assets_imported& event) {
        this->properties_load_values();
        return true;
    };
    auto node_added_event = [this](events::after_node_added& event) {
        this->get_game_window()->render();
        return true;
    };
    auto node_deleted_event = [this](events::after_node_deleted& event) {
        this->get_game_window()->render();
        return true;
    };
    reload_on_import = event_manager::register_event_callback<events::assets_imported>(reload_on_import_event, event_callback_priority::highest);
    render_on_node_added = event_manager::register_event_callback<events::after_node_added>(node_added_event, event_callback_priority::highest);
    render_on_node_deleted = event_manager::register_event_callback<events::after_node_deleted>(node_deleted_event, event_callback_priority::highest);
    connect(action_save, SIGNAL(triggered(bool)), this, SLOT(file_save()), Qt::ConnectionType::DirectConnection);
    connect(action_reload, SIGNAL(triggered(bool)), this, SLOT(file_reload()), Qt::ConnectionType::DirectConnection);
    connect(action_preview, SIGNAL(triggered(bool)), this, SLOT(file_preview()), Qt::ConnectionType::DirectConnection);
    connect(action_preview_current_scene, SIGNAL(triggered(bool)), this, SLOT(file_preview_current_scene()), Qt::ConnectionType::DirectConnection);
    connect(action_reload_plugins, SIGNAL(triggered(bool)), this, SLOT(file_reload_plugins()), Qt::ConnectionType::DirectConnection);
    connect(action_close, SIGNAL(triggered(bool)), this, SLOT(file_close()), Qt::ConnectionType::DirectConnection);
    connect(action_properties, SIGNAL(triggered(bool)), this, SLOT(file_properties()), Qt::ConnectionType::DirectConnection);
    connect(action_exit, SIGNAL(triggered(bool)), this, SLOT(file_exit()), Qt::ConnectionType::DirectConnection);
    connect(action_about, SIGNAL(triggered(bool)), this, SLOT(help_about()), Qt::ConnectionType::DirectConnection);
    connect(action_about_qt, SIGNAL(triggered(bool)), this, SLOT(help_about_qt()), Qt::ConnectionType::DirectConnection);
    connect(action_import_3d_model, SIGNAL(triggered(bool)), this, SLOT(tools_import_3d_model()), Qt::ConnectionType::DirectConnection);
    connect(action_reimport_all_assets, SIGNAL(triggered(bool)), this, SLOT(tools_reimport_all_assets()), Qt::ConnectionType::DirectConnection);
    
}

element_editor::~element_editor() {
    if (preview_process != nullptr) {
        if (preview_process->processId() != 0 && preview_process->state() != QProcess::NotRunning) {
            preview_process->terminate();
            preview_process->waitForFinished();
        } else {
            editor::cancel_preview_deferred_launch();
        }
        delete preview_process;
    }
    event_manager::unregister_event_callback<events::assets_imported>(reload_on_import, event_callback_priority::highest);
    event_manager::unregister_event_callback<events::before_node_added>(render_on_node_added, event_callback_priority::highest);
    event_manager::unregister_event_callback<events::before_node_deleted>(render_on_node_deleted, event_callback_priority::highest);
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
    properties_scroll->setWidget(new QWidget());
    current_properties_container = nullptr;
}

void element_editor::unload_scene() {
    if (scene_tree_model != nullptr) {
        delete scene_tree_model;
        scene_tree_model = nullptr;
    }
    scene_tree->setModel(nullptr);
    action_save->setEnabled(false);
    action_reload->setEnabled(false);
    action_preview_current_scene->setEnabled(false);
    action_close->setEnabled(false);
    menu_new_node->setEnabled(false);
    properties_scroll->setWidget(new QWidget());
    current_properties_container = nullptr;
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
    if (info.type == "scene") {
        current_properties_container = new properties_scene(id, nullptr);
    } else {
        auto factory = get_asset_properties_container_factory(info.type);
        if (factory == nullptr) return;
        current_properties_container = factory(id, nullptr);
    }
    load_properties_container();
}

void element_editor::properties_load_values() {
    if (current_properties_container != nullptr) {
        current_properties_container->load_values();
    }
}

void element_editor::preivew_process_finished() {
    action_preview->setEnabled(true);
    if (scenegraph::get_current_scene() != nullptr) {
        action_preview_current_scene->setEnabled(true);
    }
    preview_process->deleteLater();
    preview_process = nullptr;
}

void element_editor::file_save() {
    scenegraph::save_scene();
}

void element_editor::file_reload() {
    uuid id = scenegraph::get_current_scene()->get_id();
    scenegraph::open_scene(id);
}

void element_editor::file_close() {
    scenegraph::close_scene();
}

void element_editor::file_preview() {
    action_preview->setEnabled(false);
    action_preview_current_scene->setEnabled(false);
    preview_process = new QProcess();
    connect(preview_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(preivew_process_finished()), Qt::ConnectionType::DirectConnection);
    editor::launch_preview(preview_process, false);
}

void element_editor::file_preview_current_scene() {
    action_preview->setEnabled(false);
    action_preview_current_scene->setEnabled(false);
    preview_process = new QProcess();
    connect(preview_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(preivew_process_finished()), Qt::ConnectionType::DirectConnection);
    editor::launch_preview(preview_process, true);
}

void element_editor::file_properties() {
    project_properties dialog(this);
    dialog.exec();
}

void element_editor::file_reload_plugins() {
    plugins::load_plugins();
}

void element_editor::file_exit() {
    close();
}

void element_editor::tools_import_3d_model() {
    import_3d_model dialog(this);
    dialog.exec();
}

void element_editor::tools_reimport_all_assets() {
    asset_importer::reimport();
}

void element_editor::help_about() {
    //TODO: about
}

void element_editor::help_about_qt() {
    //TODO: about qt
}

void element_editor::context_scene_tree(const QPoint& pos) {
    QModelIndex index = scene_tree->indexAt(pos);
    const scenegraph::node_ref& ref = scene_tree_model->ref_from_index(index);
    if (ref == nullptr || ref == scenegraph::get_current_scene()->get_root_node()) return;
    QMenu menu(this);
    QAction* action_rename = new QAction(QCoreApplication::translate("element-editor", "Rename"), &menu);
    QAction* action_delete = new QAction(QCoreApplication::translate("element-editor", "Delete"), &menu);
    ui::menu_new_node* menu_new_node = new ui::menu_new_node(ref, &menu);
    menu_new_node->setTitle(QCoreApplication::translate("element-editor", "New node..."));
    menu.addAction(action_rename);
    menu.addAction(action_delete);
    menu.addSeparator();
    menu.addMenu(menu_new_node);
    connect(action_rename, SIGNAL(triggered(bool)), this, SLOT(context_node_rename()), Qt::ConnectionType::DirectConnection);
    connect(action_delete, SIGNAL(triggered(bool)), this, SLOT(context_node_delete()), Qt::ConnectionType::DirectConnection);
    node_select(index);
    menu.exec(scene_tree->mapToGlobal(pos));
}

void element_editor::context_assets_tree(const QPoint& pos) {
    QModelIndex index = assets_tree->indexAt(pos);
    std::string path = assets_tree_model->asset_from_index(index);
    if (path.empty()) return;
    QMenu menu(this);
    QAction* action_rename = new QAction(QCoreApplication::translate("element-editor", "Rename"), &menu);
    QAction* action_reimport = new QAction(QCoreApplication::translate("element-editor", "Reimport"), &menu);
    QAction* action_delete = new QAction(QCoreApplication::translate("element-editor", "Delete"), &menu);
    ui::menu_new_asset* menu_new_asset = new ui::menu_new_asset(path, &menu);
    menu_new_asset->setTitle(QCoreApplication::translate("element-editor", "New asset..."));
    menu.addAction(action_rename);
    menu.addAction(action_reimport);
    menu.addAction(action_delete);
    menu.addSeparator();
    menu.addMenu(menu_new_asset);
    connect(action_rename, SIGNAL(triggered(bool)), this, SLOT(context_asset_rename()), Qt::ConnectionType::DirectConnection);
    connect(action_reimport, SIGNAL(triggered(bool)), this, SLOT(context_asset_reimport()), Qt::ConnectionType::DirectConnection);
    connect(action_delete, SIGNAL(triggered(bool)), this, SLOT(context_asset_delete()), Qt::ConnectionType::DirectConnection);
    asset_select(index);
    if (std::filesystem::is_directory(project::project_assets_path / path)) {
        action_reimport->setEnabled(false);
    } else {
        menu_new_asset->setEnabled(false);
    }
    menu.exec(assets_tree->mapToGlobal(pos));
}

void element_editor::context_node_rename() {
    QModelIndex index = scene_tree->selectionModel()->currentIndex();
    if (!index.isValid()) return;
    scene_tree->edit(index);
}

void element_editor::context_node_delete() {
    const scenegraph::node_ref& ref = scene_tree_model->ref_from_index(scene_tree->selectionModel()->currentIndex());
    if (ref == nullptr) return;
    ref->destroy();
}

void element_editor::context_asset_rename() {
    QModelIndex index = assets_tree->selectionModel()->currentIndex();
    if (!index.isValid()) return;
    assets_tree->edit(index);
}

void element_editor::context_asset_reimport() {
    QModelIndex index = assets_tree->selectionModel()->currentIndex();
    if (!index.isValid()) return;
    const uuid& id = assets_tree_model->id_from_index(index);
    asset_importer::import(id);
}

void element_editor::context_asset_delete() {
    QModelIndex index = assets_tree->selectionModel()->currentIndex();
    if (!index.isValid()) return;
    std::string path = assets_tree_model->asset_from_index(index);
    std::filesystem::remove_all(project::project_assets_path / path);
}
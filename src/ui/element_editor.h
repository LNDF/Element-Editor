#pragma once

#include <event/event.h>
#include <render/vulkan.h>
#include <ui/models/model_assets_tree.h>
#include <ui/models/model_scenegraph_tree.h>
#include <ui/widgets/properties_container.h>
#include <ui/vulkan_window.h>
#include <QMainWindow>
#include <QProcess>

#include "ui_element_editor.h"

namespace element {
    namespace ui {
        
        class element_editor : public QMainWindow, private Ui::element_editor {
            Q_OBJECT
            private:
                QProcess* preview_process = nullptr;
                qt_vulkan_window* game_window;
                QWidget* game_window_container;
                model_scenegraph_tree* scene_tree_model = nullptr;
                model_assets_tree* assets_tree_model = nullptr;
                properties_container* current_properties_container = nullptr;
                
                event_callback_handle reload_on_import;
                event_callback_handle render_on_node_added;
                event_callback_handle render_on_node_deleted;

                void load_properties_container();
            protected:
                void closeEvent(QCloseEvent* event) override;
            public:
                element_editor();
                virtual ~element_editor();

                void load_scene();
                void unload_scene();
                
                inline qt_vulkan_window* get_game_window() {return game_window;}
                inline QWidget* get_game_window_container() {return game_window_container;}

            private slots:
                void node_select(const QModelIndex& index);
                void asset_select(const QModelIndex& index);
                void properties_load_values();
                
                void preivew_process_finished();

                void file_save();
                void file_reload();
                void file_close();
                void file_preview();
                void file_preview_current_scene();
                void file_properties();
                void file_exit();
                void tools_import_3d_model();
                void help_about();
                void help_about_qt();

                void context_scene_tree(const QPoint& pos);
                void context_assets_tree(const QPoint& pos);

                void context_node_rename();
                void context_node_delete();

                void context_asset_rename();
                void context_asset_reimport();
                void context_asset_delete();

        };

    } // namespace ui
} // namespace element

#pragma once

#include <event/event.h>
#include <render/vulkan.h>
#include <ui/models/model_assets_tree.h>
#include <ui/models/model_scenegraph_tree.h>
#include <ui/widgets/properties_container.h>
#include <QMainWindow>

#include "ui_element_editor.h"

namespace element {
    namespace ui {
        
        class element_editor : public QMainWindow, private Ui::element_editor {
            Q_OBJECT
            private:
                QWindow* game_window;
                QWidget* game_window_container;
                model_scenegraph_tree* scene_tree_model = nullptr;
                model_assets_tree* assets_tree_model = nullptr;
                properties_container* current_properties_container = nullptr;
                
                event_callback_handle reload_on_import;

                void load_properties_container();
            protected:
                void closeEvent(QCloseEvent* event) override;
            public:
                element_editor();
                virtual ~element_editor();

                void load_scene();
                void unload_scene();
                
                inline QWindow* get_game_window() {return game_window;}
                inline QWidget* get_game_window_container() {return game_window_container;}

            private slots:
                void node_select(const QModelIndex& index);
                void asset_select(const QModelIndex& index);
                void properties_load_values();

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

        };

    } // namespace ui
} // namespace element

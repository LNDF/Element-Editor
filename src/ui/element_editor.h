#pragma once

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

                void load_properties_container();
            protected:
                void closeEvent(QCloseEvent* event) override;
            public:
                element_editor();
                virtual ~element_editor();

                void load_scene();

                inline QWindow* get_game_window() {return game_window;}
                inline QWidget* get_game_window_container() {return game_window_container;}
            private slots:
                void node_selected(const QModelIndex& index);
        };

    } // namespace ui
} // namespace element

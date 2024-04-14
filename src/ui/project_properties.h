#pragma once

#include <ui/widgets/asset_input.h>
#include <QDialog>
#include <QLineEdit>
#include <QStringListModel>
#include <vector>

#include "ui_project_properties.h"

namespace element {
    namespace ui {
        
        class project_properties : public QDialog, private Ui::project_properties {
            Q_OBJECT
            private:
                std::vector<QLineEdit*> required_inputs;
                asset_input* startup_scene_input;
                std::vector<QStringListModel*> plugin_list_models;
                bool new_project;
                bool plugins_modified = false;
                bool plugin_modifications_applied = false;
            public:
                project_properties(QWidget* parent = nullptr);
            private slots:
                void handle_click(QAbstractButton* button);
                void apply_properties();
                void reload_plugins();
                void disable_apply();
                void enable_apply();
                void handle_apply_activation();
                void handle_plugin_platform_change(int index);
                void handle_plugin_select(const QModelIndex& index);
                void disable_plugin_modify_buttons();
                void enable_plugin_modify_buttons();
                void add_plugin();
                void remove_plugin();
                void move_plugin_up();
                void move_plugin_down();
        };

    } // namespace ui    
} // namespace element
#pragma once

#include <QDialog>
#include <QLineEdit>
#include <vector>

#include "ui_project_properties.h"

namespace element {
    namespace ui {
        
        class project_properties : public QDialog, private Ui::project_properties {
            Q_OBJECT
            private:
                std::vector<QLineEdit*> required_inputs;
                bool new_project;
            public:
                project_properties(QWidget* parent = nullptr);
            private slots:
                void handle_click(QAbstractButton* button);
                void apply_properties();
                void disable_apply();
                void enable_apply();
                void handle_inputs_empty();
        };

    } // namespace ui    
} // namespace element
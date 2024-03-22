#pragma once

#include <QDialog>

#include "ui_import_3d_model.h"

namespace element {
    namespace ui {
        
        class import_3d_model : public QDialog, private Ui::import_3d_model {
            Q_OBJECT
            public:
                import_3d_model(QWidget* parent = nullptr);
            private slots:
                void browse_model();
                void browse_destination();
                void import();
        };

    } // namespace ui    
} // namespace element
#pragma once

#include <ui/models/model_asset_chooser.h>
#include <QDialog>

#include "ui_asset_chooser.h"

namespace element {
    namespace ui {
        
        class asset_chooser : public QDialog, private Ui::asset_chooser {
            Q_OBJECT
            private:
                model_asset_chooser* model;
            public:
                asset_chooser(const std::string& type, QWidget* parent = nullptr);
                virtual ~asset_chooser();
            signals:
                void reset();
        };

    } // namespace ui    
} // namespace element
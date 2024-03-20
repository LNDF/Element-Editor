#pragma once

#include <ui/models/model_asset_chooser.h>
#include <QSortFilterProxyModel>
#include <QDialog>

#include "ui_asset_chooser.h"

namespace element {
    namespace ui {
        
        class asset_chooser : public QDialog, private Ui::asset_chooser {
            Q_OBJECT
            private:
                model_asset_chooser* model;
                QSortFilterProxyModel* proxy;
            public:
                asset_chooser(const std::string& type, QWidget* parent = nullptr);
                virtual ~asset_chooser();
            private slots:
                void handle_click(QAbstractButton* button);
                void selected(const QModelIndex& index);
            signals:
                void select(uuid id);
        };

    } // namespace ui    
} // namespace element
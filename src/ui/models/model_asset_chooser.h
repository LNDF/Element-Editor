#pragma once

#include <core/fs.h>
#include <QAbstractListModel>
#include <string>
#include <vector>
#include <utility>

namespace element {
    namespace ui {
        
        class model_asset_chooser : public QAbstractListModel {
            Q_OBJECT
            private:
                std::vector<std::pair<uuid, const fs_resource_info*>> assets;
            public:
                model_asset_chooser(const std::string& type);

                virtual int rowCount(const QModelIndex& parent) const;
                virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
                virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
                virtual QModelIndex parent(const QModelIndex &index) const;

                const uuid& id_from_index(const QModelIndex& index) const;
        };

    } // namespace ui    
} // namespace element
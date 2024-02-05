#pragma once

#include <scenegraph/node_ref.h>
#include <scenegraph/scene.h>
#include <utils/uuid.h>
#include <QAbstractItemModel>
#include <unordered_map>
#include <typeindex>

namespace element {
    namespace ui {
        
        class model_scenegraph_tree : public QAbstractItemModel {
            Q_OBJECT
            private:
                mutable std::unordered_map<uuid, scenegraph::node_ref> node_refs;
                scenegraph::scene* current_scene;
                std::type_index insertion_type;

                scenegraph::node_ref* get_node_ref(const uuid& id) const;
                void remove_node_ref(const scenegraph::node_ref& ref);
                QModelIndex index_from_ref(const scenegraph::node_ref& ref) const;
            public:
                model_scenegraph_tree();

                const scenegraph::node_ref& ref_from_index(const QModelIndex& index) const;
                virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
                virtual QModelIndex parent(const QModelIndex &index) const;
                virtual int rowCount(const QModelIndex &parent) const;
                virtual int columnCount(const QModelIndex &parent) const;
                virtual QVariant data(const QModelIndex &index, int role) const;
                virtual Qt::ItemFlags flags(const QModelIndex &index) const;
                virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
                virtual bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild);
                virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
                virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
                virtual Qt::DropActions supportedDragActions() const;
                virtual Qt::DropActions supportedDropActions() const;
                virtual QMimeData* mimeData(const QModelIndexList &indexes) const;
                virtual QStringList mimeTypes() const;
                virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
        };

    } // namespace ui
} // namespace element

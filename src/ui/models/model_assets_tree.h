#pragma once

#include <asset/asset_events.h>
#include <event/event.h>
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
#include <unordered_map>
#include <filesystem>
#include <string>
#include <vector>

namespace element {
    namespace ui {
        class model_assets_tree;
    } // namespace ui

    namespace __detail {
        struct __ui_model_assets_entry {
            std::string filename;
            bool is_dir;
            std::vector<__ui_model_assets_entry*> children;
            __ui_model_assets_entry* parent;
        };
        
        class __ui_model_assets_tree_unsorted : public QAbstractItemModel {
            Q_OBJECT
            private:
                friend class ui::model_assets_tree;
            
                using entry_type = __ui_model_assets_entry;

                event_callback_handle file_create_handle;
                event_callback_handle file_move_handle;
                event_callback_handle file_delete_handle;

                entry_type* root_entry;

                void create_entries(const std::filesystem::path& path, entry_type* start);
                void remove_entry(entry_type* entry);
                entry_type* entry_from_index(const QModelIndex& index) const;
                entry_type* entry_from_path(const std::filesystem::path& path) const;
                QModelIndex index_from_entry(const entry_type* entry) const;
                std::string asset_from_entry(const entry_type* entry) const;

                bool asset_created(events::asset_file_created& event);
                bool asset_deleted(events::asset_file_deleted& event);
                bool asset_moved(events::asset_file_moved& event);
                
                __ui_model_assets_tree_unsorted();
            public:
                virtual ~__ui_model_assets_tree_unsorted();

                virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
                virtual QModelIndex parent(const QModelIndex &index) const;
                virtual int rowCount(const QModelIndex &parent) const;
                virtual bool hasChildren(const QModelIndex &parent = QModelIndex()) const;
                virtual int columnCount(const QModelIndex &parent) const;
                virtual QVariant data(const QModelIndex &index, int role) const;
                virtual Qt::ItemFlags flags(const QModelIndex &index) const;
                virtual bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild);
                virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
                virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
                virtual Qt::DropActions supportedDragActions() const;
                virtual Qt::DropActions supportedDropActions() const;
                virtual QMimeData* mimeData(const QModelIndexList &indexes) const;
                virtual QStringList mimeTypes() const;
                virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
        };
    } // namespace __detail
    
    namespace ui {
        class model_assets_tree : public QSortFilterProxyModel {
            Q_OBJECT
            private:
                using entry_type = __detail::__ui_model_assets_entry;

                __detail::__ui_model_assets_tree_unsorted* source_model;
            protected:
                virtual bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const;
            public:
                model_assets_tree();
                ~model_assets_tree();

                const uuid& id_from_index(const QModelIndex& index) const;

                static const QString& get_ref_mime_type();
                static const QString& get_path_mime_type();

        };
    } // namespace ui    
} // namespace element 
#include "model_assets_tree.h"

#include <asset/asset_events.h>
#include <asset/importer.h>
#include <core/fs.h>
#include <editor/project.h>
#include <QMimeData>
#include <algorithm>
#include <functional>

using namespace element::__detail;
using namespace element::ui;

static QString mime_type("application/x-element-asset-ref");

void __ui_model_assets_tree_unsorted::create_entries(const std::filesystem::path& path, entry_type* start) {
    for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(path)) {
        std::string path = asset_importer::get_fs_path_from_system(entry.path());
        entry_type& new_entry = entries.try_emplace(path).first->second;
        new_entry.parent = start;
        new_entry.path = std::move(path);
        new_entry.filename = entry.path().filename().generic_string();
        if (entry.is_directory()) {
            new_entry.is_dir = true;
            create_entries(entry.path(), &new_entry);
        } else {
            new_entry.is_dir = false;
        }
        start->children.push_back(&new_entry);
    }
}

void __ui_model_assets_tree_unsorted::remove_entry(entry_type* entry) {
    for (entry_type* child : entry->children) {
        remove_entry(child);
    }
    entries.erase(entry->path);
}

__ui_model_assets_tree_unsorted::entry_type* __ui_model_assets_tree_unsorted::entry_from_index(const QModelIndex& index) const {
    if (!index.isValid()) {
        return root_entry;
    }
    return static_cast<entry_type*>(index.internalPointer());
}

QModelIndex __ui_model_assets_tree_unsorted::index_from_entry(const entry_type* entry) const {
    if (entry == nullptr) return QModelIndex();
    entry_type* parent = entry->parent;
    if (parent == nullptr) return QModelIndex();
    QModelIndex parent_index = index_from_entry(parent);
    int row = 0;
    const auto& children = parent->children;
    while (children[row] != entry) ++row;
    return index(row, 0, parent_index);
}

bool __ui_model_assets_tree_unsorted::asset_created(events::asset_file_created& event) {
    std::filesystem::path parent = event.path;
    parent.remove_filename();
    std::string asset_parent = asset_importer::get_fs_path_from_system(parent);
    std::string asset = asset_importer::get_fs_path_from_system(event.path);
    auto it = entries.find(asset_parent);
    if (it == entries.end()) return true;
    entry_type* entry = &it->second;
    entry_type* new_entry = &entries.try_emplace(asset).first->second;
    new_entry->parent = entry;
    new_entry->path = std::move(asset);
    new_entry->filename = event.path.filename().generic_string();
    if (event.is_dir) {
        new_entry->is_dir = true;
        create_entries(event.path, new_entry);
    } else {
        new_entry->is_dir = false;
    }
    beginInsertRows(index_from_entry(entry), entry->children.size(), entry->children.size());
    entry->children.push_back(new_entry);
    endInsertRows();
    return true;
}

bool __ui_model_assets_tree_unsorted::asset_deleted(events::asset_file_deleted& event) {
    std::string asset = asset_importer::get_fs_path_from_system(event.path);
    auto it = entries.find(asset);
    if (it == entries.end()) return false;
    entry_type* entry = &it->second;
    entry_type* parent = entry->parent;
    if (parent == nullptr) return false;
    int child = 0;
    while (parent->children[child] != entry) ++child;
    const QModelIndex& index = index_from_entry(parent);
    beginRemoveRows(index, child, child);
    parent->children.erase(parent->children.begin() + child);
    endRemoveRows();
    remove_entry(entry);
    return true;
}

bool __ui_model_assets_tree_unsorted::asset_moved(events::asset_file_moved& event) {
    std::filesystem::path from_parent = event.from;
    std::filesystem::path to_parent = event.to;
    from_parent.remove_filename();
    to_parent.remove_filename();
    std::string asset_from_parent = asset_importer::get_fs_path_from_system(from_parent);
    std::string asset_to_parent = asset_importer::get_fs_path_from_system(to_parent);
    std::string asset_from = asset_importer::get_fs_path_from_system(event.from);
    std::string asset_to = asset_importer::get_fs_path_from_system(event.to);
    auto it = entries.find(asset_from_parent);
    if (it == entries.end()) return false;
    entry_type* from_entry = &it->second;
    it = entries.find(asset_to_parent);
    if (it == entries.end()) return false;
    entry_type* to_entry = &it->second;
    it = entries.find(asset_from);
    if (it == entries.end()) return false;
    entry_type* orig_entry = &it->second;
    entry_type* new_entry = &entries.try_emplace(asset_to).first->second;
    new_entry->parent = to_entry;
    new_entry->path = std::move(asset_to);
    new_entry->filename = event.to.filename().generic_string();
    new_entry->is_dir = orig_entry->is_dir;
    int child = 0;
    while (from_entry->children[child] != orig_entry) ++child;
    if (from_entry == to_entry) {
        QList roles = QList<int>({Qt::DisplayRole, Qt::EditRole});
        const QModelIndex& index = index_from_entry(orig_entry);
        from_entry->children[child] = new_entry;
        new_entry->children = std::move(orig_entry->children);
        for (entry_type* c : new_entry->children) c->parent = new_entry;
        emit dataChanged(index, index, roles);
    } else {
        const QModelIndex& from_index = index_from_entry(from_entry);
        const QModelIndex& to_index = index_from_entry(to_entry);
        beginMoveRows(from_index, child, child, to_index, to_entry->children.size());
        new_entry->children = std::move(orig_entry->children);
        for (entry_type* c : new_entry->children) c->parent = new_entry;
        from_entry->children.erase(from_entry->children.begin() + child);
        to_entry->children.push_back(new_entry);
        endMoveRows();
    }
    entries.erase(it);
    return true;
}

__ui_model_assets_tree_unsorted::__ui_model_assets_tree_unsorted() {
    std::string root_path = asset_importer::get_fs_path_from_system(project::project_assets_path);
    root_entry = &entries.try_emplace(root_path).first->second;
    root_entry->is_dir = true;
    root_entry->parent = nullptr;
    root_entry->path = root_path;
    root_entry->filename = std::move(root_path);
    create_entries(project::project_assets_path, root_entry);
    auto create_fun = [this](events::asset_file_created& event) {
        return this->asset_created(event);
    };
    auto move_fun = [this](events::asset_file_moved& event) {
        return this->asset_moved(event);
    };
    auto delete_fun = [this](events::asset_file_deleted& event) {
        return this->asset_deleted(event);
    };
    file_create_handle = event_manager::register_event_callback<events::asset_file_created>(create_fun, event_callback_priority::highest);
    file_move_handle = event_manager::register_event_callback<events::asset_file_moved>(move_fun, event_callback_priority::highest);
    file_delete_handle = event_manager::register_event_callback<events::asset_file_deleted>(delete_fun, event_callback_priority::highest);
}

__ui_model_assets_tree_unsorted::~__ui_model_assets_tree_unsorted() {
    event_manager::unregister_event_callback<events::asset_file_created>(file_create_handle, event_callback_priority::highest);
    event_manager::unregister_event_callback<events::asset_file_moved>(file_move_handle, event_callback_priority::highest);
    event_manager::unregister_event_callback<events::asset_file_deleted>(file_delete_handle, event_callback_priority::highest);
}

QModelIndex __ui_model_assets_tree_unsorted::index(int row, int column, const QModelIndex &parent) const {
    if (column != 0 || !hasIndex(row, column, parent)) return QModelIndex();
    entry_type* entry = entry_from_index(parent);
    if (entry == nullptr) return QModelIndex();
    return createIndex(row, 0, entry->children[row]);
}

QModelIndex __ui_model_assets_tree_unsorted::parent(const QModelIndex &index) const {
    entry_type* entry = entry_from_index(index);
    if (entry == nullptr) return QModelIndex();
    entry_type* parent = entry->parent;
    if (parent == nullptr) return QModelIndex();
    entry_type* pparent = parent->parent;
    if (pparent == nullptr) return QModelIndex();
    int row = 0;
    while (pparent->children[row] != parent) ++row;
    return createIndex(row, 0, parent);
}

int __ui_model_assets_tree_unsorted::rowCount(const QModelIndex &parent) const {
    if (parent.column() > 0) return 0;
    entry_type* entry = entry_from_index(parent);
    if (entry == nullptr) return 0;
    return entry->children.size();
}

bool __ui_model_assets_tree_unsorted::hasChildren(const QModelIndex &parent) const {
    if (parent.column() > 0) return true;
    entry_type* entry = entry_from_index(parent);
    if (entry == nullptr) return true;
    return entry->is_dir;
}

int __ui_model_assets_tree_unsorted::columnCount(const QModelIndex &parent) const {
    return 1;
}

QVariant __ui_model_assets_tree_unsorted::data(const QModelIndex &index, int role) const {
    if (role != Qt::DisplayRole && role != Qt::EditRole) return QVariant();
    entry_type* entry = entry_from_index(index);
    if (entry == nullptr) return QVariant();
    return QString(entry->filename.c_str());
}

Qt::ItemFlags __ui_model_assets_tree_unsorted::flags(const QModelIndex &index) const {
    entry_type* entry = entry_from_index(index);
    if (entry == nullptr) return Qt::NoItemFlags;
    if (entry == root_entry) return Qt::ItemIsDropEnabled;
    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEnabled;
}

bool __ui_model_assets_tree_unsorted::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild) {
    entry_type* entry_src = entry_from_index(sourceParent);
    entry_type* entry_dst = entry_from_index(destinationParent);
    if (entry_src == nullptr || entry_dst == nullptr) return false;
    if (entry_src != entry_dst && entry_dst->is_dir) {
        for (int i = 0; i < count; ++i) {
            entry_type* item = entry_src->children[sourceRow + i];
            std::filesystem::rename(project::project_assets_path / item->path, project::project_assets_path / entry_dst->path / item->filename);
        }
        return true;
    }
    return false;
}

bool __ui_model_assets_tree_unsorted::removeRows(int row, int count, const QModelIndex &parent) {
    entry_type* entry = entry_from_index(parent);
    if (entry == nullptr) return false;
    for (int i = 0; i < count; ++i) {
        entry_type* item = entry->children[row + count];
        std::filesystem::remove_all(project::project_assets_path / item->path);
    }
    return true;
}

bool __ui_model_assets_tree_unsorted::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole)) return false;
    entry_type* entry = entry_from_index(index);
    if (entry == nullptr) return false;
    std::filesystem::rename(project::project_assets_path / entry->path, project::project_assets_path / entry->parent->path / value.toString().toStdString());
    return true;
}

Qt::DropActions __ui_model_assets_tree_unsorted::supportedDragActions() const {
    return Qt::CopyAction;
}

Qt::DropActions __ui_model_assets_tree_unsorted::supportedDropActions() const {
    return Qt::CopyAction;
}

QMimeData* __ui_model_assets_tree_unsorted::mimeData(const QModelIndexList &indexes) const {
    QMimeData* data = new QMimeData();
    QByteArray encoded;
    QDataStream stream(&encoded, QDataStream::WriteOnly);
    for (const auto& index : indexes) {
        entry_type* entry = entry_from_index(index);
        if (entry == nullptr) continue;
        const uuid& id = fs::get_uuid_from_resource_path(entry->path);
        stream.writeRawData((const char*) id.bytes, 16);
    }
    data->setData(QString(mime_type), encoded);
    return data;
}

QStringList __ui_model_assets_tree_unsorted::mimeTypes() const {
    return QStringList({QString(mime_type)});
}

bool __ui_model_assets_tree_unsorted::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) {
    if (data->hasFormat(mime_type)) {
        bool ret = false;
        QByteArray encoded = data->data(mime_type);
        QDataStream stream(&encoded, QDataStream::ReadOnly);
        uuid id = uuid::null();
        int drow = row;
        while (!stream.atEnd()) {
            stream.readRawData((char*) id.bytes, 16);
            const fs_resource_info& info = fs::get_resource_info(id);
            auto it = entries.find(info.path);
            if (it == entries.end()) continue;
            entry_type* entry = &it->second;
            if (entry == nullptr || entry == entry_from_index(parent)) continue;
            entry_type* parent_entry = entry->parent;
            if (parent_entry == nullptr) continue;
            int srow = 0;
            const auto& children = parent_entry->children;
            while (children[srow] != entry) ++srow;
            QModelIndex src_parent = index_from_entry(parent_entry);
            moveRow(src_parent, srow, parent, drow++);
            ret = true;
        }
        return ret;
    }
    return false;
}

model_assets_tree::model_assets_tree() {
    source_model = new __detail::__ui_model_assets_tree_unsorted();
    setSourceModel(source_model);
}

model_assets_tree::~model_assets_tree() {
    delete source_model;
}

bool model_assets_tree::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const {
    entry_type* left = source_model->entry_from_index(source_left);
    entry_type* right = source_model->entry_from_index(source_right);
    if (left->is_dir != right->is_dir) {
        return right->is_dir;
    }
    return !QSortFilterProxyModel::lessThan(source_left, source_right);
}
#include "model_assets_tree.h"

#include <asset/asset_events.h>
#include <core/fs.h>
#include <editor/project.h>
#include <QMimeData>
#include <algorithm>
#include <functional>

using namespace element::__detail;
using namespace element::ui;

static QString ref_mime_type("application/x-element-asset-ref");
static QString path_mime_type("application/x-element-asset-path");

void __ui_model_assets_tree_unsorted::create_entries(const std::filesystem::path& path, entry_type* start) {
    for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(path)) {
        entry_type* new_entry = new entry_type();
        new_entry->parent = start;
        new_entry->filename = entry.path().filename().generic_string();
        if (entry.is_directory()) {
            new_entry->is_dir = true;
            create_entries(entry.path(), new_entry);
        } else {
            new_entry->is_dir = false;
        }
        start->children.push_back(new_entry);
    }
}

void __ui_model_assets_tree_unsorted::remove_entry(entry_type* entry) {
    for (entry_type* child : entry->children) {
        remove_entry(child);
    }
    delete entry;
}

__ui_model_assets_tree_unsorted::entry_type* __ui_model_assets_tree_unsorted::entry_from_index(const QModelIndex& index) const {
    if (!index.isValid()) {
        return root_entry;
    }
    return static_cast<entry_type*>(index.internalPointer());
}

__ui_model_assets_tree_unsorted::entry_type* __ui_model_assets_tree_unsorted::entry_from_path(const std::filesystem::path& path) const {
    std::filesystem::path asset_relative = std::filesystem::relative(path, project::project_assets_path);
    entry_type* ret = root_entry;
    for (const auto& part : asset_relative) {
        std::string filename = part.filename().generic_string();
        if (filename.empty() || filename == ".") continue;
        for (entry_type* child : ret->children) {
            if (child->filename == filename) {
                ret = child;
                goto next;
            }
        }
        return nullptr;
        next:
        continue;
    }
    return ret;
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

std::string __ui_model_assets_tree_unsorted::asset_from_entry(const entry_type* entry) const {
    std::string s = entry->filename;
    entry = entry->parent;
    while (entry != nullptr && entry->parent != nullptr) {
        s = entry->filename + "/" + s;
        entry = entry->parent;
    }
    return s;
}

bool __ui_model_assets_tree_unsorted::asset_created(events::asset_file_created& event) {
    std::filesystem::path parent = event.path;
    parent.remove_filename();
    entry_type* entry = entry_from_path(parent);
    if (entry == nullptr) return false;
    entry_type* new_entry = new entry_type();
    new_entry->parent = entry;
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
    entry_type* entry = entry_from_path(event.path);
    if (entry == nullptr) return false;
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
    std::filesystem::path to_parent = event.to;
    to_parent.remove_filename();
    entry_type* orig_entry = entry_from_path(event.from);
    if (orig_entry == nullptr) return false;
    entry_type* from_entry = orig_entry->parent;
    if (from_entry == nullptr) return false;
    entry_type* to_entry = entry_from_path(to_parent);
    entry_type* new_entry = new entry_type();
    new_entry->parent = to_entry;
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
    delete orig_entry;
    return true;
}

__ui_model_assets_tree_unsorted::__ui_model_assets_tree_unsorted() {
    root_entry = new entry_type();
    root_entry->is_dir = true;
    root_entry->parent = nullptr;
    root_entry->filename = "";
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
    std::string dst_asset = asset_from_entry(entry_dst);
    if (entry_src != entry_dst && entry_dst->is_dir) {
        for (int i = 0; i < count; ++i) {
            entry_type* item = entry_src->children[sourceRow + i];
            std::string path = asset_from_entry(item);
            std::filesystem::rename(project::project_assets_path / path, project::project_assets_path / dst_asset / item->filename);
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
        std::string path = asset_from_entry(item);
        std::filesystem::remove_all(project::project_assets_path / path);
    }
    return true;
}

bool __ui_model_assets_tree_unsorted::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole)) return false;
    entry_type* entry = entry_from_index(index);
    if (entry == nullptr) return false;
    std::string path_parent = asset_from_entry(entry->parent);
    std::filesystem::rename(project::project_assets_path / path_parent / entry->filename, project::project_assets_path / path_parent / value.toString().toStdString());
    return true;
}

Qt::DropActions __ui_model_assets_tree_unsorted::supportedDragActions() const {
    return Qt::CopyAction;
}

Qt::DropActions __ui_model_assets_tree_unsorted::supportedDropActions() const {
    return Qt::CopyAction;
}

QMimeData* __ui_model_assets_tree_unsorted::mimeData(const QModelIndexList &indexes) const {
    if (indexes.size() != 1) return nullptr;
    const QModelIndex& index = indexes.at(0);
    entry_type* entry = entry_from_index(index);
    if (entry == nullptr) return nullptr;
    std::string asset = asset_from_entry(entry);
    QMimeData* data = new QMimeData();
    QByteArray encoded_path;
    QDataStream stream_path(&encoded_path, QDataStream::WriteOnly);
    stream_path.writeBytes(asset.c_str(), asset.size() + 1);
    data->setData(path_mime_type, encoded_path);
    const uuid& id = fs::get_uuid_from_resource_path(asset);
    if (!id.is_null()) {
        QByteArray encoded_ref;
        QDataStream stream_ref(&encoded_ref, QDataStream::WriteOnly);
        stream_ref.writeRawData((const char*) id.bytes, 16);
        data->setData(ref_mime_type, encoded_ref);
    }
    return data;
}

QStringList __ui_model_assets_tree_unsorted::mimeTypes() const {
    return QStringList({ref_mime_type, path_mime_type});
}

bool __ui_model_assets_tree_unsorted::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) {
    if (data->hasFormat(path_mime_type)) {
        QByteArray encoded = data->data(path_mime_type);
        QDataStream stream(&encoded, QDataStream::ReadOnly);
        char* path;
        std::uint32_t length;
        stream.readBytes(path, length);
        if (length == 0) return false;
        entry_type* entry = entry_from_path(project::project_assets_path / path);
        delete[] path;
        if (entry == nullptr || entry == entry_from_index(parent)) return false;
        entry_type* parent_entry = entry->parent;
        if (parent_entry == nullptr) return false;
        int srow = 0;
        const auto& children = parent_entry->children;
        while (children[srow] != entry) ++srow;
        QModelIndex src_parent = index_from_entry(parent_entry);
        return moveRow(src_parent, srow, parent, row);
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

const element::uuid& model_assets_tree::id_from_index(const QModelIndex& index) const {
    if (!index.isValid()) return uuid::null();
    QModelIndex source = mapToSource(index);
    if (!source.isValid()) return uuid::null();
    const entry_type* entry = (const entry_type*) source.internalPointer();
    if (entry == nullptr) return uuid::null();
    std::string path = source_model->asset_from_entry(entry);
    return fs::get_uuid_from_resource_path(path);
}

std::string model_assets_tree::asset_from_index(const QModelIndex& index) const {
    if (!index.isValid()) return "";
    QModelIndex source = mapToSource(index);
    if (!source.isValid()) return "";
    const entry_type* entry = (const entry_type*) source.internalPointer();
    if (entry == nullptr) return "";
    return source_model->asset_from_entry(entry);
}

const QString& model_assets_tree::get_ref_mime_type() {
    return ref_mime_type;
}

const QString& model_assets_tree::get_path_mime_type() {
    return path_mime_type;
}
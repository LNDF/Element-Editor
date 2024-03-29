#include "model_scenegraph_tree.h"

#include <scenegraph/node.h>
#include <scenegraph/node_type_info.h>
#include <scenegraph/editor_scene_loader.h>
#include <QMimeData>

using namespace element::ui;
using namespace element;

static QString mime_type("application/x-element-node-ref");

scenegraph::node_ref* model_scenegraph_tree::get_node_ref(const uuid& id) const {
    auto it = node_refs.find(id);
    if (it == node_refs.end()) {
        it = node_refs.try_emplace(id, id).first;
    }
    return &it->second;
}

void model_scenegraph_tree::remove_node_ref(const scenegraph::node_ref& ref) {
    for (const auto& child : ref->get_children()) {
        remove_node_ref(child);
    }
    node_refs.erase(ref.get_id());
}

const scenegraph::node_ref& model_scenegraph_tree::ref_from_index(const QModelIndex& index) const {
    if (index.isValid()) {
        return *(static_cast<const scenegraph::node_ref*>(index.internalPointer()));
    } else {
        return current_scene->get_root_node();
    }
}

QModelIndex model_scenegraph_tree::index_from_ref(const scenegraph::node_ref& ref) const {
    if (current_scene == nullptr || ref == nullptr) return QModelIndex();
    const scenegraph::node_ref& parent = ref->get_parent();
    if (parent == nullptr) return QModelIndex();
    QModelIndex parent_index = index_from_ref(parent);
    int row = 0;
    const auto& children = parent->get_children();
    while (children[row] != ref) ++row;
    return index(row, 0, parent_index);
}
#include <core/log.h>
bool model_scenegraph_tree::before_node_added(events::before_node_added& event) {
    beginInsertRows(index_from_ref(event.parent), event.index, event.index);
    return true;
}

bool model_scenegraph_tree::after_node_added(events::after_node_added& event) {
    endInsertRows();
    return true;
}

bool model_scenegraph_tree::before_node_moved(events::before_node_moved& event) {
    QModelIndex from_parent_index = index_from_ref(event.from_parent);
    QModelIndex to_parent_index = index_from_ref(event.to_parent);
    int index = 0;
    const auto& from_children = event.from_parent->get_children();
    while (from_children[index] != event.this_node) ++index;
    beginMoveRows(from_parent_index, index, index, to_parent_index, event.dest_index);
    return true;
}

bool model_scenegraph_tree::after_node_moved(events::after_node_moved& event) {
    endMoveRows();
    return true;
}

bool model_scenegraph_tree::before_node_deleted(events::before_node_deleted& event) {
    int index = 0;
    const auto& parent_children = event.parent->get_children();
    while (parent_children[index] != event.this_node) ++index;
    beginRemoveRows(index_from_ref(event.parent), index, index);
    remove_node_ref(event.this_node);
    return true;
}

bool model_scenegraph_tree::after_node_deleted(events::after_node_deleted& event) {
    endRemoveRows();
    return true;
}

model_scenegraph_tree::model_scenegraph_tree() {
    current_scene = scenegraph::get_current_scene();
    auto before_added_fun = [this](events::before_node_added& event) {
        return this->before_node_added(event);
    };
    auto after_added_fun = [this](events::after_node_added& event) {
        return this->after_node_added(event);
    };
    auto before_moved_fun = [this](events::before_node_moved& event) {
        return this->before_node_moved(event);
    };
    auto after_moved_fun = [this](events::after_node_moved& event) {
        return this->after_node_moved(event);
    };
    auto before_deleted_fun = [this](events::before_node_deleted& event) {
        return this->before_node_deleted(event);
    };
    auto after_deleted_fun = [this](events::after_node_deleted& event) {
        return this->after_node_deleted(event);
    };
    before_node_added_handle = event_manager::register_event_callback<events::before_node_added>(before_added_fun, event_callback_priority::highest);
    after_node_added_handle = event_manager::register_event_callback<events::after_node_added>(after_added_fun, event_callback_priority::highest);
    before_node_moved_handle = event_manager::register_event_callback<events::before_node_moved>(before_moved_fun, event_callback_priority::highest);
    after_node_moved_handle = event_manager::register_event_callback<events::after_node_moved>(after_moved_fun, event_callback_priority::highest);
    before_node_deleted_handle = event_manager::register_event_callback<events::before_node_deleted>(before_deleted_fun, event_callback_priority::highest);
    after_node_deleted_handle = event_manager::register_event_callback<events::after_node_deleted>(after_deleted_fun, event_callback_priority::highest);
}

model_scenegraph_tree::~model_scenegraph_tree() {
    event_manager::unregister_event_callback<events::before_node_added>(before_node_added_handle, event_callback_priority::highest);
    event_manager::unregister_event_callback<events::after_node_added>(after_node_added_handle, event_callback_priority::highest);
    event_manager::unregister_event_callback<events::before_node_moved>(before_node_moved_handle, event_callback_priority::highest);
    event_manager::unregister_event_callback<events::after_node_moved>(after_node_moved_handle, event_callback_priority::highest);
    event_manager::unregister_event_callback<events::before_node_deleted>(before_node_deleted_handle, event_callback_priority::highest);
    event_manager::unregister_event_callback<events::after_node_deleted>(after_node_deleted_handle, event_callback_priority::highest);
}

QModelIndex model_scenegraph_tree::index(int row, int column, const QModelIndex &parent) const {
    if (current_scene == nullptr || column != 0 || !hasIndex(row, column, parent)) return QModelIndex();
    const scenegraph::node_ref& ref = ref_from_index(parent);
    if (ref == nullptr) return QModelIndex();
    const uuid& id = ref->get_children()[row].get_id();
    return createIndex(row, 0, get_node_ref(id));
}

QModelIndex model_scenegraph_tree::parent(const QModelIndex &index) const {
    if (current_scene == nullptr) return QModelIndex();
    const scenegraph::node_ref& ref = ref_from_index(index);
    if (ref == nullptr) return QModelIndex();
    const scenegraph::node_ref& parent = ref->get_parent();
    if (parent == nullptr) return QModelIndex();
    const scenegraph::node_ref& pparent = parent->get_parent();
    if (pparent == nullptr) return QModelIndex();
    int row = 0;
    const auto& pparent_children = pparent->get_children();
    while (pparent_children[row] != parent) ++row;
    return createIndex(row, 0, get_node_ref(parent.get_id()));
}

int model_scenegraph_tree::rowCount(const QModelIndex &parent) const {
    if (current_scene == nullptr || parent.column() > 0) return 0;
    const scenegraph::node_ref& ref = ref_from_index(parent);
    if (ref == nullptr) return 0;
    return ref->get_children().size();
}

int model_scenegraph_tree::columnCount(const QModelIndex &parent) const {
    return 1;
}

QVariant model_scenegraph_tree::data(const QModelIndex &index, int role) const {
    if (current_scene == nullptr || (role != Qt::DisplayRole && role != Qt::EditRole)) return QVariant();
    const scenegraph::node_ref& ref = ref_from_index(index);
    if (ref == nullptr) return QVariant();
    return QString(ref->get_name().c_str());
}

Qt::ItemFlags model_scenegraph_tree::flags(const QModelIndex &index) const {
    if (current_scene == nullptr) return Qt::NoItemFlags;
    const scenegraph::node_ref& ref = ref_from_index(index);
    if (ref == nullptr) return Qt::NoItemFlags;
    if (ref == current_scene->get_root_node()) return Qt::ItemIsDropEnabled;
    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEnabled;
}

bool model_scenegraph_tree::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild) {
    if (current_scene == nullptr) return false;
    const scenegraph::node_ref& ref_src = ref_from_index(sourceParent);
    const scenegraph::node_ref& ref_dst = ref_from_index(destinationParent);
    if (ref_src == nullptr || ref_dst == nullptr) return false;
    if (destinationChild == -1) destinationChild = 0;
    if (ref_src == ref_dst) {
        int move_diff = destinationChild - sourceRow;
        if (move_diff == 0 || move_diff == 1) return false;
        if (sourceRow < destinationChild) {
            for (int i = count - 1; i >= 0; --i) {
                ref_src->move_child(sourceRow + i, destinationChild + i);
            }
        } else if (sourceRow > destinationChild) {
            for (int i = 0; i < count; ++i) {
                ref_src->move_child(sourceRow + i, destinationChild + i);
            }
        }
    } else {
        const auto& children = ref_src->get_children();
        for (int i = count - 1; i >= 0; --i) {
            ref_dst->add_child(children[sourceRow + i], destinationChild);
        }
    }
    return true;
}

bool model_scenegraph_tree::removeRows(int row, int count, const QModelIndex &parent) {
    if (current_scene == nullptr) return false;
    const scenegraph::node_ref& ref = ref_from_index(parent);
    if (ref == nullptr) return false;
    const auto& children = ref->get_children();
    for (int i = row + count - 1; i >= row; --i) {
        children[i]->destroy();
    }
    return true;
}

bool model_scenegraph_tree::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (current_scene == nullptr || !index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole)) return false;
    const scenegraph::node_ref& ref = ref_from_index(index);
    if (ref == nullptr) return false;
    ref->set_name(value.toString().toStdString());
    QList roles = QList<int>({Qt::DisplayRole, Qt::EditRole});
    emit dataChanged(index, index, roles);
    return true;
}

Qt::DropActions model_scenegraph_tree::supportedDragActions() const {
    return Qt::CopyAction;
}

Qt::DropActions model_scenegraph_tree::supportedDropActions() const {
    return Qt::CopyAction;
}

QMimeData* model_scenegraph_tree::mimeData(const QModelIndexList &indexes) const {
    QMimeData* data = new QMimeData();
    QByteArray encoded;
    QDataStream stream(&encoded, QDataStream::WriteOnly);
    for (const auto& index : indexes) {
        const scenegraph::node_ref& ref = ref_from_index(index);
        if (ref != nullptr) stream.writeRawData((const char*) ref.get_id().bytes, 16);
    }
    data->setData(QString(mime_type), encoded);
    return data;
}

QStringList model_scenegraph_tree::mimeTypes() const {
    return QStringList({QString(mime_type)});
}

bool model_scenegraph_tree::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) {
    if (data->hasFormat(mime_type)) {
        bool ret = false;
        QByteArray encoded = data->data(mime_type);
        QDataStream stream(&encoded, QDataStream::ReadOnly);
        uuid id = uuid::null();
        int drow = row;
        while (!stream.atEnd()) {
            stream.readRawData((char*) id.bytes, 16);
            scenegraph::node_ref ref = id;
            if (ref == nullptr || ref == ref_from_index(parent)) continue;
            const scenegraph::node_ref& parent_ref = ref->get_parent();
            if (parent_ref == nullptr) continue;
            int srow = 0;
            const auto& children = parent_ref->get_children();
            while (children[srow] != ref) ++srow;
            QModelIndex src_parent = index_from_ref(parent_ref);
            moveRow(src_parent, srow, parent, drow++);
            ret = true;
        }
        return ret;
    }
    return false;
}

const QString& model_scenegraph_tree::get_mime_type() {
    return mime_type;
}
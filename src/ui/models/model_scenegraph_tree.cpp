#include "model_scenegraph_tree.h"

#include <scenegraph/node.h>
#include <scenegraph/node_type_info.h>
#include <scenegraph/editor_scene_loader.h>

using namespace element::ui;
using namespace element;

scenegraph::node_ref* model_scenegraph_tree::get_node_ref(const uuid& id) const {
    auto it = node_refs.find(id);
    if (it == node_refs.end()) {
        it = node_refs.try_emplace(id, id).first;
    }
    return &it->second;
}

model_scenegraph_tree::model_scenegraph_tree() : insertion_type(std::type_index(typeid(scenegraph::node))) {
    current_scene = scenegraph::get_current_scene();
}

QModelIndex model_scenegraph_tree::index(int row, int column, const QModelIndex &parent) const {
    if (current_scene == nullptr || column != 0) {
        return QModelIndex();
    }
    const scenegraph::node_ref* ref = nullptr;
    if (parent.isValid()) {
        ref = static_cast<scenegraph::node_ref*>(parent.internalPointer());
    } else {
        ref = &current_scene->get_root_node();
    }
    const uuid& id = (*ref)->get_children()[row].get_id();
    return createIndex(row, 0, get_node_ref(id));
}

QModelIndex model_scenegraph_tree::parent(const QModelIndex &index) const {
    if (current_scene == nullptr || !index.isValid()) {
        return QModelIndex();
    }
    scenegraph::node_ref* ref = static_cast<scenegraph::node_ref*>(index.internalPointer());
    const scenegraph::node_ref& parent = (*ref)->get_parent();
    if (parent == current_scene->get_root_node()) {
        return QModelIndex();
    }
    int row = 0;
    const auto& parent_children = parent->get_parent()->get_children();
    while (parent_children[row] != parent) row++;
    return createIndex(row, 0, get_node_ref(parent.get_id()));
}

int model_scenegraph_tree::rowCount(const QModelIndex &parent) const {
    if (current_scene == nullptr || parent.column() > 0) {
        return 0;
    }
    const scenegraph::node_ref* ref = nullptr;
    if (parent.isValid()) {
        ref = static_cast<scenegraph::node_ref*>(parent.internalPointer());
    } else {
        ref = &current_scene->get_root_node();
    }
    return (*ref)->get_children().size();
}

int model_scenegraph_tree::columnCount(const QModelIndex &parent) const {
    return 1;
}

QVariant model_scenegraph_tree::data(const QModelIndex &index, int role) const {
    if (current_scene == nullptr || !index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole)) {
        return QVariant();
    }
    const scenegraph::node_ref* ref = static_cast<scenegraph::node_ref*>(index.internalPointer());
    return QString((*ref)->get_name().c_str());
}

Qt::ItemFlags model_scenegraph_tree::flags(const QModelIndex &index) const {
    if (current_scene == nullptr || !index.isValid()) {
        return Qt::NoItemFlags;
    }
    const scenegraph::node_ref* ref = static_cast<scenegraph::node_ref*>(index.internalPointer());
    if (*ref == current_scene->get_root_node()) {
        return Qt::ItemIsDropEnabled;
    }
    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEnabled;
}
#include <core/log.h>
bool model_scenegraph_tree::insertRows(int row, int count, const QModelIndex &parent) {
    ELM_WARN("Insert");
    if (current_scene == nullptr || !parent.isValid()) {
        return false;
    }
    beginInsertRows(parent, row, row + count - 1);
    const std::string& name = scenegraph::get_node_type_info(insertion_type).name;
    scenegraph::node_ref* ref = static_cast<scenegraph::node_ref*>(parent.internalPointer());
    for (int i = 0; i < count; ++i) {
        (*ref)->add_child(insertion_type, name, row + i);
    }
    endInsertRows();
    return true;
}

bool model_scenegraph_tree::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild) {
    ELM_WARN("Move");
    if (current_scene == nullptr || !sourceParent.isValid() || !destinationParent.isValid()) {
        return false;
    }
    beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild);
    const scenegraph::node_ref* ref_src = static_cast<scenegraph::node_ref*>(sourceParent.internalPointer());
    const scenegraph::node_ref* ref_dst = static_cast<scenegraph::node_ref*>(destinationParent.internalPointer());
    if (*ref_src == *ref_dst) {
        if (sourceRow < destinationChild) {
            for (int i = count - 1; i >= 0; --i) {
                (*ref_src)->move_child(sourceRow + i, destinationChild + i);
            }
        } else {
            for (int i = 0; i < count; ++i) {
                (*ref_src)->move_child(sourceRow + i, destinationChild + i);
            }
        }
    } else {
        const auto& children = (*ref_src)->get_children();
        for (int i = count - 1; i >= 0; --i) {
            (*ref_dst)->add_child(children[sourceRow + i], destinationChild);
        }
    }
    endMoveRows();
    return true;
}

bool model_scenegraph_tree::removeRows(int row, int count, const QModelIndex &parent) {
    ELM_WARN("Remove");
    if (current_scene == nullptr || !parent.isValid()) {
        return false;
    }
    beginRemoveRows(parent, row, row + count - 1);
    scenegraph::node_ref* ref = static_cast<scenegraph::node_ref*>(parent.internalPointer());
    const auto& children = (*ref)->get_children();
    for (int i = row + count - 1; i >= row; --i) {
        children[i]->destroy();
    }
    endRemoveRows();
    return true;
}

bool model_scenegraph_tree::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (current_scene == nullptr || !index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole)) {
        return false;
    }
    scenegraph::node_ref* ref = static_cast<scenegraph::node_ref*>(index.internalPointer());
    (*ref)->set_name(value.toString().toStdString());
    QList roles = QList<int>({Qt::DisplayRole, Qt::EditRole});
    emit dataChanged(index, index, roles);
    return true;
}

Qt::DropActions model_scenegraph_tree::supportedDragActions() const {
    return Qt::MoveAction;
}

Qt::DropActions model_scenegraph_tree::supportedDropActions() const {
    return Qt::TargetMoveAction;
}
#include "activable_tree.h"

using namespace element::ui;

activable_tree::activable_tree(QWidget* parent) : QTreeView(parent) {

}

#include <core/log.h>

void activable_tree::mousePressEvent(QMouseEvent* event) {
    do_click_select = true;
    select_index = indexAt(event->pos());
    QTreeView::mousePressEvent(event);
}

void activable_tree::mouseReleaseEvent(QMouseEvent* event) {
    if (do_click_select && select_index.isValid()) {
        emit activate(select_index);
    }
    do_click_select = false;
    QTreeView::mouseReleaseEvent(event);
}

void activable_tree::dragMoveEvent(QDragMoveEvent* event) {
    do_click_select = false;
    QTreeView::dragMoveEvent(event);
}

void activable_tree::setModel(QAbstractItemModel* model) {
    QTreeView::setModel(model);
    connect(selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(on_selection_changed(const QModelIndex&)), Qt::ConnectionType::DirectConnection);
}

void activable_tree::on_selection_changed(const QModelIndex& index) {
    if (!do_click_select) emit activate(index);
}
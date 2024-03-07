#include "activable_tree.h"

using namespace element::ui;

activable_tree::activable_tree(QWidget* parent) : QTreeView(parent) {
    
}

#include <core/log.h>
void activable_tree::mousePressEvent(QMouseEvent *event) {
    QTreeView::mousePressEvent(event);
}

void activable_tree::mouseReleaseEvent(QMouseEvent *event) {
    QTreeView::mouseReleaseEvent(event);
}

void activable_tree::dragMoveEvent(QDragMoveEvent *event) {
    QTreeView::dragMoveEvent(event);
}
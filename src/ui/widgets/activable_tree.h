#pragma once

#include <QApplication>
#include <QTreeView>
#include <QMouseEvent>
#include <QDebug>

namespace element {
    namespace ui {
        
        class activable_tree : public QTreeView {
            Q_OBJECT
            protected:
                virtual void mousePressEvent(QMouseEvent *event);
                virtual void mouseReleaseEvent(QMouseEvent *event);
                virtual void dragMoveEvent(QDragMoveEvent *event);
            public:
                activable_tree(QWidget* parent);
            signals:
                
        };

    } // namespace ui
} // namespace element

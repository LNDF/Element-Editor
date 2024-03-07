#pragma once

#include <QApplication>
#include <QTreeView>
#include <QMouseEvent>
#include <QDebug>

namespace element {
    namespace ui {
        
        class acticable_tree : public QTreeView {
            Q_OBJECT
            protected:
                virtual void mousePressEvent(QMouseEvent *event);
                virtual void dragLeaveEvent(QDragLeaveEvent *event);
            public:
                acticable_tree(QWidget* parent);
            signals:
                
        };

    } // namespace ui
} // namespace element

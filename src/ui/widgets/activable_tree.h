#pragma once

#include <QApplication>
#include <QTreeView>
#include <QMouseEvent>
#include <QDebug>

namespace element {
    namespace ui {
        
        class activable_tree : public QTreeView {
            Q_OBJECT
            private:
                QModelIndex select_index;
                bool do_click_select = false;
            protected:
                virtual void mousePressEvent(QMouseEvent *event);
                virtual void mouseReleaseEvent(QMouseEvent *event);
                virtual void dragMoveEvent(QDragMoveEvent *event);
            public:
                activable_tree(QWidget* parent);

                virtual void setModel(QAbstractItemModel* model);
            private slots:
                void on_selection_changed(const QModelIndex& index);
            signals:
                void activate(const QModelIndex& index);
        };

    } // namespace ui
} // namespace element

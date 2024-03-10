#pragma once

#include <scenegraph/node_ref.h>
#include <QMenu>
#include <QAction>
#include <vector>
#include <typeindex>

namespace element {
    namespace ui {
        
        class menu_new_node_action : public QAction {
            Q_OBJECT
            private:
                std::type_index type;
            public:
                menu_new_node_action(std::type_index type, QObject* parent = nullptr);

                void retranslate();
            private slots:
                void create_node();
            signals:
                void trigger(std::type_index  type);
        };

        class menu_new_node : public QMenu {
            Q_OBJECT
            private:
                scenegraph::node_ref parent_node;
                std::vector<menu_new_node_action*> actions;

                void create_node(std::type_index type, const scenegraph::node_ref& parent);
            public:
                menu_new_node(QWidget* parent = nullptr);
                menu_new_node(const scenegraph::node_ref& parent_node, QWidget* parent = nullptr);

                void retranslate();
            private slots:
                void action_triggered(std::type_index type);
        };

    } // namespace ui
} // namespace element

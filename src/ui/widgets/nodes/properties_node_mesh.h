#pragma once

#include <scenegraph/nodes/mesh_node_ref.h>
#include <ui/widgets/properties_node.h>
#include <ui/widgets/nodes/properties_node_mesh_form.h>

namespace element {
    namespace ui {
        
        class properties_node_mesh : public properties_node {
            Q_OBJECT
            private:
                properties_node_mesh_form* mesh;
            public:
                properties_node_mesh(const scenegraph::node_ref& node, QWidget* parent = nullptr);
            public slots:
                virtual void load_values();
        };

    } // namespace ui
} // namespace element

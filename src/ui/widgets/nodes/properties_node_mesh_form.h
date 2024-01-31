#pragma once

#include <scenegraph/nodes/mesh_node_ref.h>
#include <ui/widgets/properties_standard_form.h>
#include <QWidget>

namespace element {
    namespace ui {
        
        class properties_node_mesh_form : public properties_standard_form {
            Q_OBJECT
            private:
                scenegraph::mesh_node_ref node;
            public:
                properties_node_mesh_form(const scenegraph::mesh_node_ref& node, QWidget* parent = nullptr);
            public slots:
                void load_values();
            
        };

    } // namespace ui
} // namespace element 
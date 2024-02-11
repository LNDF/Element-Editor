#pragma once

#include <scenegraph/nodes/mesh_node_ref.h>
#include <ui/widgets/asset_input.h>
#include <ui/widgets/properties_standard_form.h>
#include <QWidget>

namespace element {
    namespace ui {
        
        class properties_node_mesh_form : public properties_standard_form {
            Q_OBJECT
            private:
                scenegraph::mesh_node_ref node;
                asset_input* material_input;
                asset_input* mesh_input;
            public:
                properties_node_mesh_form(const scenegraph::mesh_node_ref& node, QWidget* parent = nullptr);
            private slots:
                void set_material(uuid material);
                void set_mesh(uuid mesh);
            public slots:
                void load_values();
            
        };

    } // namespace ui
} // namespace element 
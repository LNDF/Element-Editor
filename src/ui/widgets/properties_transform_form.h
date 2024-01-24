#pragma once

#include <scenegraph/node_ref.h>
#include <ui/widgets/vector_input.h>
#include <ui/widgets/properties_standard_form.h>
#include <glm/glm.hpp>
#include <QWidget>

namespace element {
    namespace ui {
        
        class properties_transform_form : public properties_standard_form {
            Q_OBJECT
            private:
                scenegraph::node_ref node;
                vec3_input* position_input;
                vec3_input* rotation_input;
                vec3_input* scale_input;
            public:
                properties_transform_form(const scenegraph::node_ref& node, QWidget* parent = nullptr);
            private slots:
                void set_position(glm::vec3 position);
                void set_rotation(glm::vec3 rotation);
                void set_scale(glm::vec3 scale);
            public slots:
                void load_values();
            
        };

    } // namespace ui
} // namespace element 
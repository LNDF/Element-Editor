#pragma once

#include <scenegraph/nodes/camera_node_ref.h>
#include <ui/widgets/numeric_input.h>
#include <ui/widgets/properties_standard_form.h>
#include <QWidget>

namespace element {
    namespace ui {
        
        class properties_node_camera_form : public properties_standard_form {
            Q_OBJECT
            private:
                scenegraph::camera_node_ref node;
                double_input* near_plane_input;
                double_input* far_plane_input;
                double_input* fov_input;
            public:
                properties_node_camera_form(const scenegraph::camera_node_ref& node, QWidget* parent = nullptr);
            private slots:
                void set_near_plane(double near_plane);
                void set_far_plane(double far_plane);
                void set_fov(double fov);
            public slots:
                void load_values();
            
        };

    } // namespace ui
} // namespace element 
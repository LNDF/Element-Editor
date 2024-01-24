#pragma once

#include <scenegraph/node_ref.h>
#include <ui/widgets/properties_container.h>
#include <ui/widgets/properties_transform.h>

namespace element {
    namespace ui {
        
        class properties_node : public properties_container {
            Q_OBJECT
            private:
                properties_transform* transform;
            public:
                properties_node(const scenegraph::node_ref& node, QWidget* parent = nullptr);
            public slots:
                virtual void load_values();
        };

    } // namespace ui
} // namespace element

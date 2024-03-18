#pragma once

#include <ui/widgets/properties_container.h>
#include <ui/widgets/properties_scene_form.h>
#include <utils/uuid.h>
#include <QPushButton>

namespace element {
    namespace ui {

        class properties_scene : public properties_container {
            Q_OBJECT
            private:
                uuid scene;
                properties_scene_form* general_properties;
                QPushButton* load_scene_button;
            public:
                properties_scene(const uuid& scene, QWidget* parent = nullptr);
            public slots:
                virtual void load_values();
            private slots:
                void load_scene();
        };

    } // namespace ui
} // namespace element
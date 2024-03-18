#pragma once

#include <ui/widgets/properties_standard_form.h>
#include <utils/uuid.h>

namespace element {
    namespace ui {

        class properties_scene_form : public properties_standard_form {
            Q_OBJECT
            public:
                properties_scene_form(const uuid& scene, QWidget* parent = nullptr);
            public slots:
                virtual void load_values();
        };

    }
}
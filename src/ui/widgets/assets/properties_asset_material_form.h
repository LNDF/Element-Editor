#pragma once

#include <render/material.h>
#include <ui/widgets/asset_input.h>
#include <ui/widgets/properties_standard_form.h>
#include <utils/uuid.h>

namespace element {
    namespace ui {
        
        class properties_asset_material_form : public properties_standard_form {
            Q_OBJECT
            private:
                render::material* data;
                asset_input* pipeline_input;
            public:
                properties_asset_material_form(render::material* data, QWidget* parent = nullptr);
            public slots:
                void load_values();
        };

    } // namespace ui
} // namespace element

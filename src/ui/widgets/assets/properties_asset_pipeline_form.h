#pragma once

#include <render/pipeline.h>
#include <ui/widgets/asset_input.h>
#include <ui/widgets/properties_standard_form.h>
#include <utils/uuid.h>
#include <QComboBox>
#include <QCheckBox>

namespace element {
    namespace ui {
        
        class properties_asset_pipeline_form : public properties_standard_form {
            Q_OBJECT
            private:
                render::pipeline_data* data;
                asset_input* vert_input;
                asset_input* frag_input;
                QCheckBox* backface_culling_input;
                QComboBox* frontface_input;
                QCheckBox* transparent_input;
            public:
                properties_asset_pipeline_form(render::pipeline_data* data, QWidget* parent = nullptr);
            private slots:
                void set_vert(uuid id);
                void set_frag(uuid id);
                void set_backface_culling(int cull);
                void set_frontface(int face);
                void set_transparent(int transparent);
            public slots:
                void load_values();
        };

    } // namespace ui    
} // namespace element
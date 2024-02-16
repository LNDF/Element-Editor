#pragma once

#include <render/pipeline.h>
#include <ui/widgets/properties_asset.h>
#include <ui/widgets/assets/properties_asset_pipeline_form.h>
#include <utils/uuid.h>

namespace element {
    namespace ui {
        
        class properties_asset_pipeline : public properties_asset {
            Q_OBJECT
            private:
                uuid id;
                render::pipeline_data data;
                properties_asset_pipeline_form* pipeline;
            public:
                properties_asset_pipeline(const uuid& id, QWidget* parent = nullptr);
            public slots:
                virtual void save_values();
                virtual void load_values();
        };

    } // namespace ui    
} // namespace element
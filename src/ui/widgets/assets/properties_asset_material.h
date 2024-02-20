#pragma once

#include <render/material.h>
#include <ui/widgets/properties_asset.h>
#include <ui/widgets/assets/properties_asset_material_form.h>
#include <utils/uuid.h>
#include <vector>

namespace element {
    namespace ui {
        
        class properties_asset_material : public properties_asset {
            Q_OBJECT
            private:
                uuid id;
                render::material data;
                bool material_loaded;
                properties_asset_material_form* material;
                std::vector<properties_asset_material_layout_form*> resource_layouts;
            public:
                properties_asset_material(const uuid& id, QWidget* parent = nullptr);
            private slots:
                void load_material_properties();
            public slots:
                virtual void save_values();
                virtual void load_values();
        };

    } // namespace ui
} // namespace element
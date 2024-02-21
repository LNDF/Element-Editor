#pragma once

#include <render/material.h>
#include <render/pipeline.h>
#include <ui/widgets/asset_input.h>
#include <ui/widgets/properties_standard_form.h>
#include <utils/uuid.h>
#include <functional>
#include <vector>

namespace element {
    namespace ui {
        
        class properties_asset_material_layout_form : public properties_standard_form {
            Q_OBJECT
            private:
                render::material* data;
                render::shader_resource_layout* data_layout;
                std::vector<QWidget*> widgets;

                std::function<QWidget*(QWidget*)> get_widget_factory(const render::shader_block_member& member);
                
                template<typename T>
                void load_value_widget(QWidget* widget, const render::shader_block_member& member, void(*setter)(QWidget*, const T&));

                template<typename T>
                void save_value_widget(QWidget* widget, const render::shader_block_member& member, T(*getter)(QWidget*));

                void load_value(QWidget* widget, const render::shader_block_member& member);
                void save_value(QWidget* widget, const render::shader_block_member& member);
            public:
                properties_asset_material_layout_form(render::material* data, render::shader_resource_layout* data_layout, QWidget* parent = nullptr);
            
                inline const std::vector<QWidget*>& get_widgets() {return widgets;}
            public slots:
                void save_values();
                void load_values();
        };

        class properties_asset_material_form : public properties_standard_form {
            Q_OBJECT
            private:
                render::material* data;
                asset_input* pipeline_input;
            public:
                properties_asset_material_form(render::material* data, QWidget* parent = nullptr);

                inline uuid get_pipeline_id() {return pipeline_input->value();}
            public slots:
                void load_values();
        };

    } // namespace ui
} // namespace element

#include "properties_asset_material_form.h"

#include <ui/widgets/array_input.h>
#include <ui/widgets/matrix_input.h>
#include <ui/widgets/numeric_input.h>
#include <ui/widgets/vector_input.h>
#include <QCoreApplication>
#include <QCheckBox>
#include <functional>
#include <limits>

using namespace element::ui;
using namespace element;

template<typename T>
static QWidget* generic_widget_factory(QWidget* parent = nullptr) {
    return new T(parent);
}

static std::function<QWidget*(QWidget*)> get_widget_factory(const render::shader_block_member& member) {
    switch (member.type) {
        case render::shader_block_member_type::boolean_type:
            if (member.columns > 1) return nullptr;
            if (member.vecsize > 1) {
                //TODO: implement boolean vector
                return nullptr;
            }
            return generic_widget_factory<QCheckBox>;
        case render::shader_block_member_type::sint32_type:
            if (member.columns > 1) return nullptr;
            if (member.vecsize > 1) {
                //TODO: implement integer vectors
                return nullptr;
            }
            return generic_widget_factory<integer_input>;
        case render::shader_block_member_type::uint32_type:
            //TODO: implement uint32 input and vector.
        case render::shader_block_member_type::float32_type:
            if (member.columns > 1) {
                switch (member.columns) {
                    case 2:
                        switch (member.vecsize) {
                            case 2:
                                return generic_widget_factory<mat2_input>;
                            case 3:
                                return generic_widget_factory<mat23_input>;
                            case 4:
                                return generic_widget_factory<mat24_input>;
                            default:
                                return nullptr;
                        }
                    case 3:
                        switch (member.vecsize) {
                            case 2:
                                return generic_widget_factory<mat32_input>;
                            case 3:
                                return generic_widget_factory<mat3_input>;
                            case 4:
                                return generic_widget_factory<mat34_input>;
                            default:
                                return nullptr;
                        }
                    case 4:
                        switch (member.vecsize) {
                            case 2:
                                return generic_widget_factory<mat42_input>;
                            case 3:
                                return generic_widget_factory<mat43_input>;
                            case 4:
                                return generic_widget_factory<mat4_input>;
                            default:
                                return nullptr;
                        }
                    default:
                        return nullptr;
                }
            }
            if (member.vecsize > 1) {
                switch (member.vecsize) {
                    case 2:
                        return generic_widget_factory<vec2_input>;
                    case 3:
                        return generic_widget_factory<vec3_input>;
                    case 4:
                        return generic_widget_factory<vec4_input>;
                    default:
                        return nullptr;
                }
            }
            return generic_widget_factory<float_input>;
        case render::shader_block_member_type::float64_type:
            //TODO
            return nullptr;
        default:
            return nullptr;
    }
}

properties_asset_material_layout_form::properties_asset_material_layout_form(render::material* data, render::shader_resource_layout* data_layout, QWidget* parent ) : properties_standard_form(parent), data(data), data_layout(data_layout) {
    setTitle(data_layout->name.c_str());
    for (auto& member : data_layout->members) {
        auto factory = get_widget_factory(member);
        if (factory == nullptr) continue;
        QWidget* widget = nullptr;
        if (member.array_cols > 1) {
            widget = new array2d_input(member.array_cols, member.array_rows, factory, this);
            //TODO: connect
        } else if (member.array_rows > 1) {
            widget = new array_input(member.array_rows, factory, this);
            //TODO: connect
        } else {
            widget = factory(this);
            //TODO: connect
        }
        add_property(member.name.c_str(), widget);
    }
}

void properties_asset_material_layout_form::load_values() {

}

properties_asset_material_form::properties_asset_material_form(render::material* data, QWidget* parent) : properties_standard_form(parent), data(data) {
    setTitle(QCoreApplication::translate("element-editor", "Material"));
    pipeline_input = new asset_input("pipeline", this);
    pipeline_input->setObjectName("pipeline_input");
    add_property(QCoreApplication::translate("element-editor", "Pipeline"), pipeline_input);
    connect(pipeline_input, SIGNAL(value_changed(uuid)), this, SIGNAL(values_changed()), Qt::ConnectionType::DirectConnection);
}

void properties_asset_material_form::load_values() {
    pipeline_input->set_value(data->get_pipeline_id());
}
#include "properties_asset_material_form.h"

#include <ui/widgets/array_input.h>
#include <ui/widgets/matrix_input.h>
#include <ui/widgets/numeric_input.h>
#include <ui/widgets/vector_input.h>
#include <QCoreApplication>
#include <QCheckBox>
#include <limits>

using namespace element::ui;
using namespace element;

static std::uint8_t checkbox_get(QWidget* widget) {
    return (static_cast<QCheckBox*>(widget))->isChecked();
}

static void checkbox_set(QWidget* widget, const std::uint8_t& checked) {
    (static_cast<QCheckBox*>(widget))->setChecked(checked);
}

template<typename T, typename W>
static T input_get(QWidget* widget) {
    return (static_cast<W*>(widget))->value();
}

template<typename T, typename W>
static void input_set(QWidget* widget, const T& t) {
    (static_cast<W*>(widget))->set_value(t);
}

template<typename T, typename W>
static T spin_get(QWidget* widget) {
    return (static_cast<W*>(widget))->value();
}

template<typename T, typename W>
static void spin_set(QWidget* widget, const T& t) {
    (static_cast<W*>(widget))->setValue(t);
}

template<typename T>
static QWidget* generic_widget_factory(QWidget* parent = nullptr) {
    return new T(parent);
}

std::function<QWidget*(QWidget*)> properties_asset_material_layout_form::get_widget_factory(const render::shader_block_member& member) {
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
                switch (member.vecsize) {
                    case 2:
                        return generic_widget_factory<ivec2_input>;
                    case 3:
                        return generic_widget_factory<ivec3_input>;
                    case 4:
                        return generic_widget_factory<ivec4_input>;
                    default:
                        return nullptr;
                }
            }
            return generic_widget_factory<integer_input>;
        case render::shader_block_member_type::uint32_type:
            //TODO: implement uint32 input and vector.
            return nullptr;
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
            if (member.columns > 1) {
                switch (member.columns) {
                    case 2:
                        switch (member.vecsize) {
                            case 2:
                                return generic_widget_factory<dmat2_input>;
                            case 3:
                                return generic_widget_factory<dmat23_input>;
                            case 4:
                                return generic_widget_factory<dmat24_input>;
                            default:
                                return nullptr;
                        }
                    case 3:
                        switch (member.vecsize) {
                            case 2:
                                return generic_widget_factory<dmat32_input>;
                            case 3:
                                return generic_widget_factory<dmat3_input>;
                            case 4:
                                return generic_widget_factory<dmat34_input>;
                            default:
                                return nullptr;
                        }
                    case 4:
                        switch (member.vecsize) {
                            case 2:
                                return generic_widget_factory<dmat42_input>;
                            case 3:
                                return generic_widget_factory<dmat43_input>;
                            case 4:
                                return generic_widget_factory<dmat4_input>;
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
                        return generic_widget_factory<dvec2_input>;
                    case 3:
                        return generic_widget_factory<dvec3_input>;
                    case 4:
                        return generic_widget_factory<dvec4_input>;
                    default:
                        return nullptr;
                }
            }
            return generic_widget_factory<double_input>;
        default:
            return nullptr;
    }
}

template<typename T>
void properties_asset_material_layout_form::load_value_widget(QWidget* widget, const render::shader_block_member& member, void(*setter)(QWidget*, const T&)) {
    if (member.array_cols > 1 || member.array_rows > 1) {
        const std::vector<QWidget*>* widgets;
        std::vector<T> ts;
        std::size_t i = 0;
        if (member.array_rows > 1) {
            widgets = &(static_cast<array2d_input*>(widget))->get_widgets();
        } else {
            widgets = &(static_cast<array_input*>(widget))->get_widgets();
        }
        ts.resize(widgets->size());
        data->get_property_array(member.name, &ts[0]);
        for (const T& t : ts) {
            setter(widgets->at(i++), t);
        }
    } else {
        T t;
        data->get_property(member.name, t);
        setter(widget, t);
    }
}

template<typename T>
void properties_asset_material_layout_form::save_value_widget(QWidget* widget, const render::shader_block_member& member, T(*getter)(QWidget*)) {
    if (member.array_cols > 1 || member.array_rows > 1) {
        const std::vector<QWidget*>* widgets;
        std::vector<T> ts;
        if (member.array_cols > 1) {
            widgets = &(static_cast<array2d_input*>(widget))->get_widgets();
        } else {
            widgets = &(static_cast<array_input*>(widget))->get_widgets();
        }
        ts.reserve(widgets->size());
        for (QWidget* widget : *widgets) {
            ts.push_back(getter(widget));
        }
        data->set_property_array(member.name, ts.data());
    } else {
        T t = getter(widget);
        data->set_property(member.name, t);
    }
}

void properties_asset_material_layout_form::load_value(QWidget* widget, const render::shader_block_member& member) {
    switch (member.type) {
        case render::shader_block_member_type::boolean_type:
            if (member.columns > 1) break;
            if (member.vecsize > 1) {
                //TODO: implement boolean vector
                break;
            }
            load_value_widget<std::uint8_t>(widget, member, checkbox_set);
            break;
        case render::shader_block_member_type::sint32_type:
            if (member.columns > 1) break;
            if (member.vecsize > 1) {
                switch (member.vecsize) {
                    case 2:
                        load_value_widget<glm::ivec2>(widget, member, input_set<glm::ivec2, ivec2_input>);
                        break;
                    case 3:
                        load_value_widget<glm::ivec3>(widget, member, input_set<glm::ivec3, ivec3_input>);
                        break;
                    case 4:
                        load_value_widget<glm::ivec4>(widget, member, input_set<glm::ivec4, ivec4_input>);
                        break;
                    default:
                        break;
                }
                break;
            }
            load_value_widget<int>(widget, member, spin_set<int, integer_input>);
            break;
        case render::shader_block_member_type::uint32_type:
            //TODO: implement uint32 input and vector.
            break;
        case render::shader_block_member_type::float32_type:
            if (member.columns > 1) {
                switch (member.columns) {
                    case 2:
                        switch (member.vecsize) {
                            case 2:
                                load_value_widget<glm::mat2>(widget, member, input_set<glm::mat2, mat2_input>);
                                break;
                            case 3:
                                load_value_widget<glm::mat2x3>(widget, member, input_set<glm::mat2x3, mat23_input>);
                                break;
                            case 4:
                                load_value_widget<glm::mat2x4>(widget, member, input_set<glm::mat2x4, mat24_input>);
                                break;
                            default:
                                break;
                        }
                        break;
                    case 3:
                        switch (member.vecsize) {
                            case 2:
                                load_value_widget<glm::mat3x2>(widget, member, input_set<glm::mat3x2, mat32_input>);
                                break;
                            case 3:
                                load_value_widget<glm::mat3>(widget, member, input_set<glm::mat3, mat3_input>);
                                break;
                            case 4:
                                load_value_widget<glm::mat3x4>(widget, member, input_set<glm::mat3x4, mat34_input>);
                                break;
                            default:
                                break;
                        }
                        break;
                    case 4:
                        switch (member.vecsize) {
                            case 2:
                                load_value_widget<glm::mat4x2>(widget, member, input_set<glm::mat4x2, mat42_input>);
                                break;
                            case 3:
                                load_value_widget<glm::mat4x3>(widget, member, input_set<glm::mat4x3, mat43_input>);
                                break;
                            case 4:
                                load_value_widget<glm::mat4>(widget, member, input_set<glm::mat4, mat4_input>);
                                break;
                            default:
                                break;
                        }
                        break;
                    default:
                        break;
                }
                break;
            }
            if (member.vecsize > 1) {
                switch (member.vecsize) {
                    case 2:
                        load_value_widget<glm::vec2>(widget, member, input_set<glm::vec2, vec2_input>);
                        break;
                    case 3:
                        load_value_widget<glm::vec3>(widget, member, input_set<glm::vec3, vec3_input>);
                        break;
                    case 4:
                        load_value_widget<glm::vec4>(widget, member, input_set<glm::vec4, vec4_input>);
                        break;
                    default:
                        break;
                }
                break;
            }
            load_value_widget<float>(widget, member, spin_set<float, float_input>);
            break;
        case render::shader_block_member_type::float64_type:
            if (member.columns > 1) {
                switch (member.columns) {
                    case 2:
                        switch (member.vecsize) {
                            case 2:
                                load_value_widget<glm::dmat2>(widget, member, input_set<glm::dmat2, dmat2_input>);
                                break;
                            case 3:
                                load_value_widget<glm::dmat2x3>(widget, member, input_set<glm::dmat2x3, dmat23_input>);
                                break;
                            case 4:
                                load_value_widget<glm::dmat2x4>(widget, member, input_set<glm::dmat2x4, dmat24_input>);
                                break;
                            default:
                                break;
                        }
                        break;
                    case 3:
                        switch (member.vecsize) {
                            case 2:
                                load_value_widget<glm::dmat3x2>(widget, member, input_set<glm::dmat3x2, dmat32_input>);
                                break;
                            case 3:
                                load_value_widget<glm::dmat3>(widget, member, input_set<glm::dmat3, dmat3_input>);
                                break;
                            case 4:
                                load_value_widget<glm::dmat3x4>(widget, member, input_set<glm::dmat3x4, dmat34_input>);
                                break;
                            default:
                                break;
                        }
                        break;
                    case 4:
                        switch (member.vecsize) {
                            case 2:
                                load_value_widget<glm::dmat4x2>(widget, member, input_set<glm::dmat4x2, dmat42_input>);
                                break;
                            case 3:
                                load_value_widget<glm::dmat4x3>(widget, member, input_set<glm::dmat4x3, dmat43_input>);
                                break;
                            case 4:
                                load_value_widget<glm::dmat4>(widget, member, input_set<glm::dmat4, dmat4_input>);
                                break;
                            default:
                                break;
                        }
                        break;
                    default:
                        break;
                }
                break;
            }
            if (member.vecsize > 1) {
                switch (member.vecsize) {
                    case 2:
                        load_value_widget<glm::dvec2>(widget, member, input_set<glm::dvec2, dvec2_input>);
                        break;
                    case 3:
                        load_value_widget<glm::dvec3>(widget, member, input_set<glm::dvec3, dvec3_input>);
                        break;
                    case 4:
                        load_value_widget<glm::dvec4>(widget, member, input_set<glm::dvec4, dvec4_input>);
                        break;
                    default:
                        break;
                }
                break;
            }
            load_value_widget<double>(widget, member, spin_set<double, double_input>);
            break;
        default:
            break;
    }
}

void properties_asset_material_layout_form::save_value(QWidget* widget, const render::shader_block_member& member) {
    switch (member.type) {
        case render::shader_block_member_type::boolean_type:
            if (member.columns > 1) break;
            if (member.vecsize > 1) {
                //TODO: implement boolean vector
                break;
            }
            save_value_widget<std::uint8_t>(widget, member, checkbox_get);
            break;
        case render::shader_block_member_type::sint32_type:
            if (member.columns > 1) break;
            if (member.vecsize > 1) {
                switch (member.vecsize) {
                    case 2:
                        save_value_widget<glm::ivec2>(widget, member, input_get<glm::ivec2, ivec2_input>);
                        break;
                    case 3:
                        save_value_widget<glm::ivec3>(widget, member, input_get<glm::ivec3, ivec3_input>);
                        break;
                    case 4:
                        save_value_widget<glm::ivec4>(widget, member, input_get<glm::ivec4, ivec4_input>);
                        break;
                    default:
                        break;
                }
                break;
            }
            save_value_widget<int>(widget, member, spin_get<int, integer_input>);
            break;
        case render::shader_block_member_type::uint32_type:
            //TODO: implement uint32 input and vector.
            break;
        case render::shader_block_member_type::float32_type:
            if (member.columns > 1) {
                switch (member.columns) {
                    case 2:
                        switch (member.vecsize) {
                            case 2:
                                save_value_widget<glm::mat2>(widget, member, input_get<glm::mat2, mat2_input>);
                                break;
                            case 3:
                                save_value_widget<glm::mat2x3>(widget, member, input_get<glm::mat2x3, mat23_input>);
                                break;
                            case 4:
                                save_value_widget<glm::mat2x4>(widget, member, input_get<glm::mat2x4, mat24_input>);
                                break;
                            default:
                                break;
                        }
                        break;
                    case 3:
                        switch (member.vecsize) {
                            case 2:
                                save_value_widget<glm::mat3x2>(widget, member, input_get<glm::mat3x2, mat32_input>);
                                break;
                            case 3:
                                save_value_widget<glm::mat3>(widget, member, input_get<glm::mat3, mat3_input>);
                                break;
                            case 4:
                                save_value_widget<glm::mat3x4>(widget, member, input_get<glm::mat3x4, mat34_input>);
                                break;
                            default:
                                break;
                        }
                        break;
                    case 4:
                        switch (member.vecsize) {
                            case 2:
                                save_value_widget<glm::mat4x2>(widget, member, input_get<glm::mat4x2, mat42_input>);
                                break;
                            case 3:
                                save_value_widget<glm::mat4x3>(widget, member, input_get<glm::mat4x3, mat43_input>);
                                break;
                            case 4:
                                save_value_widget<glm::mat4>(widget, member, input_get<glm::mat4, mat4_input>);
                                break;
                            default:
                                break;
                        }
                        break;
                    default:
                        break;
                }
                break;
            }
            if (member.vecsize > 1) {
                switch (member.vecsize) {
                    case 2:
                        save_value_widget<glm::vec2>(widget, member, input_get<glm::vec2, vec2_input>);
                        break;
                    case 3:
                        save_value_widget<glm::vec3>(widget, member, input_get<glm::vec3, vec3_input>);
                        break;
                    case 4:
                        save_value_widget<glm::vec4>(widget, member, input_get<glm::vec4, vec4_input>);
                        break;
                    default:
                        break;
                }
                break;
            }
            save_value_widget<float>(widget, member, spin_get<float, float_input>);
            break;
        case render::shader_block_member_type::float64_type:
            if (member.columns > 1) {
                switch (member.columns) {
                    case 2:
                        switch (member.vecsize) {
                            case 2:
                                save_value_widget<glm::dmat2>(widget, member, input_get<glm::dmat2, dmat2_input>);
                                break;
                            case 3:
                                save_value_widget<glm::dmat2x3>(widget, member, input_get<glm::dmat2x3, dmat23_input>);
                                break;
                            case 4:
                                save_value_widget<glm::dmat2x4>(widget, member, input_get<glm::dmat2x4, dmat24_input>);
                                break;
                            default:
                                break;
                        }
                        break;
                    case 3:
                        switch (member.vecsize) {
                            case 2:
                                save_value_widget<glm::dmat3x2>(widget, member, input_get<glm::dmat3x2, dmat32_input>);
                                break;
                            case 3:
                                save_value_widget<glm::dmat3>(widget, member, input_get<glm::dmat3, dmat3_input>);
                                break;
                            case 4:
                                save_value_widget<glm::dmat3x4>(widget, member, input_get<glm::dmat3x4, dmat34_input>);
                                break;
                            default:
                                break;
                        }
                        break;
                    case 4:
                        switch (member.vecsize) {
                            case 2:
                                save_value_widget<glm::dmat4x2>(widget, member, input_get<glm::dmat4x2, dmat42_input>);
                                break;
                            case 3:
                                save_value_widget<glm::dmat4x3>(widget, member, input_get<glm::dmat4x3, dmat43_input>);
                                break;
                            case 4:
                                save_value_widget<glm::dmat4>(widget, member, input_get<glm::dmat4, dmat4_input>);
                                break;
                            default:
                                break;
                        }
                        break;
                    default:
                        break;
                }
                break;
            }
            if (member.vecsize > 1) {
                switch (member.vecsize) {
                    case 2:
                        save_value_widget<glm::dvec2>(widget, member, input_get<glm::dvec2, dvec2_input>);
                        break;
                    case 3:
                        save_value_widget<glm::dvec3>(widget, member, input_get<glm::dvec3, dvec3_input>);
                        break;
                    case 4:
                        save_value_widget<glm::dvec4>(widget, member, input_get<glm::dvec4, dvec4_input>);
                        break;
                    default:
                        break;
                }
                break;
            }
            save_value_widget<double>(widget, member, spin_get<double, double_input>);
            break;
        default:
            break;
    }
}

properties_asset_material_layout_form::properties_asset_material_layout_form(render::material* data, render::shader_resource_layout* data_layout, QWidget* parent ) : properties_standard_form(parent), data(data), data_layout(data_layout) {
    setTitle(data_layout->name.c_str());
    for (auto& member : data_layout->members) {
        auto factory = get_widget_factory(member);
        QWidget* widget = nullptr;
        if (factory == nullptr) {
            widget = new QWidget(this);
        } else if (member.array_cols > 1) {
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
        widgets.push_back(widget);
    }
}

void properties_asset_material_layout_form::save_values() {
    for (std::size_t i = 0; i < widgets.size(); ++i) {
        save_value(widgets[i], data_layout->members[i]);
    }
}

void properties_asset_material_layout_form::load_values() {
    for (std::size_t i = 0; i < widgets.size(); ++i) {
        load_value(widgets[i], data_layout->members[i]);
    }
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
#pragma once

#include <scenegraph/node.h>
#include <scenegraph/node_ref.h>
#include <ui/widgets/properties_container.h>
#include <ui/widgets/properties_transform_form.h>
#include <utils/pre_exec.h>
#include <type_traits>

#ifndef __ELM_ARG_SEP
#define __ELM_ARG_SEP ,
#endif

#define ELM_REGISTER_NODE_PROPERTIES_CONTAINER(node, properties) ELM_PRE_EXECUTE(element::__detail::__ui_register_node_properties_container_info<node __ELM_ARG_SEP properties>)

namespace element {
    namespace ui {
        class properties_node;
        
        using properties_node_container_factory = std::function<properties_container*(const scenegraph::node_ref& node, QWidget* parent)>;
    } // namespace ui

    namespace __detail {
        std::unordered_map<std::type_index, ui::properties_node_container_factory>& __ui_get_node_properties_container_map();

        template<typename P, typename = std::enable_if<std::is_base_of_v<ui::properties_node, P>>>
        ui::properties_node* __ui_properties_node_container_factory_fun(const scenegraph::node_ref& node, QWidget* parent = nullptr) {
            return new P(node, parent);
        }

        template<typename T, typename P, typename = std::enable_if<std::is_base_of_v<scenegraph::node, T>>, typename = std::enable_if<std::is_base_of_v<ui::properties_node, P>>>
        void __ui_register_node_properties_container_info() {
            __ui_get_node_properties_container_map().try_emplace(std::type_index(typeid(T)), __ui_properties_node_container_factory_fun<P>);
        }
    } // namespace __detail

    namespace ui {
        class properties_node : public properties_container {
            Q_OBJECT
            private:
                properties_transform_form* transform;
            public:
                properties_node(const scenegraph::node_ref& node, QWidget* parent = nullptr);
            public slots:
                virtual void load_values();
        };

        properties_node_container_factory get_node_properties_container_factory(std::type_index type);
    } // namespace ui
} // namespace element
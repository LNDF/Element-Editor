#pragma once

#include <ui/widgets/properties_container.h>
#include <utils/pre_exec.h>
#include <utils/uuid.h>
#include <QWidget>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QPushButton>

#define ELM_REGISTER_ASSET_PROPERTIES_CONTAINER(type, properties) ELM_PRE_EXECUTE(element::__detail::__ui_register_asset_properties_container_info<properties>, type)

namespace element {
    namespace ui {
        class properties_asset;

        using properties_asset_container_factory = std::function<properties_container*(const uuid& node, QWidget* parent)>;
    } // namespace ui

    namespace __detail {
        std::unordered_map<std::string, ui::properties_asset_container_factory>& __ui_get_asset_properties_container_map();

        template<typename P, typename = std::enable_if<std::is_base_of_v<ui::properties_asset, P>>>
        ui::properties_asset* __ui_properties_asset_container_factory_fun(const uuid& id, QWidget* parent = nullptr) {
            return new P(id, parent);
        }

        template<typename P, typename = std::enable_if<std::is_base_of_v<ui::properties_asset, P>>>
        void __ui_register_asset_properties_container_info(std::string&& name) {
            __ui_get_asset_properties_container_map().try_emplace(std::move(name), __ui_properties_asset_container_factory_fun<P>);
        }
    } // namespace __detail

    namespace ui {
        
        class properties_asset : public properties_container {
            Q_OBJECT
            private:
                QWidget* save_reload;
                QHBoxLayout* save_reload_layout;
                QSpacerItem* save_reload_space;
                QPushButton* save_reload_save;
                QPushButton* save_reload_reload;
            public:
                properties_asset(const uuid& id, QWidget* parent = nullptr);
            private slots:
                void save_disable();
                void save_enable();
            public slots:
                virtual void save_values();
        };

        properties_asset_container_factory get_asset_properties_container_factory(const std::string& type);
    } // namespace ui    
} // namespace element
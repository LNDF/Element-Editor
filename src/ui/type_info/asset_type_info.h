#pragma once

#include <ui/widgets/properties_asset.h>
#include <utils/uuid.h>
#include <utils/pre_exec.h>
#include <QWidget>
#include <filesystem>
#include <functional>
#include <unordered_map>

#define ELM_REGISTER_ASSET_TYPE(type, name, properties, creator) ELM_PRE_EXECUTE(element::__detail::__ui_register_asset_type_info<properties>, type, name, creator)

namespace element {
    namespace ui {
        struct asset_type_info;

        template<typename T, typename = std::enable_if<std::is_base_of_v<properties_asset, T>>>
        properties_container* properties_asset_factory(const uuid& id, QWidget* parent = nullptr) {
            return new T(id, parent);
        }
    } // namespace ui

    namespace __detail {
        std::unordered_map<std::string, ui::asset_type_info>& __ui_get_asset_type_map();

        template<typename T, typename = std::enable_if<std::is_base_of_v<ui::properties_asset, T>>>
        void __ui_register_asset_type_info(std::string&& type, std::string&& name, std::function<void(const std::filesystem::path&)> creator) {
            __ui_get_asset_type_map().try_emplace(std::move(type), std::move(name), ui::properties_asset_factory<T>, creator);
        }
    } // namespace __detail

    namespace ui {
        struct asset_type_info {
            using properties_factory_type = std::function<properties_container*(const uuid&)>;
            using creator_type = std::function<void(const std::filesystem::path&)>;

            std::string name;
            properties_factory_type properties_factory;
            creator_type creator;

            asset_type_info(const std::string& name, properties_factory_type properties_factory, creator_type creator);
            asset_type_info(std::string&& name, properties_factory_type properties_factory, creator_type creator);
        };

        inline const std::unordered_map<std::string, ui::asset_type_info>& get_asset_type_map() {return __detail::__ui_get_asset_type_map();}
        inline const asset_type_info& get_asset_type_info(const std::string& type) {return get_asset_type_map().at(type);}
        
    } // namespace ui
} // namespace element
#pragma once

#include <ui/widgets/properties_asset.h>
#include <utils/uuid.h>
#include <QWidget>
#include <filesystem>
#include <functional>
#include <unordered_map>

namespace element {
    namespace ui {
        struct asset_type_info;
    } // namespace ui

    namespace __detail {
        std::unordered_map<std::string, ui::asset_type_info> __ui_get_asset_type_map();
    } // namespace __detail

    namespace ui {
        struct asset_type_info {
            using properties_factory_type = std::function<properties_asset*(const uuid&)>;
            using creator_type = std::function<void(const std::filesystem::path&)>;

            std::string name;
            properties_factory_type properties_factory;
            creator_type creator;

            asset_type_info(const std::string& name, properties_factory_type properties_factory, creator_type creator);
            asset_type_info(std::string&& name, properties_factory_type properties_factory, creator_type creator);
        };

        const asset_type_info& get_asset_type_info(const std::string& type);
        
    } // namespace ui
} // namespace element
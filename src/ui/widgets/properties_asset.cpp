#include "properties_asset.h"

using namespace element::ui;

std::unordered_map<std::string, element::ui::properties_asset_container_factory>& element::__detail::__ui_get_asset_properties_container_map() {
    static std::unordered_map<std::string, properties_asset_container_factory> map;
    return map;
}

properties_asset_container_factory element::ui::get_asset_properties_container_factory(const std::string& type) {
    auto& map = element::__detail::__ui_get_asset_properties_container_map();
    auto it = map.find(type);
    if (it == map.end()) return nullptr;
    return it->second;
}

properties_asset::properties_asset(const uuid& id, QWidget* parent) : properties_container(parent) {
    
}
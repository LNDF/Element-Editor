#include "properties_asset.h"

#include <QCoreApplication>

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
    save_reload = new QWidget(this);
    save_reload_layout = new QHBoxLayout(save_reload);
    save_reload_space = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    save_reload_save = new QPushButton(save_reload);
    save_reload_reload = new QPushButton(save_reload);
    save_reload->setObjectName("save_reload");
    save_reload_layout->setObjectName("save_reload_layout");
    save_reload_save->setObjectName("save_reload_save");
    save_reload_reload->setObjectName("save_reload_reload");
    save_reload_save->setText(QCoreApplication::translate("element-editor", "Save"));
    save_reload_reload->setText(QCoreApplication::translate("element-editor", "Reload"));
    save_reload_layout->addItem(save_reload_space);
    save_reload_layout->addWidget(save_reload_save);
    save_reload_layout->addWidget(save_reload_reload);
}
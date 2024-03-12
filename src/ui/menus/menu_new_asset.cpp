#include "menu_new_asset.h"

#include <QCoreApplication>

using namespace element::ui;

menu_new_asset_action::menu_new_asset_action(const std::string& type, QObject* parent) : QAction(parent) {
    retranslate();
    connect(this, SIGNAL(triggered(bool)), this, SLOT(create_asset()), Qt::ConnectionType::DirectConnection);
}

void menu_new_asset_action::retranslate() {
    const auto& info = asset_creator::get_creator_info(type);
    setText(QCoreApplication::translate("element-editor", info.name.c_str()));
}

void menu_new_asset_action::create_asset() {
    emit trigger(type);
}

menu_new_asset::menu_new_asset(QWidget* parent) : menu_new_asset("", parent) {}


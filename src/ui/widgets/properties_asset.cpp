#include "properties_asset.h"

#include <QCoreApplication>

using namespace element::ui;

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
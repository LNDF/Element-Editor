#include "menu_new_asset.h"

#include <asset/creators/asset_creators_info.h>
#include <editor/project.h>
#include <QCoreApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <filesystem>

using namespace element::ui;

menu_new_asset_action::menu_new_asset_action(const std::string& type, QObject* parent) : QAction(parent), type(type) {
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

menu_new_asset::menu_new_asset(const std::string& parent_path, QWidget* parent) : QMenu(parent) {
    if (!parent_path.empty()) {
        this->parent_path = parent_path + "/";
    } else {
        this->parent_path = parent_path;
    }
    new_folder_action = new QAction(this);
    new_file_action = new QAction(this);
    connect(new_folder_action, SIGNAL(triggered(bool)), this, SLOT(action_new_folder()), Qt::ConnectionType::DirectConnection);
    connect(new_file_action, SIGNAL(triggered(bool)), this, SLOT(action_new_file()), Qt::ConnectionType::DirectConnection);
    addAction(new_folder_action);
    addAction(new_file_action);
    addSeparator();
    for (const auto& [type, info] : asset_creator::get_creator_info_map()) {
        menu_new_asset_action* action = new menu_new_asset_action(type, this);
        addAction(action);
        connect(action, SIGNAL(trigger(const std::string&)), this, SLOT(action_triggered(const std::string&)), Qt::ConnectionType::DirectConnection);
        actions.push_back(action);
    }
    retranslate();
}

std::string menu_new_asset::get_file_path(bool* ok) {
    return QInputDialog::getText(this, QCoreApplication::translate("element-editor", "Filename"), QCoreApplication::translate("element-editor", "Enter the name for the new file."), QLineEdit::Normal, QString(), ok).toStdString();
}

bool menu_new_asset::check_exists(const std::string& path) {
    if (std::filesystem::exists(project::project_assets_path / path)) {
        QMessageBox box(this);
        box.setText(QCoreApplication::translate("element-editor", "Can't create file or directory"));
        box.setInformativeText(QCoreApplication::translate("element-editor", "The file or directory aleady exists."));
        box.setIcon(QMessageBox::Critical);
        box.setStandardButtons(QMessageBox::Ok);
        box.exec();
        return true;
    }
    return false;
}

void menu_new_asset::retranslate() {
    new_folder_action->setText(QCoreApplication::translate("element-editor", "Folder"));
    new_file_action->setText(QCoreApplication::translate("element-editor", "File"));
    for (const auto& action : actions) {
        action->retranslate();
    }
}

void menu_new_asset::action_triggered(const std::string& type) {
    bool ok;
    std::string path = get_file_path(&ok) + "." + type;
    if (!ok) return;
    if (check_exists(path)) return;
    const auto& info = asset_creator::get_creator_info(type);
    info.factory(path);
}

void menu_new_asset::action_new_folder() {
    bool ok;
    std::string path = QInputDialog::getText(this, QCoreApplication::translate("element-editor", "Folder name"), QCoreApplication::translate("element-editor", "Enter the name for the new folder."), QLineEdit::Normal, QString(), &ok).toStdString();
    if (!ok) return;
    if (check_exists(path)) return;
    std::filesystem::create_directories(project::project_assets_path / path);
}

void menu_new_asset::action_new_file() {
    bool ok;
    std::string path = get_file_path(&ok);
    if (!ok) return;
    if (check_exists(path)) return;
    asset_creator::blank_file_creator(path);
}
#include "project_properties.h"

#include <editor/project.h>
#include <plugins/editor.h>
#include <utils/platform.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <filesystem>

using namespace element::ui;

project_properties::project_properties(QWidget* parent) : QDialog(parent) {
    setupUi(this);
    setFixedSize(minimumSize());
    setWindowFlags(Qt::Sheet | Qt::Dialog);
    new_project = !project::exists();
    startup->setEnabled(!new_project);
    plugins->setEnabled(!new_project);
    startup_scene_input = new asset_input("scene", startup_scene_container);
    startup_scene_input->setObjectName("startup_scene_input");
    startup_scene_container->layout()->addWidget(startup_scene_input);
    required_inputs.push_back(project_name_edit);
    required_inputs.push_back(author_edit);
    required_inputs.push_back(version_edit);
    project_name_edit->setText(project::name.c_str());
    version_edit->setText(project::version.c_str());
    author_edit->setText(project::author.c_str());
    startup_scene_input->set_value(project::startup_scene);
    plugin_list_models.reserve(plugins::platforms.size());
    for (auto platform : plugins::platforms) {
        const auto& plugins = project::plugins[platform];
        QStringList plugin_list;
        for (const auto& plugin : plugins) {
            plugin_list << QString::fromStdString(plugin);
        }
        plugin_list_models.push_back(new QStringListModel(plugin_list, this));
    }
    handle_apply_activation();
    handle_plugin_platform_change(plugins_platforms->currentIndex());
    buttons->button(QDialogButtonBox::Apply)->setEnabled(false);
    connect(this, SIGNAL(finished(int)), this, SLOT(reload_plugins()), Qt::ConnectionType::DirectConnection);
    connect(buttons, SIGNAL(clicked(QAbstractButton*)), this, SLOT(handle_click(QAbstractButton*)), Qt::ConnectionType::DirectConnection);
    connect(startup_scene_input, SIGNAL(value_changed(uuid)), this, SLOT(handle_apply_activation()), Qt::ConnectionType::DirectConnection);
    for (auto input : required_inputs) {
        connect(input, SIGNAL(textChanged(const QString&)), this, SLOT(handle_apply_activation()), Qt::ConnectionType::DirectConnection);
    }
    connect(plugins_platforms, SIGNAL(currentIndexChanged(int)), this, SLOT(handle_plugin_platform_change(int)), Qt::ConnectionType::DirectConnection);
    connect(plugins_list_add, SIGNAL(clicked()), this, SLOT(add_plugin()), Qt::ConnectionType::DirectConnection);
    connect(plugins_list_delete, SIGNAL(clicked()), this, SLOT(remove_plugin()), Qt::ConnectionType::DirectConnection);
    connect(plugins_list_up, SIGNAL(clicked()), this, SLOT(move_plugin_up()), Qt::ConnectionType::DirectConnection);
    connect(plugins_list_down, SIGNAL(clicked()), this, SLOT(move_plugin_down()), Qt::ConnectionType::DirectConnection);
}

void project_properties::handle_click(QAbstractButton* button) {
    if (button == buttons->button(QDialogButtonBox::Ok)) {
        emit apply_properties();
        emit accept();
    } else
    if (button == buttons->button(QDialogButtonBox::Apply)) {
        emit apply_properties();
    } else if (button == buttons->button(QDialogButtonBox::Cancel)) {
        emit reject();
    }
}

void project_properties::apply_properties() {
    buttons->button(QDialogButtonBox::Apply)->setEnabled(false);
    project::name = project_name_edit->text().toStdString();
    project::version = version_edit->text().toStdString();
    project::author = author_edit->text().toStdString();
    project::startup_scene = startup_scene_input->value();
    for (std::size_t i = 0; i < plugins::platforms.size(); i++) {
        QStringListModel* model = plugin_list_models[i];
        std::vector<std::string> plugins;
        plugins.reserve(model->rowCount());
        for (int j = 0; j < model->rowCount(); j++) {
            plugins.push_back(model->data(model->index(j)).toString().toStdString());
        }
        project::plugins[plugins::platforms[i]] = plugins;
    }
    if (plugins_modified) plugin_modifications_applied = true;
    project::save();
}

void project_properties::reload_plugins() {
    if (plugin_modifications_applied) {
        QMessageBox dialog;
        dialog.setWindowTitle(QCoreApplication::translate("element-editor", "Reload plugins"));
        dialog.setText(QCoreApplication::translate("element-editor", "The plugins have been modified. Do you want to reload the plugins?"));
        dialog.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        dialog.setDefaultButton(QMessageBox::Yes);
        if (dialog.exec() == QMessageBox::Yes) {
            plugins::load_plugins();
        }
    }
}

void project_properties::disable_apply() {
    buttons->button(QDialogButtonBox::Ok)->setEnabled(false);
    buttons->button(QDialogButtonBox::Apply)->setEnabled(false);
}

void project_properties::enable_apply() {
    buttons->button(QDialogButtonBox::Ok)->setEnabled(true);
    if (!new_project) buttons->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void project_properties::handle_apply_activation() {
    for (auto input : required_inputs) {
        if (input->text().isEmpty()) {
            disable_apply();
            return;
        }
    }
    enable_apply();
}

void project_properties::handle_plugin_platform_change(int index) {
    plugins_list->setModel(plugin_list_models[index]);
    disable_plugin_modify_buttons();
    connect(plugins_list->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(handle_plugin_select(const QModelIndex&)), Qt::ConnectionType::DirectConnection);
}

void project_properties::handle_plugin_select(const QModelIndex& index) {
    if (index.isValid()) {
        enable_plugin_modify_buttons();
    } else {
        disable_plugin_modify_buttons();
    }
}

void project_properties::disable_plugin_modify_buttons() {
    plugins_list_delete->setEnabled(false);
    plugins_list_up->setEnabled(false);
    plugins_list_down->setEnabled(false);
}

void project_properties::enable_plugin_modify_buttons() {
    plugins_list_delete->setEnabled(true);
    plugins_list_up->setEnabled(true);
    plugins_list_down->setEnabled(true);
}

void project_properties::add_plugin() {
    QStringList filters = {
        QCoreApplication::translate("element-editor", "All Plugins") + " (*.dll *.so)",
        QCoreApplication::translate("element-editor", "Dynamic Library") + " (*.dll)",
        QCoreApplication::translate("element-editor", "Shared Object") + " (*.so)",

    };
    std::filesystem::path starting_path = project::project_bin_path / plugins::platforms[plugins_platforms->currentIndex()];
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilters(filters);
    dialog.setDirectory(starting_path.generic_string().c_str());
    dialog.setWindowTitle(QCoreApplication::translate("element-editor", "Select plugin"));
    if (dialog.exec()) {
        auto files = dialog.selectedFiles();
        if (!files.isEmpty()) {
            std::filesystem::path plugin_path = files.first().toStdString();
            std::filesystem::path relative_path = std::filesystem::relative(plugin_path, starting_path);
            for (const auto& part : relative_path) {
                if (part == "..") {
                    QMessageBox::warning(this, QCoreApplication::translate("element-editor", "Invalid plugin"), QCoreApplication::translate("element-editor", "Plugin must be in the same directory or a subdirectory of the platform directory"));
                    return;
                }
            }
            QString plugin_name = relative_path.generic_string().c_str();
            QStringListModel* model = plugin_list_models[plugins_platforms->currentIndex()];
            for (int i = 0; i < model->rowCount(); i++) {
                if (model->data(model->index(i)).toString() == plugin_name) {
                    QMessageBox::warning(this, QCoreApplication::translate("element-editor", "Invalid plugin"), QCoreApplication::translate("element-editor", "Plugin already exists"));
                    return;
                }
            }
            model->insertRow(model->rowCount());
            model->setData(model->index(model->rowCount() - 1), plugin_name);
            handle_apply_activation();
            plugins_modified = true;
        }
    }
}

void project_properties::remove_plugin() {
    QModelIndex index = plugins_list->selectionModel()->currentIndex();
    if (index.isValid()) {
        plugin_list_models[plugins_platforms->currentIndex()]->removeRow(index.row());
        handle_apply_activation();
        plugins_modified = true;
    }
}

void project_properties::move_plugin_up() {
    QModelIndex index = plugins_list->selectionModel()->currentIndex();
    if (index.isValid() && index.row() > 0) {
        QStringListModel* model = plugin_list_models[plugins_platforms->currentIndex()];
        model->moveRow(index.parent(), index.row(), index.parent(), index.row() - 1);
        plugins_modified = true;
    }
}

void project_properties::move_plugin_down() {
    QModelIndex index = plugins_list->selectionModel()->currentIndex();
    if (index.isValid() && index.row() < plugin_list_models[plugins_platforms->currentIndex()]->rowCount() - 1) {
        QStringListModel* model = plugin_list_models[plugins_platforms->currentIndex()];
        model->moveRow(index.parent(), index.row(), index.parent(), index.row() + 2);
        plugins_modified = true;
    }
}
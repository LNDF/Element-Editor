#include "project_properties.h"

#include <editor/project.h>
#include <QPushButton>

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
    handle_inputs_empty();
    buttons->button(QDialogButtonBox::Apply)->setEnabled(false);
    connect(buttons, SIGNAL(clicked(QAbstractButton*)), this, SLOT(handle_click(QAbstractButton*)), Qt::ConnectionType::DirectConnection);
    connect(startup_scene_input, SIGNAL(value_changed(uuid)), this, SLOT(handle_inputs_empty()), Qt::ConnectionType::DirectConnection);
    for (auto input : required_inputs) {
        connect(input, SIGNAL(textChanged(const QString&)), this, SLOT(handle_inputs_empty()), Qt::ConnectionType::DirectConnection);
    }
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
    project::save();
}

void project_properties::disable_apply() {
    buttons->button(QDialogButtonBox::Ok)->setEnabled(false);
    buttons->button(QDialogButtonBox::Apply)->setEnabled(false);
}

void project_properties::enable_apply() {
    buttons->button(QDialogButtonBox::Ok)->setEnabled(true);
    if (!new_project) buttons->button(QDialogButtonBox::Apply)->setEnabled(true);
}

void project_properties::handle_inputs_empty() {
    for (auto input : required_inputs) {
        if (input->text().isEmpty()) {
            disable_apply();
            return;
        }
    }
    enable_apply();
}
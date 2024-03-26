#include "project_properties.h"

#include <editor/project.h>
#include <QPushButton>

using namespace element::ui;

project_properties::project_properties(QWidget* parent) : QDialog(parent) {
    setupUi(this);
    setFixedSize(minimumSize());
    setWindowFlags(Qt::Sheet | Qt::Dialog);
    new_project = !project::exists();
    required_inputs.push_back(project_name_edit);
    required_inputs.push_back(author_edit);
    required_inputs.push_back(version_edit);
    project_name_edit->setText(project::name.c_str());
    version_edit->setText(project::version.c_str());
    author_edit->setText(project::author.c_str());
    handle_inputs_empty();
    buttons->button(QDialogButtonBox::Apply)->setEnabled(false);
    connect(buttons, SIGNAL(clicked(QAbstractButton*)), this, SLOT(handle_click(QAbstractButton*)), Qt::ConnectionType::DirectConnection);
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
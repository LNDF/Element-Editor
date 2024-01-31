#include "properties_standard_form.h"

using namespace element::ui;

properties_standard_form::properties_standard_form(QWidget* parent) : QGroupBox(parent) {
    QSizePolicy size_policy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    size_policy.setVerticalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    setFlat(true);
    properties_standard_form_layout = new QFormLayout(this);
    properties_standard_form_layout->setObjectName("properties_standard_form_layout");
    properties_standard_form_layout->setSizeConstraint(QLayout::SetDefaultConstraint);
    properties_standard_form_layout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    properties_standard_form_layout->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
}

void properties_standard_form::add_property(const QString& name, QWidget* widget) {
    QLabel* label = new QLabel(name, this);
    properties_standard_form_layout->addRow(label, widget);
}
#include "array_input.h"

#include <QLabel>

using namespace element::ui;

array_input::array_input(std::uint32_t count, factory_type factory, QWidget* parent) : QWidget(parent) {
    QFormLayout* array_layout = new QFormLayout(this);
    array_layout->setObjectName("array_layout");
    array_layout->setSizeConstraint(QLayout::SetDefaultConstraint);
    array_layout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    array_layout->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
    widgets.resize(count);
    for (std::uint32_t i = 0; i < count; ++i) {
        widgets[i] = factory(this);
        array_layout->addRow(new QLabel(QString::number(i), this), widgets[i]);
    }
}

array2d_input::array2d_input(std::uint32_t count1, std::uint32_t count2, factory_type factory, QWidget* parent) : QWidget(parent) {
    QFormLayout* array2d_layout = new QFormLayout(this);
    array2d_layout->setObjectName("array2d_layout");
    array2d_layout->setSizeConstraint(QLayout::SetDefaultConstraint);
    array2d_layout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    array2d_layout->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
    widgets.resize(count1 * count2);
    for (std::uint32_t i = 0; i < count1; ++i) {
        QWidget* subarray = new QWidget(this);
        QFormLayout* subarray_layout = new QFormLayout(subarray);
        subarray_layout->setObjectName("subarray_layout");
        subarray_layout->setSizeConstraint(QLayout::SetDefaultConstraint);
        subarray_layout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        subarray_layout->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        for (std::uint32_t j = 0; j < count2; ++j) {
            widgets[(i * count2) + j] = factory(subarray);
            subarray_layout->addRow(new QLabel(QString::number(j), this), widgets[(i * count2) + j]);
        }
        array2d_layout->addRow(new QLabel(QString::number(i), this), subarray);
    }
}
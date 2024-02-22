#include "numeric_input.h"

#include <limits>

using namespace element::ui;

integer_input::integer_input(QWidget* parent) : QSpinBox(parent) {
    QSizePolicy size_policy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    setMinimumSize(QSize(0, 0));
    setMinimum(std::numeric_limits<int>::min());
    setMaximum(std::numeric_limits<int>::max());
}

QString integer_input::textFromValue(int value) const { //This is a hack to allow minimum size to be 0
    if (value == std::numeric_limits<int>::min() || value == std::numeric_limits<int>::max()) {
        return "";
    }
    return QSpinBox::textFromValue(value);
}

uinteger_input::uinteger_input(QWidget* parent) : QULongSpinBox(parent) {
    QSizePolicy size_policy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    setMinimumSize(QSize(0, 0));
    setMinimum(std::numeric_limits<unsigned int>::min());
    setMaximum(std::numeric_limits<unsigned int>::max());
}

QString uinteger_input::textFromValue(quint32 value) const { //This is a hack to allow minimum size to be 0
    if (value == std::numeric_limits<unsigned int>::max()) {
        return "";
    }
    return QULongSpinBox::textFromValue(value);
}

float_input::float_input(QWidget* parent) : QDoubleSpinBox(parent) {
    QSizePolicy size_policy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    setMinimumSize(QSize(0, 0));
    setMinimum(-std::numeric_limits<float>::max());
    setMaximum(std::numeric_limits<float>::max());
    setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
}

QString float_input::textFromValue(double value) const { //This is a hack to allow minimum size to be 0
    if (value == -std::numeric_limits<float>::max() || value == std::numeric_limits<float>::max()) {
        return "";
    }
    return QDoubleSpinBox::textFromValue(value);
}

double_input::double_input(QWidget* parent) : QDoubleSpinBox(parent) {
    QSizePolicy size_policy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    setMinimumSize(QSize(0, 0));
    setMinimum(-std::numeric_limits<double>::max()); // Adjusted for double
    setMaximum(std::numeric_limits<double>::max());
    setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
}

QString double_input::textFromValue(double value) const { //This is a hack to allow minimum size to be 0
    if (value == -std::numeric_limits<double>::max() || value == std::numeric_limits<double>::max()) {
        return "";
    }
    return QDoubleSpinBox::textFromValue(value);
}
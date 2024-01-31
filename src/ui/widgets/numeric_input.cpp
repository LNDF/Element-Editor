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
    setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
}

QString integer_input::textFromValue(int value) const { //This is a hack to allow minimum size to be 0
    if (value == std::numeric_limits<int>::min() || value == std::numeric_limits<int>::max()) {
        return "";
    }
    return QSpinBox::textFromValue(value);
}

real_input::real_input(QWidget* parent) : QDoubleSpinBox(parent) {
    QSizePolicy size_policy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    setMinimumSize(QSize(0, 0));
    setMinimum(std::numeric_limits<int>::min());
    setMaximum(std::numeric_limits<int>::max());
    setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
}

QString real_input::textFromValue(double value) const { //This is a hack to allow minimum size to be 0
    if (value == std::numeric_limits<int>::min() || value == std::numeric_limits<int>::max()) {
        return "";
    }
    return QDoubleSpinBox::textFromValue(value);
}
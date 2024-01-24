#include "numeric_input.h"

#include <limits>

using namespace element::ui;

integer_input::integer_input(QWidget* parent) : QSpinBox(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    setMinimum(std::numeric_limits<int>::min());
    setMaximum(std::numeric_limits<int>::max());
    setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
}

double_input::double_input(QWidget* parent) : QDoubleSpinBox(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    setMinimum(std::numeric_limits<double>::min());
    setMaximum(std::numeric_limits<double>::max());
    setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
}
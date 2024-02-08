#include "matrix_input.h"

using namespace element::ui;

static const char* input_names[16] = {"11_input", "12_input", "13_input", "14_input",
                                      "21_input", "22_input", "23_input", "24_input",
                                      "31_input", "32_input", "33_input", "34_input",
                                      "41_input", "42_input", "43_input", "44_input"
                                     };

static void add_inputs(std::uint32_t i, std::uint32_t j, real_input** inputs, QWidget* parent, QGridLayout* layout) {
    for (std::uint16_t x = 0; x < i; ++x) {
        for (std::uint32_t y = 0; y < j; ++y)  {
            std::uint32_t index = (x * i) + y;
            inputs[index] = new real_input(parent);
            inputs[index]->setObjectName(input_names[index]);
            layout->addWidget(inputs[index], y, x);
            QObject::connect(inputs[index], SIGNAL(valueChanged(double)), parent, SLOT(change_component()), Qt::ConnectionType::DirectConnection);
        }
    }
}

mat2_input::mat2_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    mat2_input_layout = new QGridLayout(this);
    mat2_input_layout->setObjectName("mat2_input_layout");
    mat2_input_layout->setSpacing(1);
    mat2_input_layout->setContentsMargins(0, 0, 0, 0);
    add_inputs(2, 2, inputs, this, mat2_input_layout);
}

float mat2_input::value(std::uint32_t i, std::uint32_t j) const {
    return inputs[((i * 2) + j)]->value();
}

glm::mat2 mat2_input::value() const {
    return glm::mat2(inputs[0]->value(), inputs[1]->value(),
                     inputs[2]->value(), inputs[3]->value());
}

void mat2_input::set_value(std::uint32_t i, std::uint32_t j, float v) {
    inputs[((i * 2) + j)]->setValue(v);
}

void mat2_input::set_value(glm::mat2 v) {
    inputs[0]->setValue(v[0][0]);
    inputs[1]->setValue(v[0][1]);
    inputs[2]->setValue(v[1][0]);
    inputs[3]->setValue(v[1][1]);
}

void mat2_input::change_value() {
    emit value_changed(value());
}
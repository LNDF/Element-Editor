#include "vector_input.h"

using namespace element::ui;

static const char* label_texts[4] = {"X:", "Y:", "Z:", "W:"};
static const char* label_names[4] = {"x_label", "y_label", "z_label", "w_label"};
static const char* input_names[4] = {"x_input", "y_input", "z_input", "w_input"};

static void add_labels(std::uint32_t count, QLabel** labels, double_input** inputs, QWidget* parent, QHBoxLayout* layout) {
    for (std::uint32_t i = 0; i < count; ++i) {
        labels[i] = new QLabel(parent);
        labels[i]->setObjectName(label_names[i]);
        labels[i]->setText(label_texts[i]);
        layout->addWidget(labels[i]);
        inputs[i] = new double_input(parent);
        inputs[i]->setObjectName(input_names[i]);
        layout->addWidget(inputs[i]);
        QObject::connect(inputs[i], SIGNAL(valueChanged(double)), parent, SLOT(change_component()), Qt::ConnectionType::DirectConnection);
    }
}

vec2_input::vec2_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    setMinimumSize(QSize(0, 0));
    vec2_input_layout = new QHBoxLayout(this);
    vec2_input_layout->setObjectName("vec2_input_layout");
    vec2_input_layout->setSpacing(0);
    vec2_input_layout->setContentsMargins(0, 0, 0, 0);
    add_labels(2, labels, inputs, this, vec2_input_layout);
}

float vec2_input::value(std::uint32_t i) {
    return inputs[i]->value();
}

glm::vec2 vec2_input::value() {
    return glm::vec2(inputs[0]->value(), inputs[1]->value());
}

void vec2_input::set_value(std::uint32_t i, float v) {
    inputs[i]->setValue(v);
}

void vec2_input::set_value(glm::vec2 v) {
    inputs[0]->setValue(v.x);
    inputs[1]->setValue(v.y);
}

void vec2_input::change_component() {
    emit value_changed(value());
}

vec3_input::vec3_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    setMinimumSize(QSize(0, 0));
    vec3_input_layout = new QHBoxLayout(this);
    vec3_input_layout->setObjectName("vec3_input_layout");
    vec3_input_layout->setSpacing(0);
    vec3_input_layout->setContentsMargins(0, 0, 0, 0);
    add_labels(3, labels, inputs, this, vec3_input_layout);
}

float vec3_input::value(std::uint32_t i) {
    return inputs[i]->value();
}

glm::vec3 vec3_input::value() {
    return glm::vec3(inputs[0]->value(), inputs[1]->value(), inputs[2]->value());
}

void vec3_input::set_value(std::uint32_t i, float v) {
    inputs[i]->setValue(v);
}

void vec3_input::set_value(glm::vec3 v) {
    inputs[0]->setValue(v.x);
    inputs[1]->setValue(v.y);
    inputs[2]->setValue(v.z);
}

void vec3_input::change_component() {
    emit value_changed(value());
}

vec4_input::vec4_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    setMinimumSize(QSize(0, 0));
    vec4_input_layout = new QHBoxLayout(this);
    vec4_input_layout->setObjectName("vec4_input_layout");
    vec4_input_layout->setSpacing(0);
    vec4_input_layout->setContentsMargins(0, 0, 0, 0);
    add_labels(4, labels, inputs, this, vec4_input_layout);
}

float vec4_input::value(std::uint32_t i) {
    return inputs[i]->value();
}

glm::vec4 vec4_input::value() {
    return glm::vec4(inputs[0]->value(), inputs[1]->value(), inputs[2]->value(), inputs[3]->value());
}

void vec4_input::set_value(std::uint32_t i, float v) {
    inputs[i]->setValue(v);
}

void vec4_input::set_value(glm::vec4 v) {
    inputs[0]->setValue(v.x);
    inputs[1]->setValue(v.y);
    inputs[2]->setValue(v.z);
    inputs[3]->setValue(v.w);
}

void vec4_input::change_component() {
    emit value_changed(value());
}
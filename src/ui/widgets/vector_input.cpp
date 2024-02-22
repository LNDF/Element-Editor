#include "vector_input.h"

using namespace element::ui;

static const char* label_texts[4] = {"X:", "Y:", "Z:", "W:"};
static const char* label_names[4] = {"x_label", "y_label", "z_label", "w_label"};
static const char* input_names[4] = {"x_input", "y_input", "z_input", "w_input"};

static void add_labels(std::uint32_t count, QLabel** labels, float_input** inputs, QWidget* parent, QHBoxLayout* layout) {
    for (std::uint32_t i = 0; i < count; ++i) {
        labels[i] = new QLabel(parent);
        labels[i]->setObjectName(label_names[i]);
        labels[i]->setText(label_texts[i]);
        layout->addWidget(labels[i]);
        inputs[i] = new float_input(parent);
        inputs[i]->setObjectName(input_names[i]);
        layout->addWidget(inputs[i]);
        QObject::connect(inputs[i], SIGNAL(valueChanged(double)), parent, SLOT(change_component()), Qt::ConnectionType::DirectConnection);
    }
}

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

static void add_labels(std::uint32_t count, QLabel** labels, integer_input** inputs, QWidget* parent, QHBoxLayout* layout) {
    for (std::uint32_t i = 0; i < count; ++i) {
        labels[i] = new QLabel(parent);
        labels[i]->setObjectName(label_names[i]);
        labels[i]->setText(label_texts[i]);
        layout->addWidget(labels[i]);
        inputs[i] = new integer_input(parent);
        inputs[i]->setObjectName(input_names[i]);
        layout->addWidget(inputs[i]);
        QObject::connect(inputs[i], SIGNAL(valueChanged(int)), parent, SLOT(change_component()), Qt::ConnectionType::DirectConnection);
    }
}

static void add_labels(std::uint32_t count, QLabel** labels, uinteger_input** inputs, QWidget* parent, QHBoxLayout* layout) {
    for (std::uint32_t i = 0; i < count; ++i) {
        labels[i] = new QLabel(parent);
        labels[i]->setObjectName(label_names[i]);
        labels[i]->setText(label_texts[i]);
        layout->addWidget(labels[i]);
        inputs[i] = new uinteger_input(parent);
        inputs[i]->setObjectName(input_names[i]);
        layout->addWidget(inputs[i]);
        QObject::connect(inputs[i], SIGNAL(valueChanged(int)), parent, SLOT(change_component()), Qt::ConnectionType::DirectConnection);
    }
}

vec2_input::vec2_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    vec2_input_layout = new QHBoxLayout(this);
    vec2_input_layout->setObjectName("vec2_input_layout");
    vec2_input_layout->setSpacing(0);
    vec2_input_layout->setContentsMargins(0, 0, 0, 0);
    add_labels(2, labels, inputs, this, vec2_input_layout);
}

float vec2_input::value(std::uint32_t i) const {
    return inputs[i]->value();
}

glm::vec2 vec2_input::value() const {
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

float vec3_input::value(std::uint32_t i) const {
    return inputs[i]->value();
}

glm::vec3 vec3_input::value() const {
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

float vec4_input::value(std::uint32_t i) const {
    return inputs[i]->value();
}

glm::vec4 vec4_input::value() const {
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

dvec2_input::dvec2_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    dvec2_input_layout = new QHBoxLayout(this);
    dvec2_input_layout->setObjectName("dvec2_input_layout");
    dvec2_input_layout->setSpacing(0);
    dvec2_input_layout->setContentsMargins(0, 0, 0, 0);
    add_labels(2, labels, inputs, this, dvec2_input_layout);
}

double dvec2_input::value(std::uint32_t i) const {
    return inputs[i]->value();
}

glm::dvec2 dvec2_input::value() const {
    return glm::dvec2(inputs[0]->value(), inputs[1]->value());
}

void dvec2_input::set_value(std::uint32_t i, double v) {
    inputs[i]->setValue(v);
}

void dvec2_input::set_value(glm::dvec2 v) {
    inputs[0]->setValue(v.x);
    inputs[1]->setValue(v.y);
}

void dvec2_input::change_component() {
    emit value_changed(value());
}

dvec3_input::dvec3_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    dvec3_input_layout = new QHBoxLayout(this);
    dvec3_input_layout->setObjectName("dvec3_input_layout");
    dvec3_input_layout->setSpacing(0);
    dvec3_input_layout->setContentsMargins(0, 0, 0, 0);
    add_labels(3, labels, inputs, this, dvec3_input_layout);
}

double dvec3_input::value(std::uint32_t i) const {
    return inputs[i]->value();
}

glm::dvec3 dvec3_input::value() const {
    return glm::dvec3(inputs[0]->value(), inputs[1]->value(), inputs[2]->value());
}

void dvec3_input::set_value(std::uint32_t i, double v) {
    inputs[i]->setValue(v);
}

void dvec3_input::set_value(glm::dvec3 v) {
    inputs[0]->setValue(v.x);
    inputs[1]->setValue(v.y);
    inputs[2]->setValue(v.z);
}

void dvec3_input::change_component() {
    emit value_changed(value());
}

dvec4_input::dvec4_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    dvec4_input_layout = new QHBoxLayout(this);
    dvec4_input_layout->setObjectName("dvec4_input_layout");
    dvec4_input_layout->setSpacing(0);
    dvec4_input_layout->setContentsMargins(0, 0, 0, 0);
    add_labels(4, labels, inputs, this, dvec4_input_layout);
}

double dvec4_input::value(std::uint32_t i) const {
    return inputs[i]->value();
}

glm::dvec4 dvec4_input::value() const {
    return glm::dvec4(inputs[0]->value(), inputs[1]->value(), inputs[2]->value(), inputs[3]->value());
}

void dvec4_input::set_value(std::uint32_t i, double v) {
    inputs[i]->setValue(v);
}

void dvec4_input::set_value(glm::dvec4 v) {
    inputs[0]->setValue(v.x);
    inputs[1]->setValue(v.y);
    inputs[2]->setValue(v.z);
    inputs[3]->setValue(v.w);
}

void dvec4_input::change_component() {
    emit value_changed(value());
}

ivec2_input::ivec2_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    ivec2_input_layout = new QHBoxLayout(this);
    ivec2_input_layout->setObjectName("ivec2_input_layout");
    ivec2_input_layout->setSpacing(0);
    ivec2_input_layout->setContentsMargins(0, 0, 0, 0);
    add_labels(2, labels, inputs, this, ivec2_input_layout);
}

int ivec2_input::value(std::uint32_t i) const {
    return inputs[i]->value();
}

glm::ivec2 ivec2_input::value() const {
    return glm::ivec2(inputs[0]->value(), inputs[1]->value());
}

void ivec2_input::set_value(std::uint32_t i, int v) {
    inputs[i]->setValue(v);
}

void ivec2_input::set_value(glm::ivec2 v) {
    inputs[0]->setValue(v.x);
    inputs[1]->setValue(v.y);
}

void ivec2_input::change_component() {
    emit value_changed(value());
}

ivec3_input::ivec3_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    ivec3_input_layout = new QHBoxLayout(this);
    ivec3_input_layout->setObjectName("ivec3_input_layout");
    ivec3_input_layout->setSpacing(0);
    ivec3_input_layout->setContentsMargins(0, 0, 0, 0);
    add_labels(3, labels, inputs, this, ivec3_input_layout);
}

int ivec3_input::value(std::uint32_t i) const {
    return inputs[i]->value();
}

glm::ivec3 ivec3_input::value() const {
    return glm::ivec3(inputs[0]->value(), inputs[1]->value(), inputs[2]->value());
}

void ivec3_input::set_value(std::uint32_t i, int v) {
    inputs[i]->setValue(v);
}

void ivec3_input::set_value(glm::ivec3 v) {
    inputs[0]->setValue(v.x);
    inputs[1]->setValue(v.y);
    inputs[2]->setValue(v.z);
}

void ivec3_input::change_component() {
    emit value_changed(value());
}

ivec4_input::ivec4_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    ivec4_input_layout = new QHBoxLayout(this);
    ivec4_input_layout->setObjectName("ivec4_input_layout");
    ivec4_input_layout->setSpacing(0);
    ivec4_input_layout->setContentsMargins(0, 0, 0, 0);
    add_labels(4, labels, inputs, this, ivec4_input_layout);
}

int ivec4_input::value(std::uint32_t i) const {
    return inputs[i]->value();
}

glm::ivec4 ivec4_input::value() const {
    return glm::ivec4(inputs[0]->value(), inputs[1]->value(), inputs[2]->value(), inputs[3]->value());
}

void ivec4_input::set_value(std::uint32_t i, int v) {
    inputs[i]->setValue(v);
}

void ivec4_input::set_value(glm::ivec4 v) {
    inputs[0]->setValue(v.x);
    inputs[1]->setValue(v.y);
    inputs[2]->setValue(v.z);
    inputs[3]->setValue(v.w);
}

void ivec4_input::change_component() {
    emit value_changed(value());
}

uvec2_input::uvec2_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    uvec2_input_layout = new QHBoxLayout(this);
    uvec2_input_layout->setObjectName("uvec2_input_layout");
    uvec2_input_layout->setSpacing(0);
    uvec2_input_layout->setContentsMargins(0, 0, 0, 0);
    add_labels(2, labels, inputs, this, uvec2_input_layout);
}

unsigned int uvec2_input::value(std::uint32_t i) const {
    return inputs[i]->value();
}

glm::uvec2 uvec2_input::value() const {
    return glm::uvec2(inputs[0]->value(), inputs[1]->value());
}

void uvec2_input::set_value(std::uint32_t i, unsigned int v) {
    inputs[i]->setValue(v);
}

void uvec2_input::set_value(glm::uvec2 v) {
    inputs[0]->setValue(v.x);
    inputs[1]->setValue(v.y);
}

void uvec2_input::change_component() {
    emit value_changed(value());
}

uvec3_input::uvec3_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    uvec3_input_layout = new QHBoxLayout(this);
    uvec3_input_layout->setObjectName("uvec3_input_layout");
    uvec3_input_layout->setSpacing(0);
    uvec3_input_layout->setContentsMargins(0, 0, 0, 0);
    add_labels(3, labels, inputs, this, uvec3_input_layout);
}

unsigned int uvec3_input::value(std::uint32_t i) const {
    return inputs[i]->value();
}

glm::uvec3 uvec3_input::value() const {
    return glm::uvec3(inputs[0]->value(), inputs[1]->value(), inputs[2]->value());
}

void uvec3_input::set_value(std::uint32_t i, unsigned int v) {
    inputs[i]->setValue(v);
}

void uvec3_input::set_value(glm::uvec3 v) {
    inputs[0]->setValue(v.x);
    inputs[1]->setValue(v.y);
    inputs[2]->setValue(v.z);
}

void uvec3_input::change_component() {
    emit value_changed(value());
}

uvec4_input::uvec4_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    uvec4_input_layout = new QHBoxLayout(this);
    uvec4_input_layout->setObjectName("uvec4_input_layout");
    uvec4_input_layout->setSpacing(0);
    uvec4_input_layout->setContentsMargins(0, 0, 0, 0);
    add_labels(4, labels, inputs, this, uvec4_input_layout);
}

unsigned int uvec4_input::value(std::uint32_t i) const {
    return inputs[i]->value();
}

glm::uvec4 uvec4_input::value() const {
    return glm::uvec4(inputs[0]->value(), inputs[1]->value(), inputs[2]->value(), inputs[3]->value());
}

void uvec4_input::set_value(std::uint32_t i, unsigned int v) {
    inputs[i]->setValue(v);
}

void uvec4_input::set_value(glm::uvec4 v) {
    inputs[0]->setValue(v.x);
    inputs[1]->setValue(v.y);
    inputs[2]->setValue(v.z);
    inputs[3]->setValue(v.w);
}

void uvec4_input::change_component() {
    emit value_changed(value());
}
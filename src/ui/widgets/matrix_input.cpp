#include "matrix_input.h"

using namespace element::ui;

static const char* input_names[16] = {"11_input", "12_input", "13_input", "14_input",
                                      "21_input", "22_input", "23_input", "24_input",
                                      "31_input", "32_input", "33_input", "34_input",
                                      "41_input", "42_input", "43_input", "44_input"
                                     };

static void add_inputs(std::uint32_t i, std::uint32_t j, float_input** inputs, QWidget* parent, QGridLayout* layout) {
    for (std::uint16_t x = 0; x < i; ++x) {
        for (std::uint32_t y = 0; y < j; ++y)  {
            std::uint32_t index = (x * i) + y;
            inputs[index] = new float_input(parent);
            inputs[index]->setObjectName(input_names[index]);
            layout->addWidget(inputs[index], y, x);
            QObject::connect(inputs[index], SIGNAL(valueChanged(double)), parent, SLOT(change_component()), Qt::ConnectionType::DirectConnection);
        }
    }
}

static void add_inputs(std::uint32_t i, std::uint32_t j, double_input** inputs, QWidget* parent, QGridLayout* layout) {
    for (std::uint16_t x = 0; x < i; ++x) {
        for (std::uint32_t y = 0; y < j; ++y)  {
            std::uint32_t index = (x * i) + y;
            inputs[index] = new double_input(parent);
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
    return glm::mat2(inputs[0]->value(), inputs[2]->value(),
                     inputs[1]->value(), inputs[3]->value());
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

mat23_input::mat23_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    mat23_input_layout = new QGridLayout(this);
    mat23_input_layout->setObjectName("mat23_input_layout");
    mat23_input_layout->setSpacing(1);
    mat23_input_layout->setContentsMargins(0, 0, 0, 0);
    add_inputs(2, 3, inputs, this, mat23_input_layout);
}

float mat23_input::value(std::uint32_t i, std::uint32_t j) const {
    return inputs[((i * 3) + j)]->value();
}

glm::mat2x3 mat23_input::value() const {
    return glm::mat2x3(inputs[0]->value(), inputs[3]->value(),
                       inputs[1]->value(), inputs[4]->value(),
                       inputs[2]->value(), inputs[5]->value()
                    );
}

void mat23_input::set_value(std::uint32_t i, std::uint32_t j, float v) {
    inputs[((i * 3) + j)]->setValue(v);
}

void mat23_input::set_value(glm::mat2x3 v) {
    inputs[0]->setValue(v[0][0]);
    inputs[1]->setValue(v[0][1]);
    inputs[2]->setValue(v[0][2]);
    inputs[3]->setValue(v[1][0]);
    inputs[4]->setValue(v[1][1]);
    inputs[5]->setValue(v[1][2]);
}

void mat23_input::change_value() {
    emit value_changed(value());
}

mat24_input::mat24_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    mat24_input_layout = new QGridLayout(this);
    mat24_input_layout->setObjectName("mat24_input_layout");
    mat24_input_layout->setSpacing(1);
    mat24_input_layout->setContentsMargins(0, 0, 0, 0);
    add_inputs(2, 4, inputs, this, mat24_input_layout);
}

float mat24_input::value(std::uint32_t i, std::uint32_t j) const {
    return inputs[((i * 4) + j)]->value();
}

glm::mat2x4 mat24_input::value() const {
    return glm::mat2x4(inputs[0]->value(), inputs[4]->value(),
                       inputs[1]->value(), inputs[5]->value(),
                       inputs[2]->value(), inputs[6]->value(),
                       inputs[3]->value(), inputs[7]->value()
                    );
}

void mat24_input::set_value(std::uint32_t i, std::uint32_t j, float v) {
    inputs[((i * 4) + j)]->setValue(v);
}

void mat24_input::set_value(glm::mat2x4 v) {
    inputs[0]->setValue(v[0][0]);
    inputs[1]->setValue(v[0][1]);
    inputs[2]->setValue(v[0][2]);
    inputs[3]->setValue(v[0][3]);
    inputs[4]->setValue(v[1][0]);
    inputs[5]->setValue(v[1][1]);
    inputs[6]->setValue(v[1][2]);
    inputs[7]->setValue(v[1][3]);
}

void mat24_input::change_value() {
    emit value_changed(value());
}

mat32_input::mat32_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    mat32_input_layout = new QGridLayout(this);
    mat32_input_layout->setObjectName("mat32_input_layout");
    mat32_input_layout->setSpacing(1);
    mat32_input_layout->setContentsMargins(0, 0, 0, 0);
    add_inputs(3, 2, inputs, this, mat32_input_layout);
}

float mat32_input::value(std::uint32_t i, std::uint32_t j) const {
    return inputs[((i * 2) + j)]->value();
}

glm::mat3x2 mat32_input::value() const {
    return glm::mat3x2(inputs[0]->value(), inputs[2]->value(), inputs[4]->value(),
                       inputs[1]->value(), inputs[3]->value(), inputs[5]->value()
                    );
}

void mat32_input::set_value(std::uint32_t i, std::uint32_t j, float v) {
    inputs[((i * 2) + j)]->setValue(v);
}

void mat32_input::set_value(glm::mat3x2 v) {
    inputs[0]->setValue(v[0][0]);
    inputs[1]->setValue(v[0][1]);
    inputs[2]->setValue(v[1][0]);
    inputs[3]->setValue(v[1][1]);
    inputs[4]->setValue(v[2][0]);
    inputs[5]->setValue(v[2][1]);
}

void mat32_input::change_value() {
    emit value_changed(value());
}

mat3_input::mat3_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    mat3_input_layout = new QGridLayout(this);
    mat3_input_layout->setObjectName("mat3_input_layout");
    mat3_input_layout->setSpacing(1);
    mat3_input_layout->setContentsMargins(0, 0, 0, 0);
    add_inputs(3, 3, inputs, this, mat3_input_layout);
}

float mat3_input::value(std::uint32_t i, std::uint32_t j) const {
    return inputs[((i * 3) + j)]->value();
}

glm::mat3 mat3_input::value() const {
    return glm::mat3(inputs[0]->value(), inputs[3]->value(), inputs[6]->value(),
                     inputs[1]->value(), inputs[4]->value(), inputs[7]->value(),
                     inputs[2]->value(), inputs[5]->value(), inputs[8]->value()
                    );
}

void mat3_input::set_value(std::uint32_t i, std::uint32_t j, float v) {
    inputs[((i * 3) + j)]->setValue(v);
}

void mat3_input::set_value(glm::mat3 v) {
    inputs[0]->setValue(v[0][0]);
    inputs[1]->setValue(v[0][1]);
    inputs[2]->setValue(v[0][2]);
    inputs[3]->setValue(v[1][0]);
    inputs[4]->setValue(v[1][1]);
    inputs[5]->setValue(v[1][2]);
    inputs[6]->setValue(v[2][0]);
    inputs[7]->setValue(v[2][1]);
    inputs[8]->setValue(v[2][2]);
}

void mat3_input::change_value() {
    emit value_changed(value());
}

mat34_input::mat34_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    mat34_input_layout = new QGridLayout(this);
    mat34_input_layout->setObjectName("mat34_input_layout");
    mat34_input_layout->setSpacing(1);
    mat34_input_layout->setContentsMargins(0, 0, 0, 0);
    add_inputs(3, 4, inputs, this, mat34_input_layout);
}

float mat34_input::value(std::uint32_t i, std::uint32_t j) const {
    return inputs[((i * 4) + j)]->value();
}

glm::mat3x4 mat34_input::value() const {
    return glm::mat3x4(inputs[0]->value(), inputs[4]->value(), inputs[8]->value(),
                       inputs[1]->value(), inputs[5]->value(), inputs[9]->value(),
                       inputs[2]->value(), inputs[6]->value(), inputs[10]->value(),
                       inputs[3]->value(), inputs[7]->value(), inputs[11]->value()
                    );
}

void mat34_input::set_value(std::uint32_t i, std::uint32_t j, float v) {
    inputs[((i * 4) + j)]->setValue(v);
}

void mat34_input::set_value(glm::mat3x4 v) {
    inputs[0]->setValue(v[0][0]);
    inputs[1]->setValue(v[0][1]);
    inputs[2]->setValue(v[0][2]);
    inputs[3]->setValue(v[0][3]);
    inputs[4]->setValue(v[1][0]);
    inputs[5]->setValue(v[1][1]);
    inputs[6]->setValue(v[1][2]);
    inputs[7]->setValue(v[1][3]);
    inputs[8]->setValue(v[2][0]);
    inputs[9]->setValue(v[2][1]);
    inputs[10]->setValue(v[2][2]);
    inputs[11]->setValue(v[2][3]);
}

void mat34_input::change_value() {
    emit value_changed(value());
}

mat42_input::mat42_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    mat42_input_layout = new QGridLayout(this);
    mat42_input_layout->setObjectName("mat42_input_layout");
    mat42_input_layout->setSpacing(1);
    mat42_input_layout->setContentsMargins(0, 0, 0, 0);
    add_inputs(4, 2, inputs, this, mat42_input_layout);
}

float mat42_input::value(std::uint32_t i, std::uint32_t j) const {
    return inputs[((i * 2) + j)]->value();
}

glm::mat4x2 mat42_input::value() const {
    return glm::mat4x2(inputs[0]->value(), inputs[2]->value(), inputs[4]->value(), inputs[6]->value(),
                       inputs[1]->value(), inputs[3]->value(), inputs[5]->value(), inputs[7]->value()
                    );
}

void mat42_input::set_value(std::uint32_t i, std::uint32_t j, float v) {
    inputs[((i * 2) + j)]->setValue(v);
}

void mat42_input::set_value(glm::mat4x2 v) {
    inputs[0]->setValue(v[0][0]);
    inputs[1]->setValue(v[0][1]);
    inputs[2]->setValue(v[1][0]);
    inputs[3]->setValue(v[1][1]);
    inputs[4]->setValue(v[2][0]);
    inputs[5]->setValue(v[2][1]);
    inputs[6]->setValue(v[3][0]);
    inputs[7]->setValue(v[3][1]);
}

void mat42_input::change_value() {
    emit value_changed(value());
}

mat43_input::mat43_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    mat43_input_layout = new QGridLayout(this);
    mat43_input_layout->setObjectName("mat43_input_layout");
    mat43_input_layout->setSpacing(1);
    mat43_input_layout->setContentsMargins(0, 0, 0, 0);
    add_inputs(4, 3, inputs, this, mat43_input_layout);
}

float mat43_input::value(std::uint32_t i, std::uint32_t j) const {
    return inputs[((i * 3) + j)]->value();
}

glm::mat4x3 mat43_input::value() const {
    return glm::mat4x3(inputs[0]->value(), inputs[3]->value(), inputs[6]->value(), inputs[9]->value(),
                       inputs[1]->value(), inputs[4]->value(), inputs[7]->value(), inputs[10]->value(),
                       inputs[2]->value(), inputs[5]->value(), inputs[8]->value(), inputs[11]->value()
                    );
}

void mat43_input::set_value(std::uint32_t i, std::uint32_t j, float v) {
    inputs[((i * 3) + j)]->setValue(v);
}

void mat43_input::set_value(glm::mat4x3 v) {
    inputs[0]->setValue(v[0][0]);
    inputs[1]->setValue(v[0][1]);
    inputs[2]->setValue(v[0][2]);
    inputs[3]->setValue(v[1][0]);
    inputs[4]->setValue(v[1][1]);
    inputs[5]->setValue(v[1][2]);
    inputs[6]->setValue(v[2][0]);
    inputs[7]->setValue(v[2][1]);
    inputs[8]->setValue(v[2][2]);
    inputs[9]->setValue(v[3][0]);
    inputs[10]->setValue(v[3][1]);
    inputs[11]->setValue(v[3][2]);
}

void mat43_input::change_value() {
    emit value_changed(value());
}

mat4_input::mat4_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    mat4_input_layout = new QGridLayout(this);
    mat4_input_layout->setObjectName("mat4_input_layout");
    mat4_input_layout->setSpacing(1);
    mat4_input_layout->setContentsMargins(0, 0, 0, 0);
    add_inputs(4, 4, inputs, this, mat4_input_layout);
}

float mat4_input::value(std::uint32_t i, std::uint32_t j) const {
    return inputs[((i * 4) + j)]->value();
}

glm::mat4 mat4_input::value() const {
    return glm::mat4(inputs[0]->value(), inputs[4]->value(), inputs[8]->value(), inputs[12]->value(),
                     inputs[1]->value(), inputs[5]->value(), inputs[9]->value(), inputs[13]->value(),
                     inputs[2]->value(), inputs[6]->value(), inputs[10]->value(), inputs[14]->value(),
                     inputs[3]->value(), inputs[7]->value(), inputs[11]->value(), inputs[15]->value()
                    );
}

void mat4_input::set_value(std::uint32_t i, std::uint32_t j, float v) {
    inputs[((i * 4) + j)]->setValue(v);
}

void mat4_input::set_value(glm::mat4 v) {
    inputs[0]->setValue(v[0][0]);
    inputs[1]->setValue(v[0][1]);
    inputs[2]->setValue(v[0][2]);
    inputs[3]->setValue(v[0][3]);
    inputs[4]->setValue(v[1][0]);
    inputs[5]->setValue(v[1][1]);
    inputs[6]->setValue(v[1][2]);
    inputs[7]->setValue(v[1][3]);
    inputs[8]->setValue(v[2][0]);
    inputs[9]->setValue(v[2][1]);
    inputs[10]->setValue(v[2][2]);
    inputs[11]->setValue(v[2][3]);
    inputs[12]->setValue(v[3][0]);
    inputs[13]->setValue(v[3][1]);
    inputs[14]->setValue(v[3][2]);
    inputs[15]->setValue(v[3][3]);
}

void mat4_input::change_value() {
    emit value_changed(value());
}

dmat2_input::dmat2_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    dmat2_input_layout = new QGridLayout(this);
    dmat2_input_layout->setObjectName("dmat2_input_layout");
    dmat2_input_layout->setSpacing(1);
    dmat2_input_layout->setContentsMargins(0, 0, 0, 0);
    add_inputs(2, 2, inputs, this, dmat2_input_layout);
}

double dmat2_input::value(std::uint32_t i, std::uint32_t j) const {
    return inputs[((i * 2) + j)]->value();
}

glm::dmat2 dmat2_input::value() const {
    return glm::dmat2(inputs[0]->value(), inputs[2]->value(),
                     inputs[1]->value(), inputs[3]->value());
}

void dmat2_input::set_value(std::uint32_t i, std::uint32_t j, double v) {
    inputs[((i * 2) + j)]->setValue(v);
}

void dmat2_input::set_value(glm::dmat2 v) {
    inputs[0]->setValue(v[0][0]);
    inputs[1]->setValue(v[0][1]);
    inputs[2]->setValue(v[1][0]);
    inputs[3]->setValue(v[1][1]);
}

void dmat2_input::change_value() {
    emit value_changed(value());
}

dmat23_input::dmat23_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    dmat23_input_layout = new QGridLayout(this);
    dmat23_input_layout->setObjectName("dmat23_input_layout");
    dmat23_input_layout->setSpacing(1);
    dmat23_input_layout->setContentsMargins(0, 0, 0, 0);
    add_inputs(2, 3, inputs, this, dmat23_input_layout);
}

double dmat23_input::value(std::uint32_t i, std::uint32_t j) const {
    return inputs[((i * 3) + j)]->value();
}

glm::dmat2x3 dmat23_input::value() const {
    return glm::dmat2x3(inputs[0]->value(), inputs[3]->value(),
                       inputs[1]->value(), inputs[4]->value(),
                       inputs[2]->value(), inputs[5]->value()
                    );
}

void dmat23_input::set_value(std::uint32_t i, std::uint32_t j, double v) {
    inputs[((i * 3) + j)]->setValue(v);
}

void dmat23_input::set_value(glm::dmat2x3 v) {
    inputs[0]->setValue(v[0][0]);
    inputs[1]->setValue(v[0][1]);
    inputs[2]->setValue(v[0][2]);
    inputs[3]->setValue(v[1][0]);
    inputs[4]->setValue(v[1][1]);
    inputs[5]->setValue(v[1][2]);
}

void dmat23_input::change_value() {
    emit value_changed(value());
}

dmat24_input::dmat24_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    dmat24_input_layout = new QGridLayout(this);
    dmat24_input_layout->setObjectName("dmat24_input_layout");
    dmat24_input_layout->setSpacing(1);
    dmat24_input_layout->setContentsMargins(0, 0, 0, 0);
    add_inputs(2, 4, inputs, this, dmat24_input_layout);
}

double dmat24_input::value(std::uint32_t i, std::uint32_t j) const {
    return inputs[((i * 4) + j)]->value();
}

glm::dmat2x4 dmat24_input::value() const {
    return glm::dmat2x4(inputs[0]->value(), inputs[4]->value(),
                       inputs[1]->value(), inputs[5]->value(),
                       inputs[2]->value(), inputs[6]->value(),
                       inputs[3]->value(), inputs[7]->value()
                    );
}

void dmat24_input::set_value(std::uint32_t i, std::uint32_t j, double v) {
    inputs[((i * 4) + j)]->setValue(v);
}

void dmat24_input::set_value(glm::dmat2x4 v) {
    inputs[0]->setValue(v[0][0]);
    inputs[1]->setValue(v[0][1]);
    inputs[2]->setValue(v[0][2]);
    inputs[3]->setValue(v[0][3]);
    inputs[4]->setValue(v[1][0]);
    inputs[5]->setValue(v[1][1]);
    inputs[6]->setValue(v[1][2]);
    inputs[7]->setValue(v[1][3]);
}

void dmat24_input::change_value() {
    emit value_changed(value());
}

dmat32_input::dmat32_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    dmat32_input_layout = new QGridLayout(this);
    dmat32_input_layout->setObjectName("dmat32_input_layout");
    dmat32_input_layout->setSpacing(1);
    dmat32_input_layout->setContentsMargins(0, 0, 0, 0);
    add_inputs(3, 2, inputs, this, dmat32_input_layout);
}

double dmat32_input::value(std::uint32_t i, std::uint32_t j) const {
    return inputs[((i * 2) + j)]->value();
}

glm::dmat3x2 dmat32_input::value() const {
    return glm::dmat3x2(inputs[0]->value(), inputs[2]->value(), inputs[4]->value(),
                       inputs[1]->value(), inputs[3]->value(), inputs[5]->value()
                    );
}

void dmat32_input::set_value(std::uint32_t i, std::uint32_t j, double v) {
    inputs[((i * 2) + j)]->setValue(v);
}

void dmat32_input::set_value(glm::dmat3x2 v) {
    inputs[0]->setValue(v[0][0]);
    inputs[1]->setValue(v[0][1]);
    inputs[2]->setValue(v[1][0]);
    inputs[3]->setValue(v[1][1]);
    inputs[4]->setValue(v[2][0]);
    inputs[5]->setValue(v[2][1]);
}

void dmat32_input::change_value() {
    emit value_changed(value());
}

dmat3_input::dmat3_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    dmat3_input_layout = new QGridLayout(this);
    dmat3_input_layout->setObjectName("dmat3_input_layout");
    dmat3_input_layout->setSpacing(1);
    dmat3_input_layout->setContentsMargins(0, 0, 0, 0);
    add_inputs(3, 3, inputs, this, dmat3_input_layout);
}

double dmat3_input::value(std::uint32_t i, std::uint32_t j) const {
    return inputs[((i * 3) + j)]->value();
}

glm::dmat3 dmat3_input::value() const {
    return glm::dmat3(inputs[0]->value(), inputs[3]->value(), inputs[6]->value(),
                     inputs[1]->value(), inputs[4]->value(), inputs[7]->value(),
                     inputs[2]->value(), inputs[5]->value(), inputs[8]->value()
                    );
}

void dmat3_input::set_value(std::uint32_t i, std::uint32_t j, double v) {
    inputs[((i * 3) + j)]->setValue(v);
}

void dmat3_input::set_value(glm::dmat3 v) {
    inputs[0]->setValue(v[0][0]);
    inputs[1]->setValue(v[0][1]);
    inputs[2]->setValue(v[0][2]);
    inputs[3]->setValue(v[1][0]);
    inputs[4]->setValue(v[1][1]);
    inputs[5]->setValue(v[1][2]);
    inputs[6]->setValue(v[2][0]);
    inputs[7]->setValue(v[2][1]);
    inputs[8]->setValue(v[2][2]);
}

void dmat3_input::change_value() {
    emit value_changed(value());
}

dmat34_input::dmat34_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    dmat34_input_layout = new QGridLayout(this);
    dmat34_input_layout->setObjectName("dmat34_input_layout");
    dmat34_input_layout->setSpacing(1);
    dmat34_input_layout->setContentsMargins(0, 0, 0, 0);
    add_inputs(3, 4, inputs, this, dmat34_input_layout);
}

double dmat34_input::value(std::uint32_t i, std::uint32_t j) const {
    return inputs[((i * 4) + j)]->value();
}

glm::dmat3x4 dmat34_input::value() const {
    return glm::dmat3x4(inputs[0]->value(), inputs[4]->value(), inputs[8]->value(),
                       inputs[1]->value(), inputs[5]->value(), inputs[9]->value(),
                       inputs[2]->value(), inputs[6]->value(), inputs[10]->value(),
                       inputs[3]->value(), inputs[7]->value(), inputs[11]->value()
                    );
}

void dmat34_input::set_value(std::uint32_t i, std::uint32_t j, double v) {
    inputs[((i * 4) + j)]->setValue(v);
}

void dmat34_input::set_value(glm::dmat3x4 v) {
    inputs[0]->setValue(v[0][0]);
    inputs[1]->setValue(v[0][1]);
    inputs[2]->setValue(v[0][2]);
    inputs[3]->setValue(v[0][3]);
    inputs[4]->setValue(v[1][0]);
    inputs[5]->setValue(v[1][1]);
    inputs[6]->setValue(v[1][2]);
    inputs[7]->setValue(v[1][3]);
    inputs[8]->setValue(v[2][0]);
    inputs[9]->setValue(v[2][1]);
    inputs[10]->setValue(v[2][2]);
    inputs[11]->setValue(v[2][3]);
}

void dmat34_input::change_value() {
    emit value_changed(value());
}

dmat42_input::dmat42_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    dmat42_input_layout = new QGridLayout(this);
    dmat42_input_layout->setObjectName("dmat42_input_layout");
    dmat42_input_layout->setSpacing(1);
    dmat42_input_layout->setContentsMargins(0, 0, 0, 0);
    add_inputs(4, 2, inputs, this, dmat42_input_layout);
}

double dmat42_input::value(std::uint32_t i, std::uint32_t j) const {
    return inputs[((i * 2) + j)]->value();
}

glm::dmat4x2 dmat42_input::value() const {
    return glm::dmat4x2(inputs[0]->value(), inputs[2]->value(), inputs[4]->value(), inputs[6]->value(),
                       inputs[1]->value(), inputs[3]->value(), inputs[5]->value(), inputs[7]->value()
                    );
}

void dmat42_input::set_value(std::uint32_t i, std::uint32_t j, double v) {
    inputs[((i * 2) + j)]->setValue(v);
}

void dmat42_input::set_value(glm::dmat4x2 v) {
    inputs[0]->setValue(v[0][0]);
    inputs[1]->setValue(v[0][1]);
    inputs[2]->setValue(v[1][0]);
    inputs[3]->setValue(v[1][1]);
    inputs[4]->setValue(v[2][0]);
    inputs[5]->setValue(v[2][1]);
    inputs[6]->setValue(v[3][0]);
    inputs[7]->setValue(v[3][1]);
}

void dmat42_input::change_value() {
    emit value_changed(value());
}

dmat43_input::dmat43_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    dmat43_input_layout = new QGridLayout(this);
    dmat43_input_layout->setObjectName("dmat43_input_layout");
    dmat43_input_layout->setSpacing(1);
    dmat43_input_layout->setContentsMargins(0, 0, 0, 0);
    add_inputs(4, 3, inputs, this, dmat43_input_layout);
}

double dmat43_input::value(std::uint32_t i, std::uint32_t j) const {
    return inputs[((i * 3) + j)]->value();
}

glm::dmat4x3 dmat43_input::value() const {
    return glm::dmat4x3(inputs[0]->value(), inputs[3]->value(), inputs[6]->value(), inputs[9]->value(),
                       inputs[1]->value(), inputs[4]->value(), inputs[7]->value(), inputs[10]->value(),
                       inputs[2]->value(), inputs[5]->value(), inputs[8]->value(), inputs[11]->value()
                    );
}

void dmat43_input::set_value(std::uint32_t i, std::uint32_t j, double v) {
    inputs[((i * 3) + j)]->setValue(v);
}

void dmat43_input::set_value(glm::dmat4x3 v) {
    inputs[0]->setValue(v[0][0]);
    inputs[1]->setValue(v[0][1]);
    inputs[2]->setValue(v[0][2]);
    inputs[3]->setValue(v[1][0]);
    inputs[4]->setValue(v[1][1]);
    inputs[5]->setValue(v[1][2]);
    inputs[6]->setValue(v[2][0]);
    inputs[7]->setValue(v[2][1]);
    inputs[8]->setValue(v[2][2]);
    inputs[9]->setValue(v[3][0]);
    inputs[10]->setValue(v[3][1]);
    inputs[11]->setValue(v[3][2]);
}

void dmat43_input::change_value() {
    emit value_changed(value());
}

dmat4_input::dmat4_input(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    dmat4_input_layout = new QGridLayout(this);
    dmat4_input_layout->setObjectName("dmat4_input_layout");
    dmat4_input_layout->setSpacing(1);
    dmat4_input_layout->setContentsMargins(0, 0, 0, 0);
    add_inputs(4, 4, inputs, this, dmat4_input_layout);
}

double dmat4_input::value(std::uint32_t i, std::uint32_t j) const {
    return inputs[((i * 4) + j)]->value();
}

glm::dmat4 dmat4_input::value() const {
    return glm::dmat4(inputs[0]->value(), inputs[4]->value(), inputs[8]->value(), inputs[12]->value(),
                     inputs[1]->value(), inputs[5]->value(), inputs[9]->value(), inputs[13]->value(),
                     inputs[2]->value(), inputs[6]->value(), inputs[10]->value(), inputs[14]->value(),
                     inputs[3]->value(), inputs[7]->value(), inputs[11]->value(), inputs[15]->value()
                    );
}

void dmat4_input::set_value(std::uint32_t i, std::uint32_t j, double v) {
    inputs[((i * 4) + j)]->setValue(v);
}

void dmat4_input::set_value(glm::dmat4 v) {
    inputs[0]->setValue(v[0][0]);
    inputs[1]->setValue(v[0][1]);
    inputs[2]->setValue(v[0][2]);
    inputs[3]->setValue(v[0][3]);
    inputs[4]->setValue(v[1][0]);
    inputs[5]->setValue(v[1][1]);
    inputs[6]->setValue(v[1][2]);
    inputs[7]->setValue(v[1][3]);
    inputs[8]->setValue(v[2][0]);
    inputs[9]->setValue(v[2][1]);
    inputs[10]->setValue(v[2][2]);
    inputs[11]->setValue(v[2][3]);
    inputs[12]->setValue(v[3][0]);
    inputs[13]->setValue(v[3][1]);
    inputs[14]->setValue(v[3][2]);
    inputs[15]->setValue(v[3][3]);
}

void dmat4_input::change_value() {
    emit value_changed(value());
}
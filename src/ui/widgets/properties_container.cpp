#include "properties_container.h"

using namespace element::ui;

properties_container::properties_container(QWidget* parent) : QWidget(parent) {
    QSizePolicy size_policy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    size_policy.setHorizontalStretch(0);
    size_policy.setVerticalStretch(0);
    size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(size_policy);
    setMinimumSize(QSize(0, 0));
    properties_container_layout = new QVBoxLayout(this);
    properties_container_layout->setObjectName("properties_container_layout");
    properties_container_layout->setContentsMargins(0, 15, 0, 15);
    properties_container_layout->setSpacing(0);
}

properties_container::~properties_container() {

}

void properties_container::load_values() {

}
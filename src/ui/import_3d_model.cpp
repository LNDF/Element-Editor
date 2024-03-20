#include "import_3d_model.h"

#include <QFileDialog>

using namespace element::ui;

import_3d_model::import_3d_model(QWidget* parent) : QDialog(parent) {
    setupUi(this);
    setFixedSize(minimumSize());
    setWindowFlags(Qt::Sheet | Qt::Dialog);
}
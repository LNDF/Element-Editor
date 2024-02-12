#include "asset_chooser.h"

using namespace element::ui;

asset_chooser::asset_chooser(const std::string& type, QWidget* parent) : QDialog(parent) {
    setupUi(this);
    setFixedSize(minimumSize());
    setWindowFlags(Qt::Sheet | Qt::Dialog);
    model = new model_asset_chooser(type);
    proxy = new QSortFilterProxyModel();
    proxy->setSourceModel(model);
    assets->setModel(proxy);
    connect(unselect, SIGNAL(clicked()), this, SIGNAL(reset()), Qt::ConnectionType::DirectConnection);
}

asset_chooser::~asset_chooser() {
    delete proxy;
    delete model;
}
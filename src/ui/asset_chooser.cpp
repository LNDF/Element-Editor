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
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    connect(unselect, SIGNAL(clicked()), this, SIGNAL(reseted()), Qt::ConnectionType::DirectConnection);
    connect(this, SIGNAL(accepted()), this, SLOT(select_current()), Qt::ConnectionType::DirectConnection);
    connect(this, SIGNAL(reseted()), this, SLOT(select_null()), Qt::ConnectionType::DirectConnection);
    connect(this, SIGNAL(reseted()), this, SLOT(reject()), Qt::ConnectionType::DirectConnection);
    connect(filter, SIGNAL(textChanged(const QString&)), proxy, SLOT(setFilterFixedString(const QString&)), Qt::ConnectionType::DirectConnection);
}

asset_chooser::~asset_chooser() {
    delete proxy;
    delete model;
}

void asset_chooser::select_null() {
    emit select(uuid::null());
}

void asset_chooser::select_current() {
    emit select(model->id_from_index(proxy->mapToSource(assets->selectionModel()->currentIndex())));
}
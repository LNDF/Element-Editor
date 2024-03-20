#include "asset_chooser.h"

#include <QPushButton>

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
    buttons->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(buttons, SIGNAL(clicked(QAbstractButton*)), this, SLOT(handle_click(QAbstractButton*)), Qt::ConnectionType::DirectConnection);
    connect(filter, SIGNAL(textChanged(const QString&)), proxy, SLOT(setFilterFixedString(const QString&)), Qt::ConnectionType::DirectConnection);
    connect(assets->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(selected(const QModelIndex&)), Qt::ConnectionType::DirectConnection);
}

asset_chooser::~asset_chooser() {
    delete proxy;
    delete model;
}

void asset_chooser::handle_click(QAbstractButton* button) {
    if (button == buttons->button(QDialogButtonBox::Reset)) {
        emit select(uuid::null());
        emit accept();
    } else if (button == buttons->button(QDialogButtonBox::Ok)) {
        emit select(model->id_from_index(proxy->mapToSource(assets->selectionModel()->currentIndex())));
        emit accept();
    } else if (button == buttons->button(QDialogButtonBox::Cancel)) {
        emit reject();
    }
}

void asset_chooser::selected(const QModelIndex& index) {
    buttons->button(QDialogButtonBox::Ok)->setEnabled(index.isValid());
}
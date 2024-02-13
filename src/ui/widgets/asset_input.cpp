#include "asset_input.h"

#include <ui/models/model_assets_tree.h>
#include <ui/asset_chooser.h>
#include <core/fs.h>
#include <QCoreApplication>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QMimeData>

using namespace element;
using namespace element::ui;

asset_input::asset_input(const std::string& type, QWidget* parent) : QPushButton(parent), type(type) {
    setAcceptDrops(true);
    connect(this, SIGNAL(clicked(bool)), this, SLOT(open_asset_chooser()), Qt::ConnectionType::DirectConnection);
}

uuid asset_input::value() const {
    return asset;
}

void asset_input::dragEnterEvent(QDragEnterEvent* event) {
    if (event->mimeData()->hasFormat(model_assets_tree::get_ref_mime_type())) {
        QByteArray encoded = event->mimeData()->data(model_assets_tree::get_ref_mime_type());
        QDataStream stream(&encoded, QDataStream::ReadOnly);
        uuid id = uuid::null();
        stream.readRawData((char*) id.bytes, 16);
        const fs_resource_info& info = fs::get_resource_info(id);
        if (info.type == type) {
            event->acceptProposedAction();
        }
    }
}

void asset_input::dragMoveEvent(QDragMoveEvent* event) {
    if (event->mimeData()->hasFormat(model_assets_tree::get_ref_mime_type())) {
        QByteArray encoded = event->mimeData()->data(model_assets_tree::get_ref_mime_type());
        QDataStream stream(&encoded, QDataStream::ReadOnly);
        uuid id = uuid::null();
        stream.readRawData((char*) id.bytes, 16);
        const fs_resource_info& info = fs::get_resource_info(id);
        if (info.type == type) {
            event->acceptProposedAction();
        }
    }
}

void asset_input::dragLeaveEvent(QDragLeaveEvent* event) {
    Q_UNUSED(event);
}

void asset_input::dropEvent(QDropEvent *event) {
    if (event->mimeData()->hasFormat(model_assets_tree::get_ref_mime_type())) {
        QByteArray encoded = event->mimeData()->data(model_assets_tree::get_ref_mime_type());
        QDataStream stream(&encoded, QDataStream::ReadOnly);
        uuid id = uuid::null();
        stream.readRawData((char*) id.bytes, 16);
        const fs_resource_info& info = fs::get_resource_info(id);
        if (info.type == type) {
            set_value(id);
            event->acceptProposedAction();
        }
    }
}

void asset_input::set_value(const uuid& v) {
    asset = v;
    const fs_resource_info& info = fs::get_resource_info(v);
    if (info.path.empty()) {
        setText(QCoreApplication::translate("element-editor", "[Click or drg asset here]"));
    } else {
        std::string filename = info.path.substr(info.path.find_last_of("/") + 1);
        setText(filename.c_str());
    }
    emit value_changed(v);
}

void asset_input::open_asset_chooser() {
    asset_chooser* dialog = new asset_chooser(type, this);
    connect(dialog, SIGNAL(select(uuid)), this, SLOT(set_value(uuid)), Qt::ConnectionType::DirectConnection);
    dialog->open();
}
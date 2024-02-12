#include "model_asset_chooser.h"

using namespace element::ui;
using namespace element;

model_asset_chooser::model_asset_chooser(const std::string& type) {
    for (const auto& [id, info] : fs::get_map()) {
        if (type == info.type) assets.emplace_back(id, &info);
    }
}

int model_asset_chooser::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return assets.size();
}

QVariant model_asset_chooser::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) return QVariant();
    std::pair<uuid, const fs_resource_info*>* info = (std::pair<uuid, const fs_resource_info*>*) index.internalPointer();
    return QString(info->second->path.c_str());
}

QModelIndex model_asset_chooser::index(int row, int column, const QModelIndex &parent) const {
    if (parent.isValid() || column != 0) return QModelIndex();
    return createIndex(row, column, &assets[row]);
}

QModelIndex model_asset_chooser::parent(const QModelIndex &index) const {
    return QModelIndex();
}

const uuid& model_asset_chooser::id_from_index(const QModelIndex& index) const {
    return ((std::pair<uuid, const fs_resource_info*>*) index.internalPointer())->first;
}
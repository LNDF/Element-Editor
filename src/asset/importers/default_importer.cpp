#include "default_importer.h"

#include <asset/importer.h>
#include <core/fs_editor.h>
#include <editor/project.h>
#include <fstream>

using namespace element;

void importers::default_importer(const uuid& id) {
    const fs_resource_info& info = fs::get_resource_info(id);
    std::ifstream input(asset_importer::get_system_path_from_fs(info.path));
    auto output = fs::get_resource_ostream(id);
    *output << input.rdbuf();
}
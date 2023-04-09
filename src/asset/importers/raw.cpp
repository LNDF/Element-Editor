#include "raw.h"

#include <asset/importer.h>
#include <core/fs_editor.h>
#include <editor/project.h>
#include <fstream>

using namespace element;

void importers::raw_importer(const uuid& id) {
    fs_resource_info info = fs_get_resource_info(id);
    std::ifstream input(project::project_assets_path / info.path);
    auto output = fs_get_resource_ostream(id);
    *output << input.rdbuf();
}
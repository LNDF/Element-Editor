#include <asset/importer.h>
#include <core/fs_editor.h>
#include <editor/project.h>
#include <scenegraph/scene.h>
#include <serialization/scenegraph.h>
#include <utils/uuid.h>
#include <fstream>

using namespace element;

static void scene_importer(const uuid& id) {
    fs_resource_info info = fs::get_resource_info(id);
    std::ifstream input(project::project_assets_path / info.path);
    auto output = fs::get_resource_ostream(id);
    scenegraph::scene s;
    text_deserializer deserialize = create_text_deserializer(input);
    binary_serializer serialize = create_binary_serializer(*output);
    deserialize(s);
    serialize(s);
}

ELM_REGISTER_IMPORTER("scene", scene_importer)
#include <asset/importer.h>
#include <asset/loaders/scene.h>
#include <core/fs_editor.h>
#include <scenegraph/scene.h>
#include <serialization/defs.h>
#include <serialization/serializers.h>
#include <serialization/scenegraph.h>
#include <utils/uuid.h>
#include <fstream>

using namespace element;

static void scene_importer(const uuid& id) {
    fs_resource_info info = fs::get_resource_info(id);
    std::optional<scenegraph::scene> s = asset_loader::scene_text_load(info.path);
    auto output = fs::get_resource_ostream(id);
    binary_serializer serialize = create_binary_serializer(*output);
    serialize(s.value());
}

ELM_REGISTER_IMPORTER("scene", scene_importer, 1)
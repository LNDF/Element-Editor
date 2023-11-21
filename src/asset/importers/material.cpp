#include <asset/importer.h>
#include <asset/dependencies.h>
#include <core/fs_editor.h>
#include <editor/project.h>
#include <render/material.h>
#include <serialization/defs.h>
#include <serialization/serializers_editor.h>
#include <serialization/render/material.h>
#include <utils/uuid.h>
#include <utils/packed_set.h>
#include <fstream>

using namespace element;

static void material_importer(const uuid& id) {
    fs_resource_info info = fs::get_resource_info(id);
    std::ifstream input(project::project_assets_path / info.path);
    render::material material;
    text_deserializer deserialize = create_text_deserializer(input);
    deserialize(ELM_SERIALIZE_NVP("material", material));
    auto output = fs::get_resource_ostream(id);
    binary_serializer serialize = create_binary_serializer(*output);
    serialize(material);
    packed_set<uuid> dependencies;
    dependencies.insert(material.get_pipeline_id());
    asset_importer::set_id_dependencies(id, std::move(dependencies));
}

ELM_REGISTER_IMPORTER("material", material_importer)
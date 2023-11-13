#include <asset/importer.h>
#include <asset/dependencies.h>
#include <core/fs_editor.h>
#include <editor/project.h>
#include <render/pipeline.h>
#include <render/pipeline_process.h>
#include <serialization/defs.h>
#include <serialization/serializers_editor.h>
#include <serialization/render/pipeline.h>
#include <utils/uuid.h>
#include <utils/packed_set.h>
#include <fstream>

using namespace element;

static void pipeline_importer(const uuid& id) {
    fs_resource_info info = fs::get_resource_info(id);
    std::ifstream input(project::project_assets_path / info.path);
    render::pipeline_data data;
    text_deserializer deserialize = create_text_deserializer(input);
    deserialize(ELM_SERIALIZE_NVP("pipeline", data));
    if (render::pipeline_create_layout_data(data)) {
        auto output = fs::get_resource_ostream(id);
        binary_serializer serialize = create_binary_serializer(*output);
        serialize(data);
    } else {
        fs::delete_resource_data(id);
    }
    packed_set<uuid> dependencies;
    dependencies.insert(data.vert_id);
    dependencies.insert(data.frag_id);
    asset_importer::set_id_dependencies(id, std::move(dependencies));
}

ELM_REGISTER_IMPORTER("pipeline", pipeline_importer)
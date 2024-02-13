#include <asset/importer.h>
#include <asset/dependencies.h>
#include <asset/loaders/pipeline.h>
#include <core/fs_editor.h>
#include <render/pipeline.h>
#include <render/pipeline_process.h>
#include <serialization/defs.h>
#include <serialization/serializers.h>
#include <serialization/render/pipeline.h>
#include <utils/uuid.h>
#include <utils/packed_set.h>
#include <fstream>

using namespace element;

static void pipeline_importer(const uuid& id) {
    const fs_resource_info& info = fs::get_resource_info(id);
    std::optional<render::pipeline_data> data = asset_loaders::pipeline_text_load(info.path);
    if (render::pipeline_create_layout_data(data.value())) {
        auto output = fs::get_resource_ostream(id);
        binary_serializer serialize = create_binary_serializer(*output);
        serialize(data.value());
    } else {
        fs::delete_resource_data(id);
    }
    packed_set<uuid> dependencies;
    dependencies.insert(data->vert_id);
    dependencies.insert(data->frag_id);
    asset_importer::set_id_dependencies(id, std::move(dependencies));
}

ELM_REGISTER_IMPORTER("pipeline", pipeline_importer, 2)
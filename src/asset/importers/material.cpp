#include <asset/importer.h>
#include <asset/dependencies.h>
#include <asset/loaders/material.h>
#include <core/fs_editor.h>
#include <render/material.h>
#include <serialization/defs.h>
#include <serialization/serializers.h>
#include <serialization/render/material.h>
#include <utils/uuid.h>
#include <utils/packed_set.h>
#include <fstream>

using namespace element;

static void material_importer(const uuid& id) {
    fs_resource_info info = fs::get_resource_info(id);
    std::optional<render::material> material = asset_loaders::material_text_load(info.path);
    if (material->get_pipeline_data() != nullptr) {
        auto output = fs::get_resource_ostream(id);
        binary_serializer serialize = create_binary_serializer(*output);
        serialize(material.value());
    } else {
        fs::delete_resource_data(id);
    }
    packed_set<uuid> dependencies;
    dependencies.insert(material->get_pipeline_id());
    asset_importer::set_id_dependencies(id, std::move(dependencies));
}

ELM_REGISTER_IMPORTER("material", material_importer)
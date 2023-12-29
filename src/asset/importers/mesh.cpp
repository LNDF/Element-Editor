#include <asset/importer.h>
#include <asset/loaders/mesh.h>
#include <core/log.h>
#include <core/fs_editor.h>
#include <render/mesh.h>
#include <mesh/mesh.h>
#include <serialization/defs.h>
#include <serialization/serializers.h>
#include <serialization/render/mesh.h>
#include <serialization/mesh/mesh.h>
#include <utils/uuid.h>
#include <fstream>

using namespace element;

static void mesh_importer(const uuid& id) {
    fs_resource_info info = fs::get_resource_info(id);
    std::optional<mesh::mesh> m = asset_loaders::mesh_binary_load(info.path);
    auto output = fs::get_resource_ostream(id);
    render::mesh rm;
    binary_serializer serialize = create_binary_serializer(*output);
    if (!m->vertex_sizes_valid()) {
        ELM_WARN("Invalid vertex data sizes for mesh {0}", info.path);
        fs::delete_resource_data(id);
        return;
    }
    std::size_t vertex_count = m->positions.size();
    rm.indices = m->indices;
    rm.vertices.resize(vertex_count);
    for (std::size_t i = 0; i < vertex_count; ++i) {
        rm.vertices[i].position = m->positions[i];
        rm.vertices[i].normal = m->normals[i];
        rm.vertices[i].tangent = m->tangents[i];
        rm.vertices[i].tex_coords = m->tex_coords[i];
    }
    serialize(rm);
}

ELM_REGISTER_IMPORTER("mesh", mesh_importer)
#include "model_importer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <mesh/mesh.h>
#include <serialization/defs.h>
#include <serialization/serializers_editor.h>
#include <serialization/mesh/mesh.h>
#include <glm/glm.hpp>
#include <cstring>
#include <regex>
#include <memory>
#include <fstream>
#include <unordered_set>

using namespace element;

static std::regex name_pattern("[^a-z0-9]", std::regex_constants::icase | std::regex_constants::optimize);

class unique_name_generator {
    private:
        std::uint32_t i = 0;
        std::string type;
        std::unordered_set<std::string> used_names;
    public:
        unique_name_generator(const char* type) : type(type) {}

        std::string get(const char* input) {
            std::string name;
            if (input == nullptr || strlen(input) == 0) {
                do {
                    name = type + std::to_string(i++);
                } while (used_names.contains(name));
            } else {
                std::string base_name = std::regex_replace(input, name_pattern, "_");
                name = base_name;
                std::uint32_t c = 0;
                while (used_names.contains(name)) {
                    name = base_name + std::to_string(c++);
                }
            }
            used_names.insert(name);
            return name;
        }
};

static void extract_all_meshes(aiMesh** meshes, std::uint32_t count, const std::filesystem::path& out) {
    unique_name_generator name_gen("mesh");
    std::filesystem::create_directories(out);
    for (std::uint32_t i = 0; i < count; ++i) {
        aiMesh* input_mesh = meshes[i];
        mesh::mesh output_mesh;
        std::uint32_t num_vertices = input_mesh->mNumVertices;
        if (input_mesh->HasPositions()) {
            output_mesh.positions.resize(num_vertices);
            memcpy(&output_mesh.positions[0], input_mesh->mVertices, sizeof(glm::vec3) * num_vertices);
        }
        if (input_mesh->HasNormals()) {
            output_mesh.normals.resize(num_vertices);
            memcpy(&output_mesh.normals[0], input_mesh->mNormals, sizeof(glm::vec3) * num_vertices);
        }
        if (input_mesh->HasTangentsAndBitangents()) {
            output_mesh.tangents.resize(num_vertices);
            memcpy(&output_mesh.tangents[0], input_mesh->mTangents, sizeof(glm::vec3) * num_vertices);
        }
        if (input_mesh->HasTextureCoords(0)) {
            output_mesh.tex_coords.resize(num_vertices);
            for (std::uint32_t j = 0; j < num_vertices; ++j) {
                output_mesh.tex_coords[j].x = input_mesh->mTextureCoords[0][j].x;
                output_mesh.tex_coords[j].y = input_mesh->mTextureCoords[0][j].y;
            }
        }
        if (input_mesh->HasFaces()) {
            std::size_t offset = 0;
            output_mesh.indices.resize(input_mesh->mNumFaces * 3);
            for (std::uint32_t j = 0; j < input_mesh->mNumFaces; ++j) {
                output_mesh.indices[offset++] = input_mesh->mFaces[i].mIndices[0];
                output_mesh.indices[offset++] = input_mesh->mFaces[i].mIndices[1];
                output_mesh.indices[offset++] = input_mesh->mFaces[i].mIndices[2];
            }
        }
        std::ofstream stream(out / (name_gen.get(input_mesh->mName.C_Str()) + ".mesh"));
        binary_serializer serialize = create_binary_serializer(stream);
        serialize(output_mesh);
    }
}

void mesh::model_import(const model_import_settings& settings) {
    Assimp::Importer importer;
    importer.SetPropertyBool(AI_CONFIG_PP_FD_REMOVE, true);
    importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);
    importer.SetPropertyFloat(AI_CONFIG_PP_CT_MAX_SMOOTHING_ANGLE, settings.tangent_smooth_angle);
    std::uint32_t flags = aiProcess_GenUVCoords
                        | aiProcess_CalcTangentSpace
                        | aiProcess_TransformUVCoords
                        | aiProcess_Triangulate
                        | aiProcess_SortByPType
                        | aiProcess_ImproveCacheLocality
                        | aiProcess_FindDegenerates
                        | aiProcess_FindInstances
                        | aiProcess_FindInvalidData;
    if (settings.join_identical_vertices) flags |= aiProcess_JoinIdenticalVertices;
    if (settings.force_generate_normals) flags |= aiProcess_ForceGenNormals;
    if (settings.generate_smooth_normals) {
        flags |= aiProcess_GenSmoothNormals;
        importer.SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, settings.normal_smooth_angle);
    } else {
        flags |= aiProcess_GenNormals;
    }
    if (settings.fix_infacing_normals) flags |= aiProcess_FixInfacingNormals;
    if (settings.optimize_meshes) flags |= aiProcess_OptimizeMeshes;
    if (settings.optimize_graph) flags |= aiProcess_OptimizeGraph;
    if (settings.flip_faces) flags |= aiProcess_FlipWindingOrder;
    const aiScene* scene = importer.ReadFile(settings.model.string(), flags);
    if (true) { //TODO: split by material
        extract_all_meshes(scene->mMeshes, scene->mNumMeshes, settings.destination / "meshes");
    }
}
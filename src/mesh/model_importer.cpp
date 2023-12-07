#include "model_importer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace element;

void mesh::model_import(const model_import_settings& settings) {
    Assimp::Importer importer;
    importer.SetPropertyBool(AI_CONFIG_PP_FD_REMOVE, true);
    importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);
    std::uint32_t flags = aiProcess_GenUVCoords
                        | aiProcess_TransformUVCoords
                        | aiProcess_Triangulate
                        | aiProcess_SortByPType
                        | aiProcess_ImproveCacheLocality
                        | aiProcess_FindDegenerates
                        | aiProcess_FindInstances
                        | aiProcess_FindInvalidData;
    if (settings.calc_tangent) {
        flags |= aiProcess_CalcTangentSpace;
        importer.SetPropertyFloat(AI_CONFIG_PP_CT_MAX_SMOOTHING_ANGLE, settings.tangent_smooth_angle);
    }
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
    if (settings.flip_faces) aiProcess_FlipWindingOrder;
    const aiScene* scene = importer.ReadFile(settings.model.string(), flags);
    if (true) { //TODO: split by material
        //TODO: extract meshes
    }
}
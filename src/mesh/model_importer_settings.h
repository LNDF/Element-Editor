#pragma once

#include <filesystem>

namespace element {
    namespace mesh {

        struct model_import_settings {
            bool calc_tangent = true;
            float tangent_smooth_angle = 175.0f;
            bool join_identical_vertices = true;
            bool force_generate_normals = true;
            bool generate_smooth_normals = true;
            float normal_smooth_angle = 80.0f;
            bool fix_infacing_normals = false;
            bool optimize_meshes = false;
            bool optimize_graph = false;
            bool flip_faces = false;
            std::filesystem::path model;
            std::filesystem::path destination;
        };

    } //namespace mesh
} //namespace element
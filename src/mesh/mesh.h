#pragma once

#include <glm/glm.hpp>
#include <cinttypes>
#include <vector>

namespace element {
    namespace mesh {

        struct mesh {
            std::uint32_t version = 0;
            std::vector<glm::vec3> positions;
            std::vector<glm::vec3> normals;
            std::vector<glm::vec3> tangents;
            std::vector<glm::vec2> tex_coords;
            std::vector<std::uint32_t> indices;
        };

    } //namespace mesh
} //namespace element
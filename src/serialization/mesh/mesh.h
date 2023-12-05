#pragma once

#include <mesh/mesh.h>
#include <serialization/glm.h>
#include <serialization/vector.h>
#include <cereal/cereal.hpp>

namespace cereal {
    
    template<class Archive>
    void serialize(Archive& ar, element::mesh::mesh& mesh) {
        ar("version", mesh.version);
        ar("positions", mesh.positions);
        ar("normals", mesh.normals);
        ar("tangents", mesh.tangents);
        ar("tex_coords", mesh.tex_coords);
    }

} //namespace cereal
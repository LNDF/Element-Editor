#pragma once

#include <mesh/mesh.h>
#include <serialization/glm.h>
#include <serialization/vector.h>
#include <cereal/cereal.hpp>

namespace cereal {
    
    template<class Archive>
    void serialize(Archive& ar, element::mesh::mesh& mesh) {
        ar(make_nvp("version", mesh.version));
        ar(make_nvp("positions", mesh.positions));
        ar(make_nvp("normals", mesh.normals));
        ar(make_nvp("tangents", mesh.tangents));
        ar(make_nvp("tex_coords", mesh.tex_coords));
        ar(make_nvp("indices", mesh.indices));
    }

} //namespace cereal
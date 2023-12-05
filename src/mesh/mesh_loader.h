#pragma once

#include <mesh/mesh.h>
#include <resource/loader.h>
#include <serialization/mesh/mesh.h>

namespace element {
    namespace mesh {
        ELM_MAKE_RESOURCE_LOADER(load, mesh);
    };
};
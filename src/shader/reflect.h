#pragma once

#include <shader/layout.h>
#include <utils/uuid.h>

namespace element {
    namespace shader {
        
        shader_layout reflect_from_spv(const std::vector<std::uint32_t>& spv);

    } // namespace shader
} // namespace element

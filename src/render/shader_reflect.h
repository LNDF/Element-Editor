#pragma once

#include <render/shader_layout.h>
#include <utils/uuid.h>

namespace element {
    namespace render {
        
        layout reflect_from_spv(const std::vector<std::uint32_t>& spv);

    } // namespace shader
} // namespace element

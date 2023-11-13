#pragma once

#include <render/shader_layout.h>
#include <utils/uuid.h>

namespace element {
    namespace render {
        
        shader_layout reflect_from_spv(const std::vector<std::uint32_t>& spv);
        shader_layout get_shader_reflection_data(const uuid& id);
        void save_shader_reflection_data(const uuid& id, const shader_layout& layout);

    } // namespace shader
} // namespace element

#pragma once

#include <shader/layout.h>
#include <utils/uuid.h>

namespace element {
    namespace shader {
        
        layout reflect_from_spv(const std::vector<std::uint32_t>& spv);
        layout get_reflection_data(const uuid& id);
        void save_reflection_data(const uuid& id, const layout& layout);

    } // namespace shader
} // namespace element

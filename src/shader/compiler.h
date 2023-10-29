#pragma once

#include <filesystem>

namespace element {
    namespace shader {
        
        enum shader_type {
            fragment, vertex
        };

        void compile_shader(const std::filesystem::path& path, shader_type type);

    } // namespace shader
} // namespace element

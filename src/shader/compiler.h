#pragma once

#include <filesystem>
#include <vector>
#include <string>

namespace element {
    namespace shader {

        struct compilation_result {
            std::vector<std::uint32_t> spv;
            std::size_t num_errors;
            std::size_t num_warnings;
            std::string message;
        };
        
        enum shader_type {
            fragment, vertex
        };

        compilation_result compile_shader(const std::filesystem::path& path, shader_type type);

    } // namespace shader
} // namespace element

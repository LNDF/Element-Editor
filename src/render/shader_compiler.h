#pragma once

#include <filesystem>
#include <vector>
#include <string>

namespace element {
    namespace render {

        struct compilation_result {
            std::vector<std::uint32_t> spv;
            std::vector<std::filesystem::path> local_includes;
            std::string message;
            bool success = false;
        };
        
        enum shader_stage {
            fragment, vertex
        };

        compilation_result compile_shader(const std::filesystem::path& path, shader_stage type);

        void compiler_init();
        void compiler_cleanup();

    } // namespace shader
} // namespace element

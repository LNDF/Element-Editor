#include "compiler.h"

#include <fstream>
#include <shaderc/shaderc.hpp>

using namespace element;

shader::compilation_result shader::compile_shader(const std::filesystem::path& path, shader_type type) {
    compilation_result ret;
    shaderc::Compiler compiler;
    shaderc::CompileOptions options;
    options.SetSourceLanguage(shaderc_source_language_glsl);
    //TODO: use actual vulkan version
    options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_0);
    options.SetTargetSpirv(shaderc_spirv_version_1_0);
    options.SetOptimizationLevel(shaderc_optimization_level_performance);
    // options.SetIncluder(); TODO
    shaderc_shader_kind kind;
    switch (type) {
        case fragment:
            kind = shaderc_vertex_shader;
            break;
        case vertex:
            kind = shaderc_fragment_shader;
            break;
        default:
            ret.num_errors = 1;
            ret.message = "Invalid or unsupported shader kind given";
            return ret;
    }
    std::ifstream stream(path, std::ios::binary);
    if (!stream.is_open()) {
        ret.num_errors = 1;
        ret.message = "Couldn't open input source file";
        return ret;
    }
    stream.seekg(0, std::ios::end);
    std::size_t stream_size = stream.tellg();
    stream.seekg(0, std::ios::beg);
    std::string source(stream_size, 0);
    stream.read(&source[0], stream_size);
    shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(source, kind, path.string().c_str(), options);
    ret.num_errors = result.GetNumErrors();
    ret.num_warnings = result.GetNumWarnings();
    ret.message = result.GetErrorMessage();
    if (result.GetCompilationStatus() == shaderc_compilation_status_success) {
        ret.spv.insert(ret.spv.begin(), result.begin(), result.end());
    }
    return ret;
}
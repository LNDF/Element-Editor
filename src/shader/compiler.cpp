#include "compiler.h"

#include <core/log.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#include <fstream>

using namespace element;

//Reference: https://lxjk.github.io/2020/03/10/Translate-GLSL-to-SPIRV-for-Vulkan-at-Runtime.html

static void init_glslang_resources(TBuiltInResource& resources) {
    resources.maxLights = 32;
    resources.maxClipPlanes = 6;
    resources.maxTextureUnits = 32;
    resources.maxTextureCoords = 32;
    resources.maxVertexAttribs = 64;
    resources.maxVertexUniformComponents = 4096;
    resources.maxVaryingFloats = 64;
    resources.maxVertexTextureImageUnits = 32;
    resources.maxCombinedTextureImageUnits = 80;
    resources.maxTextureImageUnits = 32;
    resources.maxFragmentUniformComponents = 4096;
    resources.maxDrawBuffers = 32;
    resources.maxVertexUniformVectors = 128;
    resources.maxVaryingVectors = 8;
    resources.maxFragmentUniformVectors = 16;
    resources.maxVertexOutputVectors = 16;
    resources.maxFragmentInputVectors = 15;
    resources.minProgramTexelOffset = -8;
    resources.maxProgramTexelOffset = 7;
    resources.maxClipDistances = 8;
    resources.maxComputeWorkGroupCountX = 65535;
    resources.maxComputeWorkGroupCountY = 65535;
    resources.maxComputeWorkGroupCountZ = 65535;
    resources.maxComputeWorkGroupSizeX = 1024;
    resources.maxComputeWorkGroupSizeY = 1024;
    resources.maxComputeWorkGroupSizeZ = 64;
    resources.maxComputeUniformComponents = 1024;
    resources.maxComputeTextureImageUnits = 16;
    resources.maxComputeImageUniforms = 8;
    resources.maxComputeAtomicCounters = 8;
    resources.maxComputeAtomicCounterBuffers = 1;
    resources.maxVaryingComponents = 60;
    resources.maxVertexOutputComponents = 64;
    resources.maxGeometryInputComponents = 64;
    resources.maxGeometryOutputComponents = 128;
    resources.maxFragmentInputComponents = 128;
    resources.maxImageUnits = 8;
    resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
    resources.maxCombinedShaderOutputResources = 8;
    resources.maxImageSamples = 0;
    resources.maxVertexImageUniforms = 0;
    resources.maxTessControlImageUniforms = 0;
    resources.maxTessEvaluationImageUniforms = 0;
    resources.maxGeometryImageUniforms = 0;
    resources.maxFragmentImageUniforms = 8;
    resources.maxCombinedImageUniforms = 8;
    resources.maxGeometryTextureImageUnits = 16;
    resources.maxGeometryOutputVertices = 256;
    resources.maxGeometryTotalOutputComponents = 1024;
    resources.maxGeometryUniformComponents = 1024;
    resources.maxGeometryVaryingComponents = 64;
    resources.maxTessControlInputComponents = 128;
    resources.maxTessControlOutputComponents = 128;
    resources.maxTessControlTextureImageUnits = 16;
    resources.maxTessControlUniformComponents = 1024;
    resources.maxTessControlTotalOutputComponents = 4096;
    resources.maxTessEvaluationInputComponents = 128;
    resources.maxTessEvaluationOutputComponents = 128;
    resources.maxTessEvaluationTextureImageUnits = 16;
    resources.maxTessEvaluationUniformComponents = 1024;
    resources.maxTessPatchComponents = 120;
    resources.maxPatchVertices = 32;
    resources.maxTessGenLevel = 64;
    resources.maxViewports = 16;
    resources.maxVertexAtomicCounters = 0;
    resources.maxTessControlAtomicCounters = 0;
    resources.maxTessEvaluationAtomicCounters = 0;
    resources.maxGeometryAtomicCounters = 0;
    resources.maxFragmentAtomicCounters = 8;
    resources.maxCombinedAtomicCounters = 8;
    resources.maxAtomicCounterBindings = 1;
    resources.maxVertexAtomicCounterBuffers = 0;
    resources.maxTessControlAtomicCounterBuffers = 0;
    resources.maxTessEvaluationAtomicCounterBuffers = 0;
    resources.maxGeometryAtomicCounterBuffers = 0;
    resources.maxFragmentAtomicCounterBuffers = 1;
    resources.maxCombinedAtomicCounterBuffers = 1;
    resources.maxAtomicCounterBufferSize = 16384;
    resources.maxTransformFeedbackBuffers = 4;
    resources.maxTransformFeedbackInterleavedComponents = 64;
    resources.maxCullDistances = 8;
    resources.maxCombinedClipAndCullDistances = 8;
    resources.maxSamples = 4;
    resources.maxMeshOutputVerticesNV = 256;
    resources.maxMeshOutputPrimitivesNV = 512;
    resources.maxMeshWorkGroupSizeX_NV = 32;
    resources.maxMeshWorkGroupSizeY_NV = 1;
    resources.maxMeshWorkGroupSizeZ_NV = 1;
    resources.maxTaskWorkGroupSizeX_NV = 32;
    resources.maxTaskWorkGroupSizeY_NV = 1;
    resources.maxTaskWorkGroupSizeZ_NV = 1;
    resources.maxMeshViewCountNV = 4;
    resources.limits.nonInductiveForLoops = 1;
    resources.limits.whileLoops = 1;
    resources.limits.doWhileLoops = 1;
    resources.limits.generalUniformIndexing = 1;
    resources.limits.generalAttributeMatrixVectorIndexing = 1;
    resources.limits.generalVaryingIndexing = 1;
    resources.limits.generalSamplerIndexing = 1;
    resources.limits.generalVariableIndexing = 1;
    resources.limits.generalConstantMatrixVectorIndexing = 1;
}

shader::compilation_result shader::compile_shader(const std::filesystem::path& path, shader_stage stage) {
    compilation_result ret;
    EShLanguage glslang_stage;
    switch (stage) {
    case vertex:
        glslang_stage = EShLangVertex;
        break;
    case fragment:
        glslang_stage = EShLangFragment;
        break;
    default:
        ret.message = "Unsupported shader stage. Only vertex and fragment supported.";
        return ret;
        break;
    }
    std::ifstream stream(path, std::ios::binary);
    if (!stream.is_open()) {
        ret.message = "Couldn't open input source file";
        return ret;
    }
    stream.seekg(0, std::ios::end);
    std::size_t stream_size = stream.tellg();
    stream.seekg(0, std::ios::beg);
    std::string source(stream_size, 0);
    int source_len = source.length();
    std::string path_str = path.string();
    stream.read(&source[0], stream_size);
    glslang::TShader shader(glslang_stage);
    glslang::TProgram program;
    TBuiltInResource resources = {};
    init_glslang_resources(resources);
    EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);
    const char* sources[] = {source.c_str()};
    const char* names[] = {path_str.c_str()};
    shader.setEnhancedMsgs();
    shader.setEnvClient(glslang::EShClient::EShClientVulkan, glslang::EshTargetClientVersion::EShTargetVulkan_1_0);
    shader.setEnvTarget(glslang::EShTargetLanguage::EShTargetSpv, glslang::EShTargetLanguageVersion::EShTargetSpv_1_0);
    shader.setStringsWithLengthsAndNames(sources, &source_len, names, 1);
    if (!shader.parse(&resources, 100, false, messages)) { //TODO: includer
        ret.message = shader.getInfoLog();
        return ret;
    }
    program.addShader(&shader);
    if (!program.link(messages)) {
        ret.message = shader.getInfoLog();
        return ret;
    }
    glslang::TIntermediate* intermediate = program.getIntermediate(glslang_stage);
    glslang::GlslangToSpv(*intermediate, ret.spv);
    return ret;
}

void shader::compiler_init() {
    ELM_INFO("Initializing shader compiler...");
    glslang::InitializeProcess();
}

void shader::compiler_cleanup() {
    ELM_INFO("Cleanning up shader compiler...");
    glslang::FinalizeProcess();
}
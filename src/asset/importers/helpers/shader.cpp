#include "shader.h"

#include <core/log.h>
#include <asset/importer.h>
#include <core/fs_editor.h>
#include <editor/project.h>

using namespace element;

void element::importers::helpers::shader_compile(const uuid& id, shader::shader_stage type) {
    fs_resource_info info = fs::get_resource_info(id);
    std::filesystem::path path = project::project_assets_path / info.path;
    shader::compilation_result result = shader::compile_shader(path, type);
    ELM_ERROR("Error during shader compilation:\n{0}", result.message);
    if (result.spv.size() > 0) {
        auto output = fs::get_resource_ostream(id);
        output->write(reinterpret_cast<const char*>(result.spv.data()), sizeof(std::uint32_t) * result.spv.size());
    }
}
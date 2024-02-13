#include "shader.h"

#include <core/log.h>
#include <asset/importer.h>
#include <asset/dependencies.h>
#include <core/fs_editor.h>
#include <editor/project.h>
#include <utils/packed_set.h>
#include <filesystem>
#include <string>

using namespace element;

void element::importers::helpers::shader_compile(const uuid& id, render::shader_stage type) {
    const fs_resource_info& info = fs::get_resource_info(id);
    std::filesystem::path path = asset_importer::get_system_path_from_fs(info.path);
    render::compilation_result result = render::compile_shader(path, type);
    if (!result.success) ELM_ERROR("Error during shader compilation:\n{0}", result.message);
    if (result.spv.size() > 0) {
        auto output = fs::get_resource_ostream(id);
        output->write(reinterpret_cast<const char*>(result.spv.data()), sizeof(std::uint32_t) * result.spv.size());
    } else {
        fs::delete_resource_data(id);
    }
    packed_set<std::string> dependencies;
    dependencies.reserve(result.local_includes.size());
    for (const std::filesystem::path& inc_path : result.local_includes) {
        dependencies.insert(asset_importer::get_fs_path_from_system(inc_path));
    }
    asset_importer::set_path_dependencies(id, std::move(dependencies));
}
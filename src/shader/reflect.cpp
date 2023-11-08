#include "reflect.h"

#include <core/log.h>
#include <core/fs.h>
#include <event/event.h>
#include <asset/asset_events.h>
#include <editor/project.h>
#include <editor/project_events.h>
#include <serialization/defs.h>
#include <serialization/serializers_editor.h>
#include <serialization/shader/layout.h>
#include <spirv_cross.hpp>
#include <spirv.hpp>
#include <fstream>
#include <filesystem>

using namespace element;

static std::filesystem::path shader_reflect_path;

static shader::member_type member_type_from_spirv_cross(const spirv_cross::SPIRType::BaseType& type) {
    switch (type) {
        case spirv_cross::SPIRType::Boolean:
            return shader::member_type::boolean_type;
        case spirv_cross::SPIRType::SByte:
            return shader::member_type::sint8_type;
        case spirv_cross::SPIRType::UByte:
            return shader::member_type::uint8_type;
        case spirv_cross::SPIRType::Short:
            return shader::member_type::sint16_type;
        case spirv_cross::SPIRType::UShort:
            return shader::member_type::uint16_type;
        case spirv_cross::SPIRType::Int:
            return shader::member_type::sint32_type;
        case spirv_cross::SPIRType::UInt:
            return shader::member_type::uint32_type;
        case spirv_cross::SPIRType::Int64:
            return shader::member_type::sint64_type;
        case spirv_cross::SPIRType::UInt64:
            return shader::member_type::uint64_type;
        case spirv_cross::SPIRType::Half:
            return shader::member_type::float16_type;
        case spirv_cross::SPIRType::Float:
            return shader::member_type::float32_type;
        case spirv_cross::SPIRType::Double:
            return shader::member_type::float64_type;
        case spirv_cross::SPIRType::Struct:
            return shader::member_type::struct_type;
        case spirv_cross::SPIRType::SampledImage:
            return shader::member_type::sampler_type;
        default:
            return shader::member_type::unknown;
    }
}

static void populate_array(shader::member_layout& target, const spirv_cross::SPIRType& type) {
    const auto& array = type.array;
    if (!array.empty()) {
        target.array_cols = array[0];
        if (array.size() > 1) {
            target.array_rows = array[1];
        }
        if (array.size() > 2) {
            ELM_WARN("Only up to two dimensional arrays are supported");
        }
    }
}

static void populate_resource(shader::resource_layout& target, const spirv_cross::Resource& res, const spirv_cross::Compiler& comp) {
    const auto& type = comp.get_type(res.type_id);
    target.binding = comp.get_decoration(res.id, spv::Decoration::DecorationBinding);
    target.set = comp.get_decoration(res.id, spv::Decoration::DecorationDescriptorSet);
    target.name = res.name;
    populate_array(target, type);
}

static void populate_uniform_block_resource(shader::resource_layout& target, const spirv_cross::Resource& res, const spirv_cross::Compiler& comp) {
    populate_resource(target, res, comp);
    const auto& base_type = comp.get_type(res.base_type_id);
    if (base_type.member_types.empty()) return;
    target.size = comp.get_declared_struct_size(base_type);
    target.type = shader::member_type::struct_type;
    std::uint32_t member_count = base_type.member_types.size();
    for (std::uint32_t i = 0; i < member_count; ++i) {
        shader::member_layout member;
        const auto& member_type = comp.get_type(base_type.member_types[i]);
        member.size = comp.get_declared_struct_member_size(base_type, i);
        member.offset = comp.type_struct_member_offset(base_type, i);
        if (!member_type.array.empty()) {
            member.array_stride = comp.type_struct_member_array_stride(base_type, i);
        }
        if (member_type.columns > 0) {
            member.matrix_stride = comp.type_struct_member_matrix_stride(base_type, i);
        }
        member.name = comp.get_member_name(base_type.self, i);
        member.type = member_type_from_spirv_cross(member_type.basetype);
        member.vecsize = member_type.vecsize;
        member.columns = member_type.columns;
        populate_array(member, member_type);
        target.members.push_back(std::move(member));
    }
}

shader::layout shader::reflect_from_spv(const std::vector<std::uint32_t>& spv) {
    shader::layout result;
    spirv_cross::Compiler compiler(spv);
    spirv_cross::ShaderResources resources = compiler.get_shader_resources();
    const auto& push_constants = resources.push_constant_buffers;
    const auto& uniform_buffers = resources.uniform_buffers;
    const auto& sampled_images = resources.sampled_images;
    if (push_constants.size() > 1) {
        ELM_WARN("Only 1 push constant buffer supported, {} given", push_constants.size());
    }
    if (push_constants.size() > 0) {
        populate_uniform_block_resource(result.push_constants, push_constants[0], compiler);
        result.push_constants.sampler_resource = false;
    }
    result.push_constants.sampler_resource = false;
    result.descriptor_sets.resize(uniform_buffers.size() + sampled_images.size());
    std::size_t offs = 0;
    for (const auto& buffer : uniform_buffers) {
        populate_uniform_block_resource(result.descriptor_sets[offs], buffer, compiler);
        result.descriptor_sets[offs++].sampler_resource = false;
    }
    for (const auto& image : sampled_images) {
        populate_resource(result.descriptor_sets[offs], image, compiler);
        result.descriptor_sets[offs].type = shader::member_type::sampler_type;
        result.descriptor_sets[offs++].sampler_resource = true;
    }
    return result;
}

shader::layout shader::get_reflection_data(const uuid& id) {
    std::ifstream cache_stream(shader_reflect_path / (id.str() + ".json"));
    if (!cache_stream.fail()) {
        text_deserializer deserialize = create_text_deserializer(cache_stream);
        layout result;
        deserialize(ELM_SERIALIZE_NVP("layout", result));
        return result;
    }
    std::unique_ptr<std::istream> spv_stream = fs::get_resource(id);
    if (spv_stream->fail()) {
        return shader::layout();
    }
    spv_stream->seekg(0, std::ios::end);
    std::size_t stream_size = spv_stream->tellg();
    spv_stream->seekg(0, std::ios::beg);
    std::vector<std::uint32_t> spv;
    spv.resize(stream_size / sizeof(std::uint32_t));
    spv_stream->read((char*) &spv[0], stream_size);
    layout result = reflect_from_spv(spv);
    save_reflection_data(id, result);
    return result;
}

void shader::save_reflection_data(const uuid& id, const shader::layout& layout) {
    std::filesystem::create_directories(shader_reflect_path);
    std::ofstream cache_stream(shader_reflect_path / (id.str() + ".json"));
    text_serializer serialize = create_text_serializer(cache_stream);
    serialize(ELM_SERIALIZE_NVP("layout", layout));
}

static bool set_shader_reflect_path(events::project_opened& event) {
    shader_reflect_path = project::project_cache_path / "shader_reflect";
    return true;
}

static void delete_reflect_cache(const uuid& id) {
    fs_resource_info info = fs::get_resource_info(id);
    if (info.type == "vert" || info.type == "frag") {
        std::filesystem::remove(shader_reflect_path / (id.str() + ".json"));
    }
}

static bool delete_reflect_cache_when_updated(events::asset_updated& event) {
    delete_reflect_cache(event.id);
    return true;
}

static bool delete_reflect_cache_when_deleted(events::asset_deleted& event) {
    delete_reflect_cache(event.id);
    return true;
}

ELM_REGISTER_EVENT_CALLBACK(events::project_opened, set_shader_reflect_path, event_callback_priority::highest)
ELM_REGISTER_EVENT_CALLBACK(events::asset_updated, delete_reflect_cache_when_updated, event_callback_priority::highest)
ELM_REGISTER_EVENT_CALLBACK(events::asset_deleted, delete_reflect_cache_when_deleted, event_callback_priority::highest)
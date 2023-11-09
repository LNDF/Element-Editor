#include "reflect.h"

#include <core/log.h>
#include <spirv_cross.hpp>
#include <spirv.hpp>

using namespace element;

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

shader::shader_layout shader::reflect_from_spv(const std::vector<std::uint32_t>& spv) {
    shader::shader_layout result;
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
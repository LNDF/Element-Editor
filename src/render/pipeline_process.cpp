#include "pipeline_process.h"

#include <core/log.h>
#include <core/fs.h>
#include <render/shader_reflect.h>
#include <optional>

using namespace element;

#define ELM_ERROR_PIPELINE(set, binding, vert_id, frag_id, error) ELM_ERROR("Error processing pipeline.\nResource on set {0} with binding {1} on vertex shader {2} and fragment shader {3} " error, set, binding, fs::get_resource_info(vert_id).path, fs::get_resource_info(frag_id).path);

static bool check_layout_compatibility(const uuid& vert_id, const uuid& frag_id, const render::shader_resource_layout& vert, const render::shader_resource_layout& frag) {
    if (vert.type != frag.type) {
        ELM_ERROR_PIPELINE(vert.set, vert.binding, vert_id, frag_id, "are of different types");
        return false;
    }
    if (vert.size != frag.size) {
        ELM_ERROR_PIPELINE(vert.set, vert.binding, vert_id, frag_id, "have different sizes");
        return false;
    }
    return true;
}

bool render::pipeline_create_layout_data(pipeline_data& data) {
    std::optional<shader_layout> vert_layout = get_shader_reflection_data(data.vert_id);
    std::optional<shader_layout> frag_layout = get_shader_reflection_data(data.frag_id);
    if (!vert_layout.has_value() || !frag_layout.has_value()) return false;
    if (vert_layout->push_constants.size > 0) {
        data.push_constants.second |= vk::ShaderStageFlagBits::eVertex;
        data.push_constants.first = vert_layout->push_constants;
    }
    if (frag_layout->push_constants.size > 0) {
        data.push_constants.second |= vk::ShaderStageFlagBits::eFragment;
        if (vert_layout->push_constants.size == 0) {
            data.push_constants.first = frag_layout->push_constants;
        } else if (vert_layout->push_constants.size != frag_layout->push_constants.size) {
            ELM_ERROR("Error processing pipeline.\nVertex shader {0} and fragment shader {1} have different push constant block sizes", fs::get_resource_info(data.vert_id).path, fs::get_resource_info(data.frag_id).path);
            return false;
        }
    }
    for (const shader_resource_layout& vert_res : vert_layout->descriptor_sets) {
        bool found_in_frag = false;
        vk::ShaderStageFlags stage = vk::ShaderStageFlagBits::eVertex;
        for (const shader_resource_layout& frag_res : frag_layout->descriptor_sets) {
            if (vert_res.set == frag_res.set && vert_res.binding == frag_res.binding) {
                found_in_frag = true;
                if (!check_layout_compatibility(data.vert_id, data.frag_id, vert_res, frag_res)) return false;
            }
        }
        if (found_in_frag) stage |= vk::ShaderStageFlagBits::eFragment;
        data.layouts.emplace_back(vert_res, stage);
    }
    for (const shader_resource_layout& frag_res : frag_layout->descriptor_sets) {
        bool found_in_vert = false;
        for (const shader_resource_layout& vert_res : vert_layout->descriptor_sets) {
            if (vert_res.set == frag_res.set && vert_res.binding == frag_res.binding) {
                found_in_vert = true;
                break;
            }
        }
        if (!found_in_vert) {
            data.layouts.emplace_back(frag_res, vk::ShaderStageFlagBits::eFragment);
        }
    }
    return true;
}
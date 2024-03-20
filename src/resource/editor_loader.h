#pragma once

#include <asset/importer.h>
#include <core/log.h>
#include <editor/project.h>
#include <utils/uuid.h>
#include <serialization/defs.h>
#include <serialization/serializers_editor.h>
#include <fstream>
#include <optional>

#include <cereal/details/traits.hpp>

namespace element {
    namespace resource {
        
        template<typename T>
        std::optional<T> load_editor_text_asset(const std::string& path, const char* top_level_name) {
            std::filesystem::path sys_path = asset_importer::get_system_path_from_fs(path);
            std::ifstream stream(sys_path);
            if (stream.fail()) {
                ELM_WARN("Couldn't load asset {0}", path);
                return std::nullopt;
            }
            T t;
            try {
                text_deserializer deserialize = create_text_deserializer(stream);
                deserialize(ELM_SERIALIZE_NVP(top_level_name, t));
            } catch (const std::exception& e) {
                ELM_WARN("Couldn't deserialize asset {0}: {1}", path, e.what());
                return std::nullopt;
            }
            return t;
        }

        template<typename T>
        bool save_editor_text_asset(const std::string& path, const char* top_level_name, const T& t) {
            std::filesystem::path sys_path = asset_importer::get_system_path_from_fs(path);
            std::ofstream stream(sys_path);
            if (stream.fail()) {
                ELM_WARN("Couldn't save asset {0}", path);
                return false;
            }
            text_serializer serialize = create_text_serializer(stream);
            serialize(ELM_SERIALIZE_NVP(top_level_name, t));
            return true;
        }

        template<typename T>
        std::optional<T> load_editor_binary_asset(const std::string& path) {
            std::filesystem::path sys_path = asset_importer::get_system_path_from_fs(path);
            std::ifstream stream(sys_path, std::ios::binary);
            if (stream.fail()) {
                ELM_WARN("Couldn't load asset {0}", path);
                return std::nullopt;
            }
            T t;
            try {
                binary_deserializer deserialize = create_binary_deserializer(stream);
                deserialize(t);
            } catch (const std::exception& e) {
                ELM_WARN("Couldn't deserialize asset {0}: {1}", path, e.what());
                return std::nullopt;
            }
            return t;
        }

        template<typename T>
        bool save_editor_binary_asset(const std::string& path, const T& t) {
            std::filesystem::path sys_path = asset_importer::get_system_path_from_fs(path);
            std::ofstream stream(sys_path, std::ios::binary);
            if (stream.fail()) {
                ELM_WARN("Couldn't save asset {0}", path);
                return false;
            }
            binary_serializer serialize = create_binary_serializer(stream);
            serialize(t);
            return true;
        }

    } // namespace resource
} // namespace element

#define ELM_MAKE_EDITOR_ASSET_FUNCTIONS(fun, type, top_level_name) inline std::optional<type> fun##_text_load(const std::string& path) {return element::resource::load_editor_text_asset<type>(path, top_level_name);} \
inline bool fun##_text_save(const std::string& path, const type& t) {return element::resource::save_editor_text_asset<type>(path, top_level_name, t);} \
inline std::optional<type> fun##_binary_load(const std::string& path) {return element::resource::load_editor_binary_asset<type>(path);} \
inline bool fun##_binary_save(const std::string& path, const type& t) {return element::resource::save_editor_binary_asset<type>(path, t);}

#pragma once

#include <utils/pre_exec.h>
#include <functional>
#include <string>
#include <unordered_map>

#define ELM_REGISTER_ASSET_CREATOR(type, name, factory) ELM_PRE_EXECUTE(element::__detail::__asset_creator_register_creatory_factory, type, {name, factory})

namespace element {
    namespace asset_creator {
        struct creator_type_info;
    } // namespace asset_chreator

    namespace __detail {
        std::unordered_map<std::string, asset_creator::creator_type_info>& __asset_creator_get_creator_info();

        void __asset_creator_register_creator_info(std::string&& type, asset_creator::creator_type_info factory);
    } // namespace __detail

    namespace asset_creator {
        struct creator_type_info {
            using creator_factory_type = std::function<void(const std::string&)>;

            std::string name;
            creator_factory_type factory;
        };

        const creator_type_info& get_creator_info(const std::string& type);
        const std::unordered_map<std::string, asset_creator::creator_type_info>& get_creator_info_map();

        void blank_file_creator(const std::string& path);
    } // namespace asset_creator    
} // namespace element
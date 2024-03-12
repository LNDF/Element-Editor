#include "asset_creators_info.h"

#include <editor/project.h>
#include <filesystem>
#include <fstream>

using namespace element;

std::unordered_map<std::string, asset_creator::creator_type_info>& __detail::__asset_creator_get_creator_info() {
    static std::unordered_map<std::string, asset_creator::creator_type_info> map;
    return map;
}

void __detail::__asset_creator_register_creator_info(std::string&& type, asset_creator::creator_type_info factory) {
    __asset_creator_get_creator_info().try_emplace(std::move(type), factory);
}

 const asset_creator::creator_type_info& asset_creator::get_creator_info(const std::string& type) {
    return __detail::__asset_creator_get_creator_info().at(type);
 }

const std::unordered_map<std::string, asset_creator::creator_type_info>& asset_creator::get_creator_info_map() {
    return __detail::__asset_creator_get_creator_info();
}

void asset_creator::blank_file_creator(const std::string& path) {
    std::filesystem::path fs_path = project::project_assets_path / path;
    std::ofstream stream(fs_path);
}
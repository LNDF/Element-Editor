#include "asset_type_info.h"

using namespace element;

ui::asset_type_info::asset_type_info(const std::string& name, properties_factory_type properties_factory, creator_type creator)
    : name(name), properties_factory(properties_factory), creator(creator) {}

ui::asset_type_info::asset_type_info(std::string&& name, properties_factory_type properties_factory, creator_type creator)
    : name(std::move(name)), properties_factory(properties_factory), creator(creator) {}
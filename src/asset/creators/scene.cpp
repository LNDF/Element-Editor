#include <asset/creators/asset_creators_info.h>
#include <asset/loaders/scene.h>
#include <string>

static void asset_create_scene(const std::string& path) {
    element::scenegraph::scene scene;
    element::asset_loader::scene_text_save(path, scene);
}

ELM_REGISTER_ASSET_CREATOR("scene", "Scene", asset_create_scene)
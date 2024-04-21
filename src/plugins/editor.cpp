#include "editor.h"

#include <core/log.h>
#include <editor/project.h>
#include <plugins/loader.h>
#include <utils/platform.h>

using namespace element;

std::array<const char*, 2> plugins::platforms = {
    "windows",
    "linux"
};

void plugins::load_plugins() {
    ELM_INFO("Loading plugins...");
    unload_all_plugins();
    for (const auto& platform_plugin : project::plugins[ELM_PLATFORM_STRING]) {
        load_plugin(project::project_bin_path / ELM_PLATFORM_STRING / platform_plugin);
    }
}
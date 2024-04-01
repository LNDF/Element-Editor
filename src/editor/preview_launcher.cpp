#include "preview_launcher.h"

#include <editor/project.h>
#include <utils/cmdline.h>
#include <string>

static char* editor_argv0 = nullptr;
static std::string project_escaped_path;

using namespace element;

void editor::launch_preview(const QProcess& process, bool current_scene) {
    //TODO: Implement this function
}

void editor::setup_preview_launcher(char* argv0) {
    editor_argv0 = argv0;
    project_escaped_path = process_string_for_cmdline(project::project_path);
}
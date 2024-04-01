#include "preview_launcher.h"

#include <core/log.h>
#include <asset/importer.h>
#include <asset/asset_events.h>
#include <event/event.h>
#include <editor/project.h>
#include <scenegraph/scene.h>
#include <scenegraph/editor_scene_loader.h>
#include <string>

static char* editor_argv0 = nullptr;
static QProcess* deferred_launch = nullptr;

using namespace element;

void editor::launch_preview(QProcess* process, bool current_scene) {
    ELM_DEBUG("Configuring preview process.");
    process->setProgram(editor_argv0);
    QStringList args;
    args << "--preview";
    if (current_scene) {
        args << "--preview-scene" << scenegraph::get_current_scene()->get_id().str().c_str();
    }
    args << project::project_path.c_str();
    process->setArguments(args);
    QObject::connect(process, &QProcess::finished, asset_importer::resume_imports);
    scenegraph::save_scene();
    if (asset_importer::are_imports_pending() || scenegraph::get_current_scene() != nullptr) {
        ELM_INFO("Deferring preview launch until all assets are imported.");
        deferred_launch = process;
    } else {
        ELM_INFO("Launching preview.");
        asset_importer::pause_imports();
        process->start();
    }
}

void editor::cancel_preview_deferred_launch() {
    deferred_launch = nullptr;
}

void editor::setup_preview_launcher(char* argv0) {
    editor_argv0 = argv0;
}

static bool launch_deferred(events::assets_imported& event) {
    if (deferred_launch != nullptr) {
        ELM_INFO("Launching deferred preview.");
        asset_importer::pause_imports();
        deferred_launch->start();
        deferred_launch = nullptr;
    }
    return true;
}

ELM_REGISTER_EVENT_CALLBACK(events::assets_imported, launch_deferred, event_callback_priority::highest)

#include <QApplication>

#include <core/engine.h>
#include <core/log.h>
#include <display/backend.h>
#include <display/window.h>
#include <editor/editor.h>
#include <editor/project.h>
#include <render/vulkan_qt.h>
#include <render/render.h>
#include <scenegraph/scene_manager.h>
#include <ui/project_properties.h>
#include <utils/uuid.h>
#include <tclap/CmdLine.h>
#include <QMessageBox>
#include <iostream>

static QApplication* qt_app = nullptr;

using namespace element;

static void launch_editor(QApplication* qt_app) {
    vulkan::vulkan_init_qt_extensions();
    engine::setup();
    ELM_INFO("This is the editor. Running editor...");
    editor::run_editor(qt_app);
    engine::cleanup();
}

static void launch_preview(const element::uuid& preview_scene) {
    display::init_backend();
    engine::setup();
    ELM_INFO("Loading preview scene...");
    display::open_window();
    scenegraph::load_scene(preview_scene);
    render::get_screen_scene_renderer()->select_scene(preview_scene);
    ELM_INFO("This is the editor. Running preview...");
    engine::execute();
    display::close_window();
    engine::cleanup();
    display::cleanup_backend();
}

static void run(char* argv0, const std::string& project_path, bool preview_mode, const element::uuid& preview_scene) {
    project::open(project_path);
    if (!project::exists()) {
        ELM_INFO("Creating new project...");
        project::name = "";
        project::author = "";
        project::version = "";
        project::startup_scene = uuid::null();
        ui::project_properties dialog;
        if (dialog.exec() == QDialog::Rejected) {
            ELM_INFO("User cancelled project creation. Exiting...");
            return;
        }
    } else {
        project::mkdir();
        project::load();
    }
    engine::settings.app_name = "Element editor";
    engine::settings.app_version = ELM_EDITOR_VERSION;
    if (preview_mode) {
        if (preview_scene == uuid::null()) {
            ELM_DEBUG("No preview scene specified. Using project startup scene.");
            launch_preview(project::startup_scene);
        } else {
            ELM_DEBUG("Preview scene specified: {0}", preview_scene.str());
            launch_preview(preview_scene);
        }
    } else {
        launch_editor(qt_app);
    }
}

static void terminate() {
    {   
        QMessageBox msg;
        msg.setWindowTitle("Element editor");
        msg.setText(QCoreApplication::translate("element-editor", "An error occurred and the Element editor must be stopped."));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        std::exception_ptr p = std::current_exception();
        if(p != nullptr) {
            try {
                std::rethrow_exception(p);
            } catch(const std::exception& e) {
                ELM_CRITICAL("An exception occurred: {0}", e.what());
                msg.setInformativeText(e.what());
            }
        } else {
            ELM_CRITICAL("An unknown error occurred.");
            msg.setInformativeText("Unknown error.");
        }
        msg.exec();
    }
    delete qt_app;
}

int main(int argc, char** argv) {
    TCLAP::CmdLine cmd("Element editor", ' ', ELM_EDITOR_VERSION);
    TCLAP::SwitchArg preview_mode("", "preview", "Run in preview mode", cmd, false);
    TCLAP::ValueArg<std::string> preview_scene("", "preview-scene", "Scene to load in preview mode", false, "00000000-0000-0000-0000-000000000000", "UUID", cmd);
    TCLAP::UnlabeledValueArg<std::string> project_path("project", "Project path", true, "", "path", cmd);
    try {
        cmd.parse(argc, argv);
    } catch (TCLAP::ArgException& e) {
        std::cout << "Error: " << e.error() << " for arg " << e.argId() << std::endl;
        return 1;
    }
    ELM_SET_LOG_LEVEL(ELM_LOG_LEVEL_DEBUG);
    int args = 1;
    qt_app = new QApplication(args, argv);
    std::set_terminate(terminate);
    run(argv[0], project_path.getValue(), preview_mode.getValue(), element::uuid(preview_scene.getValue()));
    return 0;
}
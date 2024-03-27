#include <QApplication>

#include <core/engine.h>
#include <core/log.h>
#include <editor/editor.h>
#include <editor/project.h>
#include <render/vulkan_qt.h>
#include <ui/project_properties.h>
#include <utils/uuid.h>
#include <tclap/CmdLine.h>
#include <QMessageBox>
#include <iostream>

static QApplication* qt_app = nullptr;

using namespace element;

void launch_editor(QApplication* qt_app) {
    vulkan::vulkan_init_qt_extensions();
    engine::setup();
    ELM_INFO("This is the editor. Running editor...");
    editor::run_editor(qt_app);
    engine::cleanup();
}

void run(char* argv0, const std::string& project_path, bool preview_mode, const element::uuid& preview_scene) {
    project::open(project_path);
    if (!project::exists()) {
        ELM_INFO("Creating new project...");
        project::name = "";
        project::author = "";
        project::version = "";
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
    launch_editor(qt_app);
}

void terminate() {
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
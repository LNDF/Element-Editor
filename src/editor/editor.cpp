#include "editor.h"

#include <QMetaObject>
#include <core/log.h>
#include <core/fs_editor.h>
#include <asset/importer.h>
#include <render/vulkan_qt.h>

using namespace element;

QApplication* editor::qt_app = nullptr;
ui::element_editor* editor::main_window = nullptr;

void editor::execute_in_editor_thread(std::function<void()> call) {
    QMetaObject::invokeMethod(main_window, call, Qt::QueuedConnection);
}

void editor::run_editor(QApplication* app) {
    fs::save_resources();
    QVulkanInstance* q_vulkan_instance = new QVulkanInstance();
    q_vulkan_instance->setVkInstance(vulkan::get_instance());
    vulkan_qt::set_vulkan_instance(q_vulkan_instance);
    qt_app = app;
    main_window = new ui::element_editor();
    main_window->show();
    asset_importer::start();
    ELM_DEBUG("Executing editor Qt application. Qt is taking control of the program.");
    qt_app->exec();
    asset_importer::stop();
    delete main_window;
    main_window = nullptr;
    qt_app = nullptr;
    delete vulkan_qt::get_vulkan_instance();
    vulkan_qt::set_vulkan_instance(nullptr);
}
#include <core/application.h>
#include <core/log.h>
#include <event/event.h>
#include <event/core_events.h>

using namespace element;

bool update_callback(const events::update& e) {
    static int counter = 5;
    ELM_DEBUG("Update counter: {0}", counter);
    if (counter-- == 0) {
        event_manager::post_event<events::close>({});
    }
    return true;
}

int main() {
    application_settings settings;
    settings.app_name = "Element editor";
    settings.app_version = ELM_EDITOR_VERSION;
    application::setup_engine(settings);
    event_manager::register_listener<events::update>(update_callback);
    application::start();
    application::cleanup_engine();
    return 0;
}
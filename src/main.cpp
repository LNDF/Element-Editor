#include <core/application.h>
#include <core/log.h>

int main() {
    element::application::setup_engine({});
    ELM_INFO("TEST");
    element::application::cleanup_engine();
    return 0;
}
#include "qt_display.h"

#include <assert.h>

#include <core/log.h>
#include <render/display.h>

static QWidget* game_widget = nullptr;

static element::display* game_display = nullptr; //only one display for now...

namespace element {
    class qt_display : public display {
        public:
            qt_display();
            ~qt_display() final override;
            int get_width() const final override;
            int get_height() const final override;
            void set_size(int width, int height) final override;
            rendering_context* get_rendering_context() final override;
            void update() final override;
    };

    void qt_display_set_game_widget(QWidget* widget) {
        game_widget = widget;
    }

    QWidget* qt_display_get_game_widget() {
        return game_widget;
    }

    display* create_display() {
        if (game_display == nullptr) {
            ELM_INFO("Creating new display...");
            game_display = new qt_display();
            return game_display;
        }
        ELM_WARN("Only one display is allowed for now.");
        return nullptr;
    }

    void update_all_displays() {
        if (game_display != nullptr) game_display->update();
    }

    void cleanup_displays() {
        ELM_INFO("Cleanning up displays...");
        delete game_display;
        game_display = nullptr;
        game_widget = nullptr;
    }

    qt_display::qt_display() {
        assert(game_widget != nullptr);
    }
    qt_display::~qt_display() {}

    int qt_display::get_width() const {
        return game_widget->size().width();
    }

    int qt_display::get_height() const {
        return game_widget->size().height();
    }

    void qt_display::set_size(int width, int height) {
        //cannot resize editor widget
    }

    rendering_context* qt_display::get_rendering_context() {
        return nullptr;
    }

    void qt_display::update() {
        game_widget->update();
    }

} // namespace element

#pragma once

#include <QWidget>

namespace element {
    void qt_display_set_game_widget(QWidget* widget);
    QWidget* qt_display_get_game_widget();
} // namespace element

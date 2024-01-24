#pragma once

#include <QSpinBox>
#include <QDoubleSpinBox>

namespace element {
    namespace ui {
        
        class integer_input : public QSpinBox {
            Q_OBJECT
            public:
                integer_input(QWidget* parent = nullptr);
        };

        class double_input : public QDoubleSpinBox {
            Q_OBJECT
            public:
                double_input(QWidget* parent = nullptr);
        };

    } // namespace ui
} // namespace element
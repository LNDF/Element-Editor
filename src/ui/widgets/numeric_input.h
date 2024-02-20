#pragma once

#include <QSpinBox>
#include <QDoubleSpinBox>

namespace element {
    namespace ui {
        
        class integer_input : public QSpinBox {
            Q_OBJECT
            protected:
                virtual QString textFromValue(int value) const;
            public:
                integer_input(QWidget* parent = nullptr);
        };

        class float_input : public QDoubleSpinBox {
            Q_OBJECT
            public:
                float_input(QWidget* parent = nullptr);

                virtual QString textFromValue(double value) const;
        };

        class double_input : public QDoubleSpinBox {
            Q_OBJECT
            public:
                double_input(QWidget* parent = nullptr);

                virtual QString textFromValue(double value) const;
        };

    } // namespace ui
} // namespace element
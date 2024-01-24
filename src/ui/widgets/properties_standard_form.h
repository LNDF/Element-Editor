#pragma once

#include <QWidget>
#include <QGroupBox>
#include <QFormLayout>
#include <QLabel>
#include <string>
#include <utility>

namespace element {
    namespace ui {
        
        class properties_standard_form : public QGroupBox {
            Q_OBJECT
            private:
                QFormLayout* properties_standard_form_layout;
            public:
                properties_standard_form(QWidget* parent = nullptr);
                void add_property(const QString& name, QWidget* widget);
            signals:
                void values_changed();
        };

    } // namespace ui
} // namespace element 
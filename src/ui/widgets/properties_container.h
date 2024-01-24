#pragma once

#include <QWidget>
#include <QVBoxLayout>

namespace element {
    namespace ui {
        
        class properties_container : public QWidget {
            Q_OBJECT
            private:
                QVBoxLayout* properties_container_layout = nullptr;
            public:
                properties_container(QWidget* parent = nullptr);
                virtual ~properties_container();
            public slots:
                virtual void load_values();
            signals:
                void values_changed();
        };

    } // namespace ui
} // namespace element 
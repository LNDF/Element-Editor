#pragma once

#include <utils/uuid.h>
#include <QPushButton>
#include <string>

namespace element {
    namespace ui {

        class asset_input : public QPushButton {
            Q_OBJECT
            private:
                uuid asset = uuid::null();
                std::string type;
            protected:
                virtual void dragEnterEvent(QDragEnterEvent* event);
                virtual void dragMoveEvent(QDragMoveEvent* event);
                virtual void dragLeaveEvent(QDragLeaveEvent* event);
                virtual void dropEvent(QDropEvent *event);
            public:
                asset_input(const std::string& type, QWidget* parent = nullptr);
                uuid value() const;
                void set_value(const uuid& v);
            private slots:
                void open_asset_chooser();
            signals:
                void value_changed(uuid v);
        };

    } // namespace ui
} // namespace element

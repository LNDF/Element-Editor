#pragma once

#include <ui/widgets/properties_container.h>
#include <utils/uuid.h>
#include <QWidget>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QPushButton>

namespace element {
    namespace ui {
        
        class properties_asset : public properties_container {
            Q_OBJECT
            private:
                QWidget* save_reload;
                QHBoxLayout* save_reload_layout;
                QSpacerItem* save_reload_space;
                QPushButton* save_reload_save;
                QPushButton* save_reload_reload;
            public:
                properties_asset(const uuid& id, QWidget* parent = nullptr);
        };

    } // namespace ui    
} // namespace element
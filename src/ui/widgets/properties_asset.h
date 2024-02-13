#pragma once

#include <ui/widgets/properties_container.h>
#include <utils/uuid.h>

namespace element {
    namespace ui {
        
        class properties_asset : public properties_container {
            Q_OBJECT
            public:
                properties_asset(const uuid& id, QWidget* parent = nullptr);
        };

    } // namespace ui    
} // namespace element
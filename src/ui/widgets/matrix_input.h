#pragma once

#include <ui/widgets/numeric_input.h>
#include <glm/glm.hpp>
#include <QGridLayout>
#include <QWidget>

namespace element {
    namespace ui {
        
        class mat2_input : public QWidget {
            Q_OBJECT
            private:
                QGridLayout* mat2_input_layout = nullptr;
                real_input* inputs[2 * 2];
            public:
                mat2_input(QWidget* parent = nullptr);
                float value(std::uint32_t i, std::uint32_t j) const;
                glm::mat2 value() const;
                void set_value(std::uint32_t i, std::uint32_t j, float v);
                void set_value(glm::mat2 v);
            private slots:
                void change_value();
            signals:
                void value_changed(glm::mat2 v);
        };

    } // namespace ui    
} // namespace element
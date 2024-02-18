#pragma once

#include <QFormLayout>
#include <QWidget>
#include <functional>
#include <vector>

namespace element {
    namespace ui {
        
        class array_input : public QWidget {
            Q_OBJECT
            private:
                using factory_type = std::function<QWidget*(QWidget*)>;

                std::vector<QWidget*> widgets;
            public:
                array_input(std::uint32_t count, factory_type factory, QWidget* parent = nullptr);
                inline const std::vector<QWidget*>& get_widgets() {return widgets;}
        };

        class array2d_input : public QWidget {
            Q_OBJECT
            private:
                using factory_type = std::function<QWidget*(QWidget*)>;

                std::vector<QWidget*> widgets;
            public:
                array2d_input(std::uint32_t count1, std::uint32_t count2, factory_type factory, QWidget* parent = nullptr);
                inline const std::vector<QWidget*>& get_widgets() {return widgets;}
        };

    } // namespace ui
} // namespace element

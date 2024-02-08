#pragma once

#include <ui/widgets/numeric_input.h>
#include <glm/glm.hpp>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

namespace element {
    namespace ui {

        class vec2_input : public QWidget {
            Q_OBJECT
            private:
                QHBoxLayout* vec2_input_layout = nullptr;
                QLabel* labels[2];
                real_input* inputs[2];
            public:
                vec2_input(QWidget* parent = nullptr);
                float value(std::uint32_t i) const;
                glm::vec2 value() const;
                void set_value(std::uint32_t i, float v);
                void set_value(glm::vec2 v);

                inline float x() const {return value(0);}
                inline void set_x(float v) {set_value(0, v);}
                inline float y() const {return value(1);}
                inline void set_y(float v) {set_value(1, v);}
            private slots:
                void change_component();
            signals:
                void value_changed(glm::vec2 v);
        };

        class vec3_input : public QWidget {
            Q_OBJECT
            private:
                QHBoxLayout* vec3_input_layout = nullptr;
                QLabel* labels[3];
                real_input* inputs[3];
            public:
                vec3_input(QWidget* parent = nullptr);
                float value(std::uint32_t i) const;
                glm::vec3 value() const;
                void set_value(std::uint32_t i, float v);
                void set_value(glm::vec3 v);

                inline float x() const {return value(0);}
                inline void set_x(float v) {set_value(0, v);}
                inline float y() const {return value(1);}
                inline void set_y(float v) {set_value(1, v);}
                inline float z() const {return value(2);}
                inline void set_z(float v) {set_value(2, v);}
            private slots:
                void change_component();
            signals:
                void value_changed(glm::vec3 v);
        };

        class vec4_input : public QWidget {
            Q_OBJECT
            private:
                QHBoxLayout* vec4_input_layout = nullptr;
                QLabel* labels[4];
                real_input* inputs[4];
            public:
                vec4_input(QWidget* parent = nullptr);
                float value(std::uint32_t i) const;
                glm::vec4 value() const;
                void set_value(std::uint32_t i, float v);
                void set_value(glm::vec4 v);

                inline float x() const {return value(0);}
                inline void set_x(float v) {set_value(0, v);}
                inline float y() const {return value(1);}
                inline void set_y(float v) {set_value(1, v);}
                inline float z() const {return value(2);}
                inline void set_z(float v) {set_value(2, v);}
                inline float w() const {return value(3);}
                inline void set_w(float v) {set_value(3, v);}
            private slots:
                void change_component();
            signals:
                void value_changed(glm::vec2 v);
        };

    } // namespace ui
} // namespace element

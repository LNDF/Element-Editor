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
                float_input* inputs[2];
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
                float_input* inputs[3];
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
                float_input* inputs[4];
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

        class dvec2_input : public QWidget {
            Q_OBJECT
            private:
                QHBoxLayout* dvec2_input_layout = nullptr;
                QLabel* labels[2];
                double_input* inputs[2];
            public:
                dvec2_input(QWidget* parent = nullptr);
                double value(std::uint32_t i) const;
                glm::dvec2 value() const;
                void set_value(std::uint32_t i, double v);
                void set_value(glm::dvec2 v);

                inline double x() const {return value(0);}
                inline void set_x(double v) {set_value(0, v);}
                inline double y() const {return value(1);}
                inline void set_y(double v) {set_value(1, v);}
            private slots:
                void change_component();
            signals:
                void value_changed(glm::dvec2 v);
        };

        class dvec3_input : public QWidget {
            Q_OBJECT
            private:
                QHBoxLayout* dvec3_input_layout = nullptr;
                QLabel* labels[3];
                double_input* inputs[3];
            public:
                dvec3_input(QWidget* parent = nullptr);
                double value(std::uint32_t i) const;
                glm::dvec3 value() const;
                void set_value(std::uint32_t i, double v);
                void set_value(glm::dvec3 v);

                inline double x() const {return value(0);}
                inline void set_x(double v) {set_value(0, v);}
                inline double y() const {return value(1);}
                inline void set_y(double v) {set_value(1, v);}
                inline double z() const {return value(2);}
                inline void set_z(double v) {set_value(2, v);}
            private slots:
                void change_component();
            signals:
                void value_changed(glm::dvec3 v);
        };

        class dvec4_input : public QWidget {
            Q_OBJECT
            private:
                QHBoxLayout* dvec4_input_layout = nullptr;
                QLabel* labels[4];
                double_input* inputs[4];
            public:
                dvec4_input(QWidget* parent = nullptr);
                double value(std::uint32_t i) const;
                glm::dvec4 value() const;
                void set_value(std::uint32_t i, double v);
                void set_value(glm::dvec4 v);

                inline double x() const {return value(0);}
                inline void set_x(double v) {set_value(0, v);}
                inline double y() const {return value(1);}
                inline void set_y(double v) {set_value(1, v);}
                inline double z() const {return value(2);}
                inline void set_z(double v) {set_value(2, v);}
                inline double w() const {return value(3);}
                inline void set_w(double v) {set_value(3, v);}
            private slots:
                void change_component();
            signals:
                void value_changed(glm::dvec4 v);
        };

        class ivec2_input : public QWidget {
            Q_OBJECT
            private:
                QHBoxLayout* ivec2_input_layout = nullptr;
                QLabel* labels[2];
                integer_input* inputs[2];
            public:
                ivec2_input(QWidget* parent = nullptr);
                int value(std::uint32_t i) const;
                glm::ivec2 value() const;
                void set_value(std::uint32_t i, int v);
                void set_value(glm::ivec2 v);

                inline int x() const {return value(0);}
                inline void set_x(int v) {set_value(0, v);}
                inline int y() const {return value(1);}
                inline void set_y(int v) {set_value(1, v);}
            private slots:
                void change_component();
            signals:
                void value_changed(glm::ivec2 v);
        };

        class ivec3_input : public QWidget {
            Q_OBJECT
            private:
                QHBoxLayout* ivec3_input_layout = nullptr;
                QLabel* labels[3];
                integer_input* inputs[3];
            public:
                ivec3_input(QWidget* parent = nullptr);
                int value(std::uint32_t i) const;
                glm::ivec3 value() const;
                void set_value(std::uint32_t i, int v);
                void set_value(glm::ivec3 v);

                inline int x() const {return value(0);}
                inline void set_x(int v) {set_value(0, v);}
                inline int y() const {return value(1);}
                inline void set_y(int v) {set_value(1, v);}
                inline int z() const {return value(2);}
                inline void set_z(int v) {set_value(2, v);}
            private slots:
                void change_component();
            signals:
                void value_changed(glm::ivec3 v);
        };

        class ivec4_input : public QWidget {
            Q_OBJECT
            private:
                QHBoxLayout* ivec4_input_layout = nullptr;
                QLabel* labels[4];
                integer_input* inputs[4];
            public:
                ivec4_input(QWidget* parent = nullptr);
                int value(std::uint32_t i) const;
                glm::ivec4 value() const;
                void set_value(std::uint32_t i, int v);
                void set_value(glm::ivec4 v);

                inline int x() const {return value(0);}
                inline void set_x(int v) {set_value(0, v);}
                inline int y() const {return value(1);}
                inline void set_y(int v) {set_value(1, v);}
                inline int z() const {return value(2);}
                inline void set_z(int v) {set_value(2, v);}
                inline int w() const {return value(3);}
                inline void set_w(int v) {set_value(3, v);}
            private slots:
                void change_component();
            signals:
                void value_changed(glm::ivec4 v);
        };

    } // namespace ui
} // namespace element
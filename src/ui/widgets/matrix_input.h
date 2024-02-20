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
                float_input* inputs[2 * 2];
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

        class mat23_input : public QWidget {
            Q_OBJECT
            private:
                QGridLayout* mat23_input_layout = nullptr;
                float_input* inputs[2 * 3];
            public:
                mat23_input(QWidget* parent = nullptr);
                float value(std::uint32_t i, std::uint32_t j) const;
                glm::mat2x3 value() const;
                void set_value(std::uint32_t i, std::uint32_t j, float v);
                void set_value(glm::mat2x3 v);
            private slots:
                void change_value();
            signals:
                void value_changed(glm::mat2x3 v);
        };

        class mat24_input : public QWidget {
            Q_OBJECT
            private:
                QGridLayout* mat24_input_layout = nullptr;
                float_input* inputs[2 * 4];
            public:
                mat24_input(QWidget* parent = nullptr);
                float value(std::uint32_t i, std::uint32_t j) const;
                glm::mat2x4 value() const;
                void set_value(std::uint32_t i, std::uint32_t j, float v);
                void set_value(glm::mat2x4 v);
            private slots:
                void change_value();
            signals:
                void value_changed(glm::mat2x4 v);
        };

        class mat32_input : public QWidget {
            Q_OBJECT
            private:
                QGridLayout* mat32_input_layout = nullptr;
                float_input* inputs[3 * 2];
            public:
                mat32_input(QWidget* parent = nullptr);
                float value(std::uint32_t i, std::uint32_t j) const;
                glm::mat3x2 value() const;
                void set_value(std::uint32_t i, std::uint32_t j, float v);
                void set_value(glm::mat3x2 v);
            private slots:
                void change_value();
            signals:
                void value_changed(glm::mat3x2 v);
        };

        class mat3_input : public QWidget {
            Q_OBJECT
            private:
                QGridLayout* mat3_input_layout = nullptr;
                float_input* inputs[3 * 3];
            public:
                mat3_input(QWidget* parent = nullptr);
                float value(std::uint32_t i, std::uint32_t j) const;
                glm::mat3 value() const;
                void set_value(std::uint32_t i, std::uint32_t j, float v);
                void set_value(glm::mat3 v);
            private slots:
                void change_value();
            signals:
                void value_changed(glm::mat3 v);
        };

        class mat34_input : public QWidget {
            Q_OBJECT
            private:
                QGridLayout* mat34_input_layout = nullptr;
                float_input* inputs[3 * 4];
            public:
                mat34_input(QWidget* parent = nullptr);
                float value(std::uint32_t i, std::uint32_t j) const;
                glm::mat3x4 value() const;
                void set_value(std::uint32_t i, std::uint32_t j, float v);
                void set_value(glm::mat3x4 v);
            private slots:
                void change_value();
            signals:
                void value_changed(glm::mat3x4 v);
        };

        class mat42_input : public QWidget {
            Q_OBJECT
            private:
                QGridLayout* mat42_input_layout = nullptr;
                float_input* inputs[4 * 2];
            public:
                mat42_input(QWidget* parent = nullptr);
                float value(std::uint32_t i, std::uint32_t j) const;
                glm::mat4x2 value() const;
                void set_value(std::uint32_t i, std::uint32_t j, float v);
                void set_value(glm::mat4x2 v);
            private slots:
                void change_value();
            signals:
                void value_changed(glm::mat4x2 v);
        };

        class mat43_input : public QWidget {
            Q_OBJECT
            private:
                QGridLayout* mat43_input_layout = nullptr;
                float_input* inputs[4 * 3];
            public:
                mat43_input(QWidget* parent = nullptr);
                float value(std::uint32_t i, std::uint32_t j) const;
                glm::mat4x3 value() const;
                void set_value(std::uint32_t i, std::uint32_t j, float v);
                void set_value(glm::mat4x3 v);
            private slots:
                void change_value();
            signals:
                void value_changed(glm::mat4x3 v);
        };

        class mat4_input : public QWidget {
            Q_OBJECT
            private:
                QGridLayout* mat4_input_layout = nullptr;
                float_input* inputs[4 * 4];
            public:
                mat4_input(QWidget* parent = nullptr);
                float value(std::uint32_t i, std::uint32_t j) const;
                glm::mat4 value() const;
                void set_value(std::uint32_t i, std::uint32_t j, float v);
                void set_value(glm::mat4 v);
            private slots:
                void change_value();
            signals:
                void value_changed(glm::mat4 v);
        };

    } // namespace ui    
} // namespace element
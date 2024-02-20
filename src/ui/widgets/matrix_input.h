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

        class dmat2_input : public QWidget {
            Q_OBJECT
            private:
                QGridLayout* dmat2_input_layout = nullptr;
                double_input* inputs[2 * 2];
            public:
                dmat2_input(QWidget* parent = nullptr);
                double value(std::uint32_t i, std::uint32_t j) const;
                glm::dmat2 value() const;
                void set_value(std::uint32_t i, std::uint32_t j, double v);
                void set_value(glm::dmat2 v);
            private slots:
                void change_value();
            signals:
                void value_changed(glm::dmat2 v);
        };

        class dmat23_input : public QWidget {
            Q_OBJECT
            private:
                QGridLayout* dmat23_input_layout = nullptr;
                double_input* inputs[2 * 3];
            public:
                dmat23_input(QWidget* parent = nullptr);
                double value(std::uint32_t i, std::uint32_t j) const;
                glm::dmat2x3 value() const;
                void set_value(std::uint32_t i, std::uint32_t j, double v);
                void set_value(glm::dmat2x3 v);
            private slots:
                void change_value();
            signals:
                void value_changed(glm::dmat2x3 v);
        };

        class dmat24_input : public QWidget {
            Q_OBJECT
            private:
                QGridLayout* dmat24_input_layout = nullptr;
                double_input* inputs[2 * 4];
            public:
                dmat24_input(QWidget* parent = nullptr);
                double value(std::uint32_t i, std::uint32_t j) const;
                glm::dmat2x4 value() const;
                void set_value(std::uint32_t i, std::uint32_t j, double v);
                void set_value(glm::dmat2x4 v);
            private slots:
                void change_value();
            signals:
                void value_changed(glm::dmat2x4 v);
        };

        class dmat32_input : public QWidget {
            Q_OBJECT
            private:
                QGridLayout* dmat32_input_layout = nullptr;
                double_input* inputs[3 * 2];
            public:
                dmat32_input(QWidget* parent = nullptr);
                double value(std::uint32_t i, std::uint32_t j) const;
                glm::dmat3x2 value() const;
                void set_value(std::uint32_t i, std::uint32_t j, double v);
                void set_value(glm::dmat3x2 v);
            private slots:
                void change_value();
            signals:
                void value_changed(glm::dmat3x2 v);
        };

        class dmat3_input : public QWidget {
            Q_OBJECT
            private:
                QGridLayout* dmat3_input_layout = nullptr;
                double_input* inputs[3 * 3];
            public:
                dmat3_input(QWidget* parent = nullptr);
                double value(std::uint32_t i, std::uint32_t j) const;
                glm::dmat3 value() const;
                void set_value(std::uint32_t i, std::uint32_t j, double v);
                void set_value(glm::dmat3 v);
            private slots:
                void change_value();
            signals:
                void value_changed(glm::dmat3 v);
        };

        class dmat34_input : public QWidget {
            Q_OBJECT
            private:
                QGridLayout* dmat34_input_layout = nullptr;
                double_input* inputs[3 * 4];
            public:
                dmat34_input(QWidget* parent = nullptr);
                double value(std::uint32_t i, std::uint32_t j) const;
                glm::dmat3x4 value() const;
                void set_value(std::uint32_t i, std::uint32_t j, double v);
                void set_value(glm::dmat3x4 v);
            private slots:
                void change_value();
            signals:
                void value_changed(glm::dmat3x4 v);
        };

        class dmat42_input : public QWidget {
            Q_OBJECT
            private:
                QGridLayout* dmat42_input_layout = nullptr;
                double_input* inputs[4 * 2];
            public:
                dmat42_input(QWidget* parent = nullptr);
                double value(std::uint32_t i, std::uint32_t j) const;
                glm::dmat4x2 value() const;
                void set_value(std::uint32_t i, std::uint32_t j, double v);
                void set_value(glm::dmat4x2 v);
            private slots:
                void change_value();
            signals:
                void value_changed(glm::dmat4x2 v);
        };

        class dmat43_input : public QWidget {
            Q_OBJECT
            private:
                QGridLayout* dmat43_input_layout = nullptr;
                double_input* inputs[4 * 3];
            public:
                dmat43_input(QWidget* parent = nullptr);
                double value(std::uint32_t i, std::uint32_t j) const;
                glm::dmat4x3 value() const;
                void set_value(std::uint32_t i, std::uint32_t j, double v);
                void set_value(glm::dmat4x3 v);
            private slots:
                void change_value();
            signals:
                void value_changed(glm::dmat4x3 v);
        };

        class dmat4_input : public QWidget {
            Q_OBJECT
            private:
                QGridLayout* dmat4_input_layout = nullptr;
                double_input* inputs[4 * 4];
            public:
                dmat4_input(QWidget* parent = nullptr);
                double value(std::uint32_t i, std::uint32_t j) const;
                glm::dmat4 value() const;
                void set_value(std::uint32_t i, std::uint32_t j, double v);
                void set_value(glm::dmat4 v);
            private slots:
                void change_value();
            signals:
                void value_changed(glm::dmat4 v);
        };

    } // namespace ui    
} // namespace element
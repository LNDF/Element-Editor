#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QAbstractSpinBox>
#include <QtWidgets/QLineEdit>

/*
 * From https://stackoverflow.com/questions/8383620/64bit-int-spin-box-in-qt
*/

class QULongSpinBoxPrivate;
class QULongSpinBox : public QAbstractSpinBox {
    Q_OBJECT
    private:
        Q_PROPERTY(quint32 minimum READ minimum WRITE setMinimum)
        Q_PROPERTY(quint32 maximum READ maximum WRITE setMaximum)

        Q_PROPERTY(quint32 value READ value WRITE setValue NOTIFY valueChanged USER true)

        quint32 m_minimum;
        quint32 m_maximum;
        quint32 m_value;
    public:
        explicit QULongSpinBox(QWidget* parent = nullptr);
        ~QULongSpinBox();
        inline quint32 value() const {return m_value;}
        inline quint32 minimum() const {return m_minimum;}
        inline void setMinimum(quint32 min) {m_minimum = min;}
        inline quint32 maximum() const {return m_minimum;}
        inline void setMaximum(quint32 max) {m_maximum = max;}
        void setRange(quint32 min, quint32 max);
        virtual void stepBy(int steps);
    protected:
        virtual QValidator::State validate(QString &input, int &pos) const;
        virtual quint32 valueFromText(const QString &text) const;
        virtual QString textFromValue(quint32 val) const;
        virtual void fixup(QString &str) const;
        virtual QAbstractSpinBox::StepEnabled stepEnabled() const;
    public slots:
        void setValue(quint32 val);
    protected slots:
        void onEditFinished();
    signals:
        void textChanged(const QString &text);
        void valueChanged(quint32 v);
    private:
        Q_DISABLE_COPY(QULongSpinBox)
        Q_DECLARE_PRIVATE(QULongSpinBox)
};
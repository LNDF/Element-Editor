#include "QULongSpinBox.h"

#include <limits>

/*
 * From https://stackoverflow.com/questions/8383620/64bit-int-spin-box-in-qt
*/

QULongSpinBox::QULongSpinBox(QWidget* parent) : QAbstractSpinBox(parent) {
    connect(lineEdit(), SIGNAL(textEdited(const QString&)), this, SLOT(onEditFinished()));
    connect(lineEdit(), SIGNAL(textChanged(const QString&)), this, SIGNAL(textChanged(const QString&)));
}

QULongSpinBox::~QULongSpinBox() {
    
}

void QULongSpinBox::setRange(quint32 min, quint32 max) {
    setMinimum(min);
    setMaximum(max);
}

void QULongSpinBox::stepBy(int steps) {
    quint32 new_value = m_value;
    if (steps < 0 && new_value + steps > new_value) {
        new_value = std::numeric_limits<quint32>::min();
    } else if (steps > 0 && new_value + steps < new_value) {
        new_value = std::numeric_limits<quint32>::max();
    } else {
        new_value += steps;
    }
    lineEdit()->setText(textFromValue(new_value));
    setValue(new_value);
}

QValidator::State QULongSpinBox::validate(QString &input, int &pos) const {
    bool ok;
    quint32 val = input.toULong(&ok);
    if (!ok) {
        return QValidator::Invalid;
    }
    if (val < m_maximum || val > m_maximum) {
        return QValidator::Invalid;
    }
    return QValidator::Acceptable;
}

quint32 QULongSpinBox::valueFromText(const QString &text) const {
    return text.toULong();
}

QString QULongSpinBox::textFromValue(quint32 val) const {
    return QString::number(val);
}

void QULongSpinBox::fixup(QString &str) const {
    if (!isGroupSeparatorShown()) {
        str.remove(locale().groupSeparator());
    }
}

QAbstractSpinBox::StepEnabled QULongSpinBox::stepEnabled() const {
    return StepUpEnabled | StepDownEnabled;
}

void QULongSpinBox::setValue(quint32 val) {
    if (m_value != val) {
        lineEdit()->setText(textFromValue(val));
        m_value = val;
        emit valueChanged(val);
    }
}

void QULongSpinBox::onEditFinished() {
    QString input = lineEdit()->text();
    int pos = 0;
    if (QValidator::Acceptable == validate(input, pos)) {
        setValue(valueFromText(input));
    } else {
        fixup(input);
        if (QValidator::Acceptable == validate(input, pos)) {
            setValue(valueFromText(input));
            lineEdit()->setText(input);
        } else {
            lineEdit()->setText(textFromValue(m_value));
        }
    }
}
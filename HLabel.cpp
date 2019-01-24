#include "HLabel.h"
#include <QMouseEvent>

HLabel::HLabel(QWidget *parent, Qt::WindowFlags f):
    QLabel(parent, f)
{ }

HLabel::HLabel(const QString &text, QWidget *parent, Qt::WindowFlags f):
    QLabel(text, parent, f)
{ }

void HLabel::mousePressEvent(QMouseEvent *event)
{
    QLabel::mousePressEvent(event);
    emit clicked();
}

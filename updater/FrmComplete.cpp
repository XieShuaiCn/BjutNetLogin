#include "FrmComplete.h"
#include <QLabel>

FrmComplete::FrmComplete(QWidget* parent)
    : FrmStep(parent)
{
    m_lblInfo = new QLabel(this);
    m_lblInfo->setGeometry(100, 20, 400, 50);
    m_lblInfo->setText("Complete!");
}

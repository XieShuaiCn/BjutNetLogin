#include "FrmWelcome.h"
#include <QLabel>

FrmWelcome::FrmWelcome(QWidget* parent)
    : FrmStep(parent)
{
    m_lblInfo = new QLabel(this);
    m_lblInfo->setGeometry(100, 20, 400, 50);
    m_lblInfo->setText("Welcome to update <BjutNetLogin>!");
}


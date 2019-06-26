#ifndef FRMUPDATE_H
#define FRMUPDATE_H

#include "FrmStep.h"

class QLabel;

class FrmUpdate : public FrmStep
{
public:
    FrmUpdate(QWidget* parent = Q_NULLPTR);

private:
    QLabel *m_lblInfo;
};

#endif // FRMUPDATE_H

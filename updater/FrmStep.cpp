#include "FrmStep.h"

FrmStep::FrmStep(QWidget* parent)
    : QFrame(parent)
{
    this->setVisible(false);
}

bool FrmStep::canGoLast()
{
    return true;
}

bool FrmStep::canGoNext()
{
    return true;
}

bool FrmStep::canCancel()
{
    return true;
}
bool FrmStep::canProcess()
{
    return true;
}

void FrmStep::doBefore()
{}

void FrmStep::doProcess()
{}

void FrmStep::doAfter()
{}

void FrmStep::doRollBack()
{}

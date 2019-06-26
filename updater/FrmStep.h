#ifndef FRMSTEP_H
#define FRMSTEP_H

#include <QFrame>

class FrmStep : public QFrame
{
public:
    FrmStep(QWidget* parent = Q_NULLPTR);
    virtual bool canGoLast();
    virtual bool canGoNext();
    virtual bool canCancel();

    virtual bool canProcess();

    virtual void doBefore();
    virtual void doProcess();
    virtual void doAfter();
    virtual void doRollBack();
};

#endif // FRMSTEP_H

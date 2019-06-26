#ifndef WNDMAIN_H
#define WNDMAIN_H

#include <QWidget>
#include <vector>
#include <QTimer>
#include "FrmStep.h"

class QFrame;
class QPushButton;

class WndMain : public QWidget
{
    Q_OBJECT

public:

    WndMain(QWidget *parent = 0);

    ~WndMain();

    void doSwitchFrame();
    void finishSwitchFrame();

    void MoveFrame();

public slots:

    void initUI();

    void btnNextClicked();
    void btnLastClicked();
    void btnCancelClicked();

private:
    QFrame *m_frmShowStep;
    std::vector<FrmStep*> m_vecFrameStep;
    int m_nCurrentStep = -1;

    QFrame *m_frmButton;
    QPushButton *m_btnNext;
    QPushButton *m_btnLast;
    QPushButton *m_btnCancel;

    QTimer m_tmMoveFrame;
    int m_nDirection;

};

#endif // WNDMAIN_H

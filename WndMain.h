#ifndef MAINWND_H
#define MAINWND_H

#include "common.h"
#include "BjutNet.h"

class QAction;
class QApplication;
class QButtonGroup;
class QComboBox;
class QHeaderView;
class QLCDNumber;
class QLabel;
class QLineEdit;
class QPushButton;
class QTextEdit;
class QWidget;
class QMenu;
class QShowEvent;
class QCloseEvent;


class WndMain : public QWidget
{
    Q_OBJECT

public:
    explicit WndMain(QWidget *parent = 0);
    ~WndMain();
    void show();
    QString getStatus();

Q_SIGNALS:
    void showed();
    void closeEvent(QCloseEvent *event);
public slots:
    void initUI();
    void on_show();
    void on_close(QCloseEvent *event);
    void on_btnApply_click();
    void on_btnLogout_click();
    void on_btnLogin_click();
    void on_actApplyLogin_triggered(bool checked = false);
    void on_actApplyOnly_triggered(bool checked = false);
    void on_txtMsg_message(const QDateTime& time, const QString& info);
    void on_account_status(bool login, int time, int flow, int fee);
private:
    //界面所需变量
    QLabel *m_lblInfoTime;
    QLabel *m_lblInfoFlow;
    QLabel *m_lblInfoFee;
    QLCDNumber *m_lcdNumTime;
    QLCDNumber *m_lcdNumFlow;
    QLCDNumber *m_lcdNumFee;
    QLabel *m_lblAccount;
    QLabel *m_lblPassword;
    QLabel *m_lblType;
    QLineEdit *m_editAccount;
    QLineEdit *m_editPassword;
    QComboBox *m_cmbType;
    QPushButton *m_btnApplyMenu;
    QPushButton *m_btnApply;
    QPushButton *m_btnLogout;
    QPushButton *m_btnLogin;
    QTextEdit *m_txtMsg;
    QLabel *m_lblTimeUnit;
    QLabel *m_lblFlowUnit;
    QLabel *m_lblFeeUnit;
    QMenu *m_menuBtnApply;
    QAction *m_actMenuApplyOnly;
    QAction *m_actMenuApplyLogin;

    //逻辑控制所需变量
    bool m_bApplyLogin;
    BjutNet m_net;
};

#endif // MAINWND_H

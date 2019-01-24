#ifndef MAINWND_H
#define MAINWND_H

#include "common.h"
#include "BjutNet.h"
#include "Updater.h"
#include "HLabel.h"
#include <QProgressDialog>

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
class QPaintEvent;
class BjutNet;

class WndMain : public QWidget
{
    Q_OBJECT
    friend class BjutNet;
public:
    explicit WndMain(QWidget *parent = 0);
    ~WndMain();
    void show();
    QString getStatus();

Q_SIGNALS:
    void showed();
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *event);
public slots:
    void initUI();
    void on_show();
    void on_close(QCloseEvent *event);
    void on_paint(QPaintEvent *event);
    void on_btnApply_clicked();
    void on_btnLogout_clicked();
    void on_btnLogin_clicked();
    void on_actApplyLogin_triggered(bool checked = false);
    void on_actApplyOnly_triggered(bool checked = false);
    void on_lblVersion_clicked();
    void on_txtMsg_message(const QDateTime& time, const QString& info);
    void on_account_status(bool login, int time, int flow, int fee);
protected slots:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
private:
    //界面所需变量
    QFrame *m_frmGraph;
    QLabel *m_lblService;

    QFrame *m_frmInfo;
    QLabel *m_lblInfoTime;
    QLabel *m_lblInfoFlow;
    QLabel *m_lblInfoFee;
    QLCDNumber *m_lcdNumTime;
    QLCDNumber *m_lcdNumFlow;
    QLCDNumber *m_lcdNumFee;
    QLabel *m_lblTimeUnit;
    QLabel *m_lblFlowUnit;
    QLabel *m_lblFeeUnit;

    QFrame *m_frmOnline;

    QFrame *m_frmAccount;
    QLabel *m_lblAccount;
    QLabel *m_lblPassword;
    QLabel *m_lblType;
    QLineEdit *m_editAccount;
    QLineEdit *m_editPassword;
    QComboBox *m_cmbType;
    QPushButton *m_btnLogin;
    QPushButton *m_btnLogout;
    QPushButton *m_btnApply;
    QPushButton *m_btnApplyMenu;
    QMenu *m_menuBtnApply;
    QAction *m_actMenuApplyOnly;
    QAction *m_actMenuApplyLogin;

    QTextEdit *m_txtMsg;
    HLabel *m_lblVersion;

    //逻辑控制所需变量
    bool m_bApplyLogin;
    bool m_bNeedUpdate;
    BjutNet *m_net;
    Updater m_updater;
    QProgressDialog *m_dlgProgress;

};

#endif // MAINWND_H

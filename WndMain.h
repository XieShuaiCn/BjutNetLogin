#ifndef MAINWND_H
#define MAINWND_H

#include "common.h"
#include "BjutNet.h"

class QMenu;
class QShowEvent;
class QCloseEvent;

namespace Ui {
class WndMain;
}

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
private slots:
    void on_show();
    void on_close(QCloseEvent *event);
    void on_btnApply_click();
    void on_btnLogout_click();
    void on_actApplyLogin_triggered(bool checked = false);
    void on_actApplyOnly_triggered(bool checked = false);
    void on_txtMsg_message(const QDateTime& time, const QString& info);
    void on_account_status(bool login, int time, int flow, int fee);
private:
    Ui::WndMain *ui;
    QMenu *m_menuBtnApply;
    QAction *m_actMenuApplyOnly;
    QAction *m_actMenuApplyLogin;
    bool m_bApplyLogin;
    BjutNet net;
};

#endif // MAINWND_H

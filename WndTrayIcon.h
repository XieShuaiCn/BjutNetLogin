#ifndef WNDTRAYICON_H
#define WNDTRAYICON_H

#include "common.h"
#include <QTimer>
#include <QSystemTrayIcon>
#include <QLocalServer>

class WndMain;
class WndSetting;
class BjutNet;

class WndTrayIcon : public QSystemTrayIcon
{
public:
    WndTrayIcon(QApplication *app, QObject *parent = Q_NULLPTR);
    ~WndTrayIcon();
    PROPERTY_READ(BjutNet*, BjutNet, m_net)
    PROPERTY_READ_WRITE(WndMain*, MainWindow, m_wndMain)

Q_SIGNALS:
    void quitApp();

public slots:
    void reciveMessage(const QString &msg);

protected slots:
    void on_actived(QSystemTrayIcon::ActivationReason reason);
    void on_clicked();
    void on_actMenuQuit_trigger();
    void on_actMenuShowMain_trigger();
    void on_actMenuSetting_trigger();
    void on_actMenuLogin_trigger();
    void on_actMenuLogout_trigger();
    void socket_newConnection();

    QTimer m_tmClick;

    QApplication *m_app;
    WndMain *m_wndMain;
    WndSetting *m_wndSetting;
    BjutNet *m_net;
    QMenu *m_menuTray;
    QAction *m_actMenuShowMain;
    QAction *m_actMenuLogout;
    QAction *m_actMenuLogin;
    QAction *m_actMenuSetting;
    QAction *m_actMenuQuit;
};

#endif // WNDTRAYICON_H

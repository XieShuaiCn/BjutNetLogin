#ifndef WNDTRAYICON_H
#define WNDTRAYICON_H

#include "common.h"
#include "WndMain.h"
#include <QTimer>
#include <QSystemTrayIcon>
#include <QLocalServer>

class WndTrayIcon : public QSystemTrayIcon
{
public:
    WndTrayIcon(QApplication *app = Q_NULLPTR, WndMain *parent = Q_NULLPTR);
    ~WndTrayIcon();

Q_SIGNALS:
    void quitApp();

public slots:
    void reciveMessage(const QString &msg);

protected slots:
    void on_actived(QSystemTrayIcon::ActivationReason reason);
    void on_clicked();
    void on_actMenuQuit_trigger();
    void on_actMenuShowMain_trigger();
    void socket_newConnection();

    QTimer m_tmClick;

    WndMain *m_wndMain;
    QApplication *m_app;
    QMenu *m_menuTray;
    QAction *m_actMenuShowMain;
    QAction *m_actMenuLogout;
    QAction *m_actMenuLogin;
    QAction *m_actMenuQuit;
};

#endif // WNDTRAYICON_H

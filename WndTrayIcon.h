#ifndef WNDTRAYICON_H
#define WNDTRAYICON_H

#include "common.h"
#include "WndMain.h"
#include <QTimer>
#include <QSystemTrayIcon>

class WndTrayIcon : public QSystemTrayIcon
{
public:
    WndTrayIcon(QApplication *app = Q_NULLPTR, WndMain *parent = Q_NULLPTR);
    ~WndTrayIcon();

Q_SIGNALS:
    void quitApp();

protected slots:
    void on_actived(QSystemTrayIcon::ActivationReason reason);
    void on_clicked();
    void on_actMenuQuit_trigger();
    void on_actMenuShowMain_trigger();

    QTimer m_tmClick;

    WndMain *m_wndMain;
    QApplication *m_app;
    QMenu *m_menuTray;
    QAction *m_actMenuShowMain;
    QAction *m_actMenuQuit;
};

#endif // WNDTRAYICON_H

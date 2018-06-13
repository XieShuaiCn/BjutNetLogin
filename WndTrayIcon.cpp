#include "WndTrayIcon.h"
#include <QMenu>
#include <QApplication>

WndTrayIcon::WndTrayIcon(QApplication *app,WndMain *parent):
    QSystemTrayIcon(parent),
    m_wndMain(parent),
    m_app(app)
{
    this->setToolTip("BJUT网关登录");
    QIcon ico(":/icon/logo.ico");
    this->setIcon(ico);

    m_menuTray = new QMenu();
    m_actMenuShowMain = new QAction("显示主窗口");
    m_actMenuQuit = new QAction("退出");
    m_menuTray->addAction(m_actMenuShowMain);
    m_menuTray->addSeparator();
    m_menuTray->addAction(m_actMenuQuit);
    this->setContextMenu(m_menuTray);

    //双击间隔200ms
    m_tmClick.setInterval(200);
    //一次触发
    m_tmClick.setSingleShot(true);

    connect(this, &WndTrayIcon::activated, this, &WndTrayIcon::on_actived);
    connect(m_actMenuShowMain, &QAction::triggered, this, &WndTrayIcon::on_actMenuShowMain_trigger);
    connect(m_actMenuQuit, &QAction::triggered, this, &WndTrayIcon::on_actMenuQuit_trigger);
    connect(&m_tmClick, &QTimer::timeout, this, &WndTrayIcon::on_clicked);
}

WndTrayIcon::~WndTrayIcon()
{
    delete m_menuTray;
}

void WndTrayIcon::on_clicked()
{
    this->showMessage("BJUT网关登录", m_wndMain->getStatus(), QSystemTrayIcon::NoIcon);
    m_tmClick.stop();
}

void WndTrayIcon::on_actMenuShowMain_trigger()
{
    if (m_wndMain != Q_NULLPTR)
    {
        m_wndMain->show();
    }
}

void WndTrayIcon::on_actMenuQuit_trigger()
{
    if(m_app != Q_NULLPTR)
    {
        m_app->quit();
    }
}

void WndTrayIcon::on_actived(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Context:
        m_menuTray->exec();
        break;
    case QSystemTrayIcon::DoubleClick:
        on_actMenuShowMain_trigger();
        break;
    case QSystemTrayIcon::Trigger:
        if(m_tmClick.isActive())
        {//双击
            m_tmClick.stop();
            on_actMenuShowMain_trigger();
        }
        else
        {//单击计时器
            m_tmClick.start();
        }
        break;
    default:
        break;
    }
}
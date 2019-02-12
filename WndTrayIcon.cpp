#include "WndTrayIcon.h"
#include "WndMain.h"
#include "WndSetting.h"
#include "BjutNet.h"
#include <QMenu>
#include <QApplication>
#include <QLocalSocket>

WndTrayIcon::WndTrayIcon(QApplication *app,QObject *parent):
    QSystemTrayIcon(parent),
    m_app(app),
    m_wndMain(Q_NULLPTR),
    m_wndSetting(Q_NULLPTR)
{
    this->setToolTip("BJUT网关登录");
    QIcon ico(":/icon/logo.ico");
    this->setIcon(ico);

    m_menuTray = new QMenu();
    m_actMenuShowMain = new QAction("显示主窗口");
    m_actMenuLogin = new QAction("上线");
    m_actMenuLogout = new QAction("下线");
    m_actMenuSetting = new QAction("设置");
    m_actMenuQuit = new QAction("退出");
    m_menuTray->addAction(m_actMenuShowMain);
    m_menuTray->addSeparator();
    m_menuTray->addAction(m_actMenuLogin);
    m_menuTray->addAction(m_actMenuLogout);
    m_menuTray->addSeparator();
    m_menuTray->addAction(m_actMenuSetting);
    m_menuTray->addAction(m_actMenuQuit);
    this->setContextMenu(m_menuTray);

    //双击间隔200ms
    m_tmClick.setInterval(200);
    //一次触发
    m_tmClick.setSingleShot(true);

    connect(this, &WndTrayIcon::activated, this, &WndTrayIcon::on_actived);
    connect(m_actMenuShowMain, &QAction::triggered, this, &WndTrayIcon::on_actMenuShowMain_trigger);
    connect(m_actMenuLogin, &QAction::triggered, this, &WndTrayIcon::on_actMenuLogin_trigger);
    connect(m_actMenuLogout, &QAction::triggered, this, &WndTrayIcon::on_actMenuLogout_trigger);
    connect(m_actMenuSetting, QAction::triggered, this, &WndTrayIcon::on_actMenuSetting_trigger);
    connect(m_actMenuQuit, &QAction::triggered, this, &WndTrayIcon::on_actMenuQuit_trigger);
    connect(&m_tmClick, &QTimer::timeout, this, &WndTrayIcon::on_clicked);

    m_net = new BjutNet();
    //启动网关监控
    if(m_net->loadAccount())
    {
        m_net->start_monitor();
    }
}

WndTrayIcon::~WndTrayIcon()
{
    if(m_net)
    {
        m_net->stop_monitor();
        m_net->deleteLater();
        m_net = Q_NULLPTR;
    }
    if(m_wndSetting)
    {
        m_wndSetting->close();
    }
    if(m_wndMain)
    {
        m_wndMain->close();
    }
    delete m_menuTray;
}

void WndTrayIcon::on_clicked()
{
    QString status;
    const char* flowUnit[] = {"KB", "MB", "GB", "TB"};
    int flowUnitIndex = 0;
    WebLgn &lgn = m_net->getWebLgn();
    float ftime = float(lgn.getTime()) / 60;
    float fflow = float(lgn.getFlow());
    while(fflow > 1024)
    {
        fflow /= 1024;
        ++flowUnitIndex;
    }
    float ffee = float(lgn.getFee()) / 100;
    status.sprintf("已用时间：%.2f小时，已用流量：%.2f%s，剩余金额：%.2f元", ftime, fflow, flowUnit[flowUnitIndex], ffee);
    //return status;
    this->showMessage("BJUT网关登录", status, QSystemTrayIcon::NoIcon);
    m_tmClick.stop();
}

void WndTrayIcon::on_actMenuShowMain_trigger()
{
    if (!m_wndMain)
    {
        m_wndMain = new WndMain(m_app, this);
    }
    m_wndMain->show();
}

void WndTrayIcon::on_actMenuSetting_trigger()
{
    if(!m_wndSetting)
    {
        m_wndSetting = new WndSetting();
    }
    m_wndSetting->show();
}

void WndTrayIcon::on_actMenuQuit_trigger()
{
    if(m_app != Q_NULLPTR)
    {
        m_app->quit();
    }
}

void WndTrayIcon::on_actMenuLogin_trigger()
{
    m_net->stop_monitor();
    m_net->start_monitor();
}

void WndTrayIcon::on_actMenuLogout_trigger()
{
    m_net->stop_monitor();
    m_net->getWebLgn().logout();
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

void WndTrayIcon::reciveMessage(const QString &msg)
{
    const char *m = msg.toStdString().data();
    if(0==strcmp(m, "ShowMainWnd")){
        on_actMenuShowMain_trigger();
    }
    else if(0==strcmp(m, "Exit")){
        on_actMenuQuit_trigger();
    }
}

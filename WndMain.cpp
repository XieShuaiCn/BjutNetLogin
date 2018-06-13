#include "WndMain.h"
#include "ui_mainwnd.h"
#include <QShowEvent>
#include <QMenu>
#include <QMessageBox>

WndMain::WndMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WndMain)
{
    //初始化界面
    ui->setupUi(this);
    //设置固定窗口大小
    setFixedSize(this->width(), this->height());
    //更新类型列表
    QStringList typelist;
    typelist.append("IPv4");
    typelist.append("IPv6");
    typelist.append("IPv4+IPv6");
    ui->cmbType->addItems(typelist);
    ui->cmbType->setCurrentIndex(2);
    //设置应用按钮菜单
    m_menuBtnApply = new QMenu(this);

    m_actMenuApplyOnly = new QAction("只保存");
    m_actMenuApplyOnly->setCheckable(true);
    m_menuBtnApply->addAction(m_actMenuApplyOnly);

    m_actMenuApplyLogin = new QAction("保存并上线");
    m_actMenuApplyLogin->setCheckable(true);
    m_actMenuApplyLogin->setChecked(true);
    m_bApplyLogin = true;
    m_menuBtnApply->addAction(m_actMenuApplyLogin);

    ui->btnApplyMenu->setMenu(m_menuBtnApply);
    //关联信号
    connect(ui->btnApply, &QPushButton::clicked, this, &WndMain::on_btnApply_click);
    connect(ui->btnLogout, &QPushButton::clicked, this, &WndMain::on_btnLogout_click);
    //connect(this, &WndMain::showed, this, &WndMain::on_show);
    connect(this, &WndMain::closeEvent, this, &WndMain::on_close);
    connect(m_actMenuApplyOnly, &QAction::triggered, this, &WndMain::on_actApplyOnly_triggered);
    connect(m_actMenuApplyLogin, &QAction::triggered, this, &WndMain::on_actApplyLogin_triggered);
    connect(&net, &BjutNet::message, this, &WndMain::on_txtMsg_message);
    connect(&net, &BjutNet::status_update, this, &WndMain::on_account_status);
    //
    ui->txtMsg->setFocus();
}

WndMain::~WndMain()
{
    net.stop_monitor();
    delete m_actMenuApplyLogin;
    delete m_actMenuApplyOnly;
    delete m_menuBtnApply;
    delete ui;
}

void WndMain::show()
{
    QWidget::show();
    //后台处理事件
    QCoreApplication::processEvents();
    on_show();
}

QString WndMain::getStatus()
{
    QString status;
    const char* flowUnit[] = {"KB", "MB", "GB", "TB"};
    int flowUnitIndex = 0;
    float ftime = float(net.getTime()) / 60;
    float fflow = float(net.getFlow());
    while(fflow > 1024)
    {
        fflow /= 1024;
        ++flowUnitIndex;
    }
    float ffee = float(net.getFee()) / 100;
    status.sprintf("已用时间：%.2f小时，已用流量：%.2f%s，剩余金额：%.2f元", ftime, fflow, flowUnit[flowUnitIndex], ffee);
    return status;
}

void WndMain::on_close(QCloseEvent *event)
{
    //QMessageBox::StandardButton ret = QMessageBox::information(this, "关闭", "是否关闭？", QMessageBox::Cancel, QMessageBox::Ok);
    //event->setAccepted(ret == QMessageBox::Ok);
    //取消关闭命令
    event->setAccepted(false);
    this->hide();
}

void WndMain:: on_show()
{
    net.load_account();
    ui->editAccount->setText(net.getAccount());
    ui->cmbType->setCurrentIndex(int(net.getType()));
    net.start_monitor();
    //QString msg;
    //net.check(msg);
    //net.login(msg);
}

void WndMain::on_account_status(bool login, int time, int flow, int fee)
{
    QString timeUnit[] = {"分钟", "小时"};
    QString flowUnit[] = {"KB", "MB", "GB", "TB"};
    //QString feeUnit[] = {"元"};
    if(time < 60)
    {
        ui->lcdNumTime->display(time);
        ui->lblTimeUnit->setText(timeUnit[0]);
    }
    else
    {
        ui->lcdNumTime->display(float(time) / 60);
        ui->lblTimeUnit->setText(timeUnit[1]);
    }
    int flowUnitIndex = 0;
    float fflow = flow;
    while(fflow > 1024)
    {
        fflow /= 1024;
        ++flowUnitIndex;
    }
    ui->lcdNumFlow->display(fflow);
    ui->lblFlowUnit->setText(flowUnit[flowUnitIndex]);
    ui->lcdNumFee->display(float(fee) / 100);
}

void WndMain::on_txtMsg_message(const QDateTime& time, const QString& info)
{
    ui->txtMsg->append(time.toString("yyyy-MM-dd hh:mm:ss  ") + (info.endsWith('\n') ? info : (info + "\r\n")));
}

void WndMain::on_btnApply_click()
{
    net.setAccount(ui->editAccount->text());
    net.setPassword(ui->editPassword->text());
    net.setType(ui->cmbType->currentIndex());
    if(net.save_account())
    {
        on_txtMsg_message(QDateTime::currentDateTime(), "账号信息保存成功");
    }
    if(m_bApplyLogin)
    {
        net.stop_monitor();
        net.start_monitor();
    }
}

void WndMain::on_btnLogout_click()
{
    net.stop_monitor();
    QString msg;
    net.logout(msg);
    on_txtMsg_message(QDateTime::currentDateTime(), msg);
}

void WndMain::on_actApplyOnly_triggered(bool checked)
{
    m_bApplyLogin = false;
    m_actMenuApplyOnly->setChecked(true);
    m_actMenuApplyLogin->setChecked(false);
}

void WndMain::on_actApplyLogin_triggered(bool checked)
{
    m_bApplyLogin = true;
    m_actMenuApplyOnly->setChecked(false);
    m_actMenuApplyLogin->setChecked(true);
}

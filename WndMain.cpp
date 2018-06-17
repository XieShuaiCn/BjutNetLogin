#include "WndMain.h"
#include <QShowEvent>
#include <QMenu>
#include <QMessageBox>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

WndMain::WndMain(QWidget *parent) :
    QWidget(parent)
{
    //初始化界面
    initUI();

    //关联信号
    connect(m_btnApply, &QPushButton::clicked, this, &WndMain::on_btnApply_click);
    connect(m_btnLogout, &QPushButton::clicked, this, &WndMain::on_btnLogout_click);
    connect(m_btnLogin, &QPushButton::clicked, this, &WndMain::on_btnLogin_click);
    //connect(this, &WndMain::showed, this, &WndMain::on_show);
    connect(this, &WndMain::closeEvent, this, &WndMain::on_close);
    connect(m_actMenuApplyOnly, &QAction::triggered, this, &WndMain::on_actApplyOnly_triggered);
    connect(m_actMenuApplyLogin, &QAction::triggered, this, &WndMain::on_actApplyLogin_triggered);
    connect(&m_net, &BjutNet::message, this, &WndMain::on_txtMsg_message);
    connect(&m_net, &BjutNet::status_update, this, &WndMain::on_account_status);
    //光标
    m_txtMsg->setFocus();
    //启动网关监控
    m_net.load_account();
    m_net.start_monitor();
}

WndMain::~WndMain()
{
    m_net.stop_monitor();
    delete m_actMenuApplyLogin;
    delete m_actMenuApplyOnly;
    delete m_menuBtnApply;
}

void WndMain::initUI()
{
    this->setObjectName(QStringLiteral("WndMain"));
    this->setWindowIcon(QIcon(":/icon/logo.ico"));
    //设置固定窗口大小
    this->setFixedSize(575, 297);
    m_lblInfoTime = new QLabel(this);
    m_lblInfoTime->setGeometry(QRect(20, 20, 100, 20));
    QFont font_s13;
    font_s13.setPointSize(13);
    m_lblInfoTime->setFont(font_s13);
    m_lblInfoFlow = new QLabel(this);
    m_lblInfoFlow->setGeometry(QRect(20, 60, 100, 20));
    m_lblInfoFlow->setFont(font_s13);
    m_lblInfoFee = new QLabel(this);
    m_lblInfoFee->setGeometry(QRect(20, 100, 100, 20));
    m_lblInfoFee->setFont(font_s13);
    m_lcdNumTime = new QLCDNumber(this);
    m_lcdNumTime->setGeometry(QRect(120, 20, 81, 23));
    m_lcdNumTime->setFrameShadow(QFrame::Plain);
    m_lcdNumTime->setLineWidth(1);
    m_lcdNumTime->setDigitCount(7);
    m_lcdNumTime->setSegmentStyle(QLCDNumber::Flat);
    m_lcdNumFlow = new QLCDNumber(this);
    m_lcdNumFlow->setGeometry(QRect(120, 60, 81, 23));
    m_lcdNumFlow->setFrameShadow(QFrame::Plain);
    m_lcdNumFlow->setLineWidth(1);
    m_lcdNumFlow->setDigitCount(7);
    m_lcdNumFlow->setSegmentStyle(QLCDNumber::Flat);
    m_lcdNumFee = new QLCDNumber(this);
    m_lcdNumFee->setGeometry(QRect(120, 100, 81, 23));
    m_lcdNumFee->setFrameShadow(QFrame::Plain);
    m_lcdNumFee->setLineWidth(1);
    m_lcdNumFee->setDigitCount(7);
    m_lcdNumFee->setSegmentStyle(QLCDNumber::Flat);
    m_lblAccount = new QLabel(this);
    m_lblAccount->setGeometry(QRect(270, 20, 50, 20));
    m_lblAccount->setFont(font_s13);
    m_lblPassword = new QLabel(this);
    m_lblPassword->setGeometry(QRect(270, 60, 50, 20));
    m_lblPassword->setFont(font_s13);
    m_lblType = new QLabel(this);
    m_lblType->setGeometry(QRect(270, 100, 50, 20));
    m_lblType->setFont(font_s13);
    m_editAccount = new QLineEdit(this);
    m_editAccount->setGeometry(QRect(330, 20, 120, 25));
    m_editAccount->setFont(font_s13);
    m_editPassword = new QLineEdit(this);
    m_editPassword->setGeometry(QRect(330, 60, 120, 25));
    m_editPassword->setFont(font_s13);
    m_editPassword->setEchoMode(QLineEdit::Password);
    //登录类型
    m_cmbType = new QComboBox(this);
    m_cmbType->setGeometry(QRect(330, 100, 120, 25));
    m_cmbType->setFont(font_s13);
    //更新类型列表
    QStringList typelist;
    typelist.append("IPv4");
    typelist.append("IPv6");
    typelist.append("IPv4+IPv6");
    m_cmbType->addItems(typelist);
    m_cmbType->setCurrentIndex(2);
    //设置应用菜单按钮
    m_btnApplyMenu = new QPushButton(this);
    m_btnApplyMenu->setGeometry(QRect(535, 100, 20, 30));
    //设置应用按钮菜单
    m_menuBtnApply = new QMenu(this);
    m_actMenuApplyOnly = new QAction("只保存");
    m_actMenuApplyOnly->setCheckable(true);
    m_actMenuApplyLogin = new QAction("保存并上线");
    m_actMenuApplyLogin->setCheckable(true);
    m_actMenuApplyLogin->setChecked(true);
    m_menuBtnApply->addAction(m_actMenuApplyOnly);
    m_menuBtnApply->addAction(m_actMenuApplyLogin);
    m_btnApplyMenu->setMenu(m_menuBtnApply);
    m_bApplyLogin = true;
    //应用按钮
    m_btnApply = new QPushButton(this);
    m_btnApply->setGeometry(QRect(470, 100, 70, 30));
    m_btnApply->setFlat(false);
    //上线按钮
    m_btnLogin = new QPushButton(this);
    m_btnLogin->setGeometry(QRect(470, 60, 85, 30));
    //下线按钮
    m_btnLogout = new QPushButton(this);
    m_btnLogout->setGeometry(QRect(470, 20, 85, 30));
    //消息日志框
    m_txtMsg = new QTextEdit(this);
    m_txtMsg->setGeometry(QRect(20, 140, 531, 131));
    m_txtMsg->setReadOnly(true);
    //时间单位
    m_lblTimeUnit = new QLabel(this);
    m_lblTimeUnit->setGeometry(QRect(210, 20, 40, 20));
    m_lblTimeUnit->setFont(font_s13);
    //流量单位
    m_lblFlowUnit = new QLabel(this);
    m_lblFlowUnit->setGeometry(QRect(210, 60, 40, 20));
    m_lblFlowUnit->setFont(font_s13);
    //金额单位
    m_lblFeeUnit = new QLabel(this);
    m_lblFeeUnit->setGeometry(QRect(210, 100, 40, 20));
    m_lblFeeUnit->setFont(font_s13);

    //设置各控件的文本
    this->setWindowTitle(QApplication::translate("WndMain", "BJUT\347\275\221\345\205\263\347\231\273\345\275\225", Q_NULLPTR));
    m_lblInfoTime->setText(QApplication::translate("WndMain", "\345\267\262\344\275\277\347\224\250\346\227\266\351\227\264\357\274\232", Q_NULLPTR));
    m_lblInfoFlow->setText(QApplication::translate("WndMain", "\345\267\262\344\275\277\347\224\250\346\265\201\351\207\217\357\274\232", Q_NULLPTR));
    m_lblInfoFee->setText(QApplication::translate("WndMain", "\345\211\251\344\275\231\351\207\221\351\242\235\357\274\232", Q_NULLPTR));
    m_lblAccount->setText(QApplication::translate("WndMain", "\350\264\246\345\217\267\357\274\232", Q_NULLPTR));
    m_lblPassword->setText(QApplication::translate("WndMain", "\345\257\206\347\240\201\357\274\232", Q_NULLPTR));
    m_lblType->setText(QApplication::translate("WndMain", "\347\261\273\345\236\213\357\274\232", Q_NULLPTR));
    m_btnApplyMenu->setText(QString());
    m_btnApply->setText(QApplication::translate("WndMain", "\345\272\224\347\224\250", Q_NULLPTR));
    m_btnLogin->setText(tr("上线"));
    m_btnLogout->setText(QApplication::translate("WndMain", "\344\270\213\347\272\277", Q_NULLPTR));
    m_lblTimeUnit->setText(QApplication::translate("WndMain", "\345\210\206\351\222\237", Q_NULLPTR));
    m_lblFlowUnit->setText(QApplication::translate("WndMain", "KB", Q_NULLPTR));
    m_lblFeeUnit->setText(QApplication::translate("WndMain", "\345\205\203", Q_NULLPTR));
    //根据objectName关联信号
    //QMetaObject::connectSlotsByName(this);
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
    float ftime = float(m_net.getTime()) / 60;
    float fflow = float(m_net.getFlow());
    while(fflow > 1024)
    {
        fflow /= 1024;
        ++flowUnitIndex;
    }
    float ffee = float(m_net.getFee()) / 100;
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
    m_editAccount->setText(m_net.getAccount());
    m_cmbType->setCurrentIndex(int(m_net.getLoginType())-1);
    //更新页面显示的流量状态
    on_account_status(m_net.getTime() > 0, m_net.getTime(), m_net.getFlow(), m_net.getFee());
    //QString msg;
    //m_net.check(msg);
}

void WndMain::on_account_status(bool login, int time, int flow, int fee)
{
    UNUSED(login);
    //窗口隐藏时，不更新
    if(this->isHidden())
    {
        return;
    }
    QString timeUnit[] = {"分钟", "小时"};
    QString flowUnit[] = {"KB", "MB", "GB", "TB"};
    //QString feeUnit[] = {"元"};
    if(time < 60)
    {
        m_lcdNumTime->display(time);
        m_lblTimeUnit->setText(timeUnit[0]);
    }
    else
    {
        m_lcdNumTime->display(float(time) / 60);
        m_lblTimeUnit->setText(timeUnit[1]);
    }
    int flowUnitIndex = 0;
    float fflow = flow;
    while(fflow > 1024)
    {
        fflow /= 1024;
        ++flowUnitIndex;
    }
    m_lcdNumFlow->display(fflow);
    m_lblFlowUnit->setText(flowUnit[flowUnitIndex]);
    m_lcdNumFee->display(float(fee) / 100);
}

void WndMain::on_txtMsg_message(const QDateTime& time, const QString& info)
{
    m_txtMsg->append(time.toString("yyyy-MM-dd hh:mm:ss  ") + (info.endsWith('\n') ? info : (info + '\n')));
}

void WndMain::on_btnApply_click()
{
    m_net.setAccount(m_editAccount->text());
    m_net.setPassword(m_editPassword->text());
    m_net.setLoginType(m_cmbType->currentIndex());
    if(m_net.save_account())
    {
        on_txtMsg_message(QDateTime::currentDateTime(), "账号信息保存成功");
    }
    if(m_bApplyLogin)
    {
        m_net.stop_monitor();
        QString msg;
        m_net.logout(msg);
        on_txtMsg_message(QDateTime::currentDateTime(), msg);
        msg.clear();
        m_net.login(msg);
        on_txtMsg_message(QDateTime::currentDateTime(), msg);
        m_net.start_monitor();
    }
}

void WndMain::on_btnLogout_click()
{
    m_net.stop_monitor();
    QString msg;
    m_net.logout(msg);
    on_txtMsg_message(QDateTime::currentDateTime(), msg);
}

void WndMain::on_btnLogin_click()
{
    m_net.start_monitor();
}

void WndMain::on_actApplyOnly_triggered(bool checked)
{
    UNUSED(checked);
    m_bApplyLogin = false;
    m_actMenuApplyOnly->setChecked(true);
    m_actMenuApplyLogin->setChecked(false);
}

void WndMain::on_actApplyLogin_triggered(bool checked)
{
    UNUSED(checked);
    m_bApplyLogin = true;
    m_actMenuApplyOnly->setChecked(false);
    m_actMenuApplyLogin->setChecked(true);
}

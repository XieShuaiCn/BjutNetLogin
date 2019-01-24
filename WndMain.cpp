#include "WndMain.h"
#include <QtGui/QPainter>
#include <QtGui/QShowEvent>
#include <QtGui/QDesktopServices>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
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
    QWidget(parent),
    m_bNeedUpdate(false)
{
    m_net = new BjutNet(this);
    m_net->setMainWindow(this);
    //初始化界面
    initUI();

    //关联信号
    connect(m_btnApply, &QPushButton::clicked, this, &WndMain::on_btnApply_clicked);
    connect(m_btnLogout, &QPushButton::clicked, this, &WndMain::on_btnLogout_clicked);
    connect(m_btnLogin, &QPushButton::clicked, this, &WndMain::on_btnLogin_clicked);
    connect(m_lblVersion, &HLabel::clicked, this, &WndMain::on_lblVersion_clicked);
    //connect(this, &WndMain::showed, this, &WndMain::on_show);
    connect(this, &WndMain::closeEvent, this, &WndMain::on_close);
    connect(this, &WndMain::paintEvent, this, &WndMain::on_paint);
    connect(m_actMenuApplyOnly, &QAction::triggered, this, &WndMain::on_actApplyOnly_triggered);
    connect(m_actMenuApplyLogin, &QAction::triggered, this, &WndMain::on_actApplyLogin_triggered);
    connect(m_net, &BjutNet::message, this, &WndMain::on_txtMsg_message);
    connect(&m_net->getWebLgn(), &WebLgn::status_update, this, &WndMain::on_account_status);
    //光标
    m_txtMsg->setFocus();
    //启动网关监控
    m_net->loadAccount();
    m_net->getWebLgn().start_monitor();
    m_net->getWebJfself().login();
    m_net->getWebJfself().refreshAccount();
}

WndMain::~WndMain()
{
    m_net->getWebLgn().stop_monitor();
    delete m_net;
    delete m_actMenuApplyLogin;
    delete m_actMenuApplyOnly;
    delete m_menuBtnApply;
}

void WndMain::initUI()
{
    QFont font_s10;
    font_s10.setPointSize(10);
    QFont font_s13;
    font_s13.setPointSize(13);
    //窗口
    this->setObjectName(QStringLiteral("WndMain"));
    this->setWindowIcon(QIcon(":/icon/logo.ico"));
    //设置固定窗口大小
    this->setFixedSize(575, 600);
    //窗口控件

    //状态图
    m_frmGraph = new QFrame(this);
    m_frmGraph->setGeometry(QRect(0, 0, 300, 150));
    m_frmGraph->setFrameShape(QFrame::StyledPanel);
    m_frmGraph->setFrameShadow(QFrame::Raised);
    m_lblService = new QLabel(m_frmGraph);
    m_lblService->setGeometry(QRect(200, 120, 100, 20));
    m_lblService->setFont(font_s13);
    //账户状态
    m_frmInfo = new QFrame(this);
    m_frmInfo->setGeometry(QRect(300, 0, 250, 150));
    m_frmInfo->setFrameShape(QFrame::StyledPanel);
    m_frmInfo->setFrameShadow(QFrame::Raised);
    //状态label
    m_lblInfoTime = new QLabel(m_frmInfo);
    m_lblInfoTime->setGeometry(QRect(20, 20, 100, 20));
    m_lblInfoTime->setFont(font_s13);
    m_lblInfoFlow = new QLabel(m_frmInfo);
    m_lblInfoFlow->setGeometry(QRect(20, 60, 100, 20));
    m_lblInfoFlow->setFont(font_s13);
    m_lblInfoFee = new QLabel(m_frmInfo);
    m_lblInfoFee->setGeometry(QRect(20, 100, 100, 20));
    m_lblInfoFee->setFont(font_s13);
    //状态value
    m_lcdNumTime = new QLCDNumber(m_frmInfo);
    m_lcdNumTime->setGeometry(QRect(120, 20, 81, 23));
    m_lcdNumTime->setFrameShadow(QFrame::Plain);
    m_lcdNumTime->setLineWidth(1);
    m_lcdNumTime->setDigitCount(7);
    m_lcdNumTime->setSegmentStyle(QLCDNumber::Flat);
    m_lcdNumFlow = new QLCDNumber(m_frmInfo);
    m_lcdNumFlow->setGeometry(QRect(120, 60, 81, 23));
    m_lcdNumFlow->setFrameShadow(QFrame::Plain);
    m_lcdNumFlow->setLineWidth(1);
    m_lcdNumFlow->setDigitCount(7);
    m_lcdNumFlow->setSegmentStyle(QLCDNumber::Flat);
    m_lcdNumFee = new QLCDNumber(m_frmInfo);
    m_lcdNumFee->setGeometry(QRect(120, 100, 81, 23));
    m_lcdNumFee->setFrameShadow(QFrame::Plain);
    m_lcdNumFee->setLineWidth(1);
    m_lcdNumFee->setDigitCount(7);
    m_lcdNumFee->setSegmentStyle(QLCDNumber::Flat);
    //状态unit
    m_lblTimeUnit = new QLabel(m_frmInfo);
    m_lblTimeUnit->setGeometry(QRect(210, 20, 40, 20));
    m_lblTimeUnit->setFont(font_s13);
    m_lblFlowUnit = new QLabel(m_frmInfo);
    m_lblFlowUnit->setGeometry(QRect(210, 60, 40, 20));
    m_lblFlowUnit->setFont(font_s13);
    m_lblFeeUnit = new QLabel(m_frmInfo);
    m_lblFeeUnit->setGeometry(QRect(210, 100, 40, 20));
    m_lblFeeUnit->setFont(font_s13);
    //在线列表
    m_frmOnline = new QFrame(this);
    m_frmOnline->setGeometry(QRect(0, 150, 250, 150));
    m_frmOnline->setFrameShape(QFrame::StyledPanel);
    m_frmOnline->setFrameShadow(QFrame::Raised);
    //账号信息
    m_frmAccount = new QFrame(this);
    m_frmAccount->setGeometry(QRect(250, 150, 310, 150));
    m_frmAccount->setFrameShape(QFrame::StyledPanel);
    m_frmAccount->setFrameShadow(QFrame::Raised);
    m_lblAccount = new QLabel(m_frmAccount);
    m_lblAccount->setGeometry(QRect(20, 10, 50, 20));
    m_lblAccount->setFont(font_s13);
    m_lblPassword = new QLabel(m_frmAccount);
    m_lblPassword->setGeometry(QRect(20, 50, 50, 20));
    m_lblPassword->setFont(font_s13);
    m_lblType = new QLabel(m_frmAccount);
    m_lblType->setGeometry(QRect(20, 90, 50, 20));
    m_lblType->setFont(font_s13);
    m_editAccount = new QLineEdit(m_frmAccount);
    m_editAccount->setGeometry(QRect(80, 10, 120, 25));
    m_editAccount->setFont(font_s13);
    m_editPassword = new QLineEdit(m_frmAccount);
    m_editPassword->setGeometry(QRect(80, 50, 120, 25));
    m_editPassword->setFont(font_s13);
    m_editPassword->setEchoMode(QLineEdit::Password);
    //登录类型
    m_cmbType = new QComboBox(m_frmAccount);
    m_cmbType->setGeometry(QRect(80, 90, 120, 25));
    m_cmbType->setFont(font_s13);
    //更新类型列表
    QStringList typelist;
    typelist.append("IPv4");
    typelist.append("IPv6");
    typelist.append("IPv4+IPv6");
    m_cmbType->addItems(typelist);
    m_cmbType->setCurrentIndex(2);
    //设置应用菜单按钮
    m_btnApplyMenu = new QPushButton(m_frmAccount);
    m_btnApplyMenu->setGeometry(QRect(285, 90, 20, 30));
    //设置应用按钮菜单
    m_menuBtnApply = new QMenu(m_frmAccount);
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
    m_btnApply = new QPushButton(m_frmAccount);
    m_btnApply->setGeometry(QRect(220, 90, 70, 30));
    m_btnApply->setFlat(false);
    //上线按钮
    m_btnLogin = new QPushButton(m_frmAccount);
    m_btnLogin->setGeometry(QRect(220, 50, 85, 30));
    //下线按钮
    m_btnLogout = new QPushButton(m_frmAccount);
    m_btnLogout->setGeometry(QRect(220, 10, 85, 30));
    //消息日志框
    m_txtMsg = new QTextEdit(this);
    m_txtMsg->setGeometry(QRect(20, 300, 531, 131));
    m_txtMsg->setReadOnly(true);
    //版本号
    m_lblVersion = new HLabel(this);
    m_lblVersion->setGeometry(QRect(10, this->height()-25, 250, 20));
    m_lblVersion->setFont(font_s10);

    //设置各控件的文本
    this->setWindowTitle(QString("BJUT网关登录器"));
    m_lblInfoTime->setText(QString("已用时间："));
    m_lblInfoFlow->setText(QString("已用流量："));
    m_lblInfoFee->setText(QString("剩余金额："));
    m_lblAccount->setText(QString("账号："));
    m_lblPassword->setText(QString("密码："));
    m_lblType->setText(QString("类型"));
    m_btnApplyMenu->setText(QString());
    m_btnApply->setText(QString("应用"));
    m_btnLogin->setText(QString("上线"));
    m_btnLogout->setText(QString("下线"));
    m_lblTimeUnit->setText(QString("时"));
    m_lblFlowUnit->setText(QString("KB"));
    m_lblFeeUnit->setText(QString("元"));
    m_lblVersion->setText(QString("版本号：") + m_updater.getOldVersion());
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

void WndMain::on_paint(QPaintEvent *event)
{
    //Q_UNUSED(event);
    QPainter painter(this);
    // 设置反锯齿
    painter.setRenderHint(QPainter::Antialiasing);
    //饼图外接矩形框
    QRect rectCircle(65, 20, 120, 120);
//    QFile f("flow.txt");
//    f.open(QIODevice::ReadOnly);
//    auto data = f.readAll();
//    f.close();
//    int currentFlow = QString(data).toInt();
    int currentFlow = m_net->getWebLgn().getFlow() / 1024;//MB
    int totalFlow = m_net->getWebJfself().getTotalFlow();//MB
    QBrush brushPie(QColor(60,180,60));
    int angleStart = 90;
    int angleLength = 0;
    if(totalFlow > 0)
    {
        //绘制底色
        double flowRate = 1.0 * currentFlow / totalFlow;
        angleLength = 360 * flowRate;
        if(angleLength >= 360){
            angleLength = 360;//
            brushPie.setColor(QColor(240, 60, 60));
        }
        else {
            painter.setBrush(QBrush(QColor(220,220,220)));
            painter.setPen(QPen(QColor(200,200,200)));
            painter.drawEllipse(rectCircle);
            if(angleLength < 180){}
            else if(angleLength < 270)//流量大于一半，变红警示
            {
                brushPie.setColor(QColor(int(75+180*(flowRate-0.5)/0.25),
                                         180,
                                         40));
            }
            else if(angleLength < 361)//流量大于一半，变红警示
            {
                brushPie.setColor(QColor(255,
                                         int(180-140*(flowRate-0.75)/0.25),
                                         40));
            }
        }
    }
    painter.setBrush(brushPie);
    painter.setPen(Qt::NoPen);
    painter.drawPie(rectCircle, angleStart*16, -angleLength*16);
    event->accept();
}

void WndMain:: on_show()
{
    WebLgn &lgn = m_net->getWebLgn();
    WebJfself &jfself = m_net->getWebJfself();
    m_lblService->setText(jfself.getAccountStatus());
    m_editAccount->setText(lgn.getAccount());
    m_cmbType->setCurrentIndex(int(lgn.getLoginType())-1);
    //更新页面显示的流量状态
    on_account_status(lgn.getTime() > 0, lgn.getTime(), lgn.getFlow(), lgn.getFee());
    m_updater.checkUpdate();
    if(m_updater.needUpdate()){
        m_bNeedUpdate = true;
        QString ver("%1 <font color=#dd3333>最新版本：%2 点我更新！</font>");
        m_lblVersion->setText(ver.arg(m_updater.getOldVersion(), m_updater.getNewVersion()));
    }
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

void WndMain::on_btnApply_clicked()
{
    WebLgn &lgn = m_net->getWebLgn();
    m_net->setAccount(m_editAccount->text());
    m_net->setPassword(m_editPassword->text());
    m_net->setLoginType(m_cmbType->currentIndex() + 1);
    if(m_net->saveAccount())
    {
        on_txtMsg_message(QDateTime::currentDateTime(), "账号信息保存成功");
    }
    if(m_bApplyLogin)
    {
        if(!m_net->getWebLgn().stop_monitor())
        {
            on_txtMsg_message(QDateTime::currentDateTime(), "在线保持停止失败");
        }
        QString msg;
        if(lgn.logout(msg))
        {
            on_txtMsg_message(QDateTime::currentDateTime(), "账号下线失败");
        }
        on_txtMsg_message(QDateTime::currentDateTime(), msg);
        msg.clear();
        if(lgn.login(msg))
        {
            on_txtMsg_message(QDateTime::currentDateTime(), "账号上线失败");
        }
        on_txtMsg_message(QDateTime::currentDateTime(), msg);
        if(!m_net->getWebLgn().start_monitor())
        {
            on_txtMsg_message(QDateTime::currentDateTime(), "在线保持启动失败");
        }
    }
}

void WndMain::on_btnLogout_clicked()
{
    m_net->getWebLgn().stop_monitor();
    QString msg;
    m_net->getWebLgn().logout(msg);
    on_txtMsg_message(QDateTime::currentDateTime(), msg);
}

void WndMain::on_btnLogin_clicked()
{
    m_net->getWebLgn().stop_monitor();
    m_net->getWebLgn().start_monitor();
}

void WndMain::on_lblVersion_clicked()
{
    QMessageBox mb(this);
    if(m_bNeedUpdate) {
        QString msg = QString("新版本更新：\n版本号：%1\n%2")
                .arg(m_updater.getNewVersion())
                .arg(m_updater.getNewIntroduction());
        mb.setWindowTitle("立即更新");
        mb.setText(msg);
        mb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        if(QMessageBox::Yes == mb.exec()) {
            QDesktopServices::openUrl(QUrl(QLatin1String("http://bnl.hrrcn.com/")));
//            m_dlgProgress = new QProgressDialog("正在下载....", "无法取消", 0, 101, this);
//            m_dlgProgress->setWindowModality(Qt::WindowModal);
//            m_dlgProgress->show();
//            connect(&m_updater, &Updater::downloadProgress, this, &WndMain::downloadProgress);
//            if(m_updater.doDownloadAndInstall()) {
//                mb.setWindowTitle("更新成功");
//                mb.setText("软件更新成功，重启生效。");
//            } else {
//                mb.setWindowTitle("更新失败");
//                mb.setText("软件更新失败，请稍候重试。");
//            }
//            disconnect(&m_updater, nullptr, nullptr, nullptr);
//            mb.setStandardButtons(QMessageBox::Ok);
//            mb.exec();
        }
    } else {
        mb.setWindowTitle("已是最新版本");
        QString status("未检测到更新。当前已是最新版本(%1)。");
        mb.setText(status.arg(m_updater.getNewVersion()));
        mb.setStandardButtons(QMessageBox::Ok);
        mb.exec();
    }
}

void WndMain::on_actApplyOnly_triggered(bool checked)
{
    Q_UNUSED(checked);
    m_bApplyLogin = false;
    m_actMenuApplyOnly->setChecked(true);
    m_actMenuApplyLogin->setChecked(false);
}

void WndMain::on_actApplyLogin_triggered(bool checked)
{
    Q_UNUSED(checked);
    m_bApplyLogin = true;
    m_actMenuApplyOnly->setChecked(false);
    m_actMenuApplyLogin->setChecked(true);
}

void WndMain::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if(m_dlgProgress) {
        m_dlgProgress->setValue(100.0 * bytesReceived / bytesTotal);
    }
}

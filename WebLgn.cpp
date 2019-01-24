#include "WebLgn.h"
#include <QFile>
#include <QDir>
#include <QRegExp>
#include <QJsonObject>
#include <QTextCodec>
#include <QEventLoop>
#include <QJsonDocument>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QSslConfiguration>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QHostInfo>


WebLgn::WebLgn() :
    QThread(),
    m_loginType(IPv4),
    m_netType(UnknownNet),
    m_nTime(0.0f),
    m_nFlow(0.0f),
    m_nFee(0.0f)
{
    m_bRun = false;
    m_http.setCodec("GBK");
}


bool WebLgn::login(QString& msg)
{
    if(m_strAccount.length() == 0)
    {
        msg.append("Your account has not been setted.");
        return false;
    }

    QString test =  m_http.getUrlHtml(QUrl("http://lgn.bjut.edu.cn"));
    if(test.size() == 0)
    {
        test =  m_http.getUrlHtml(QUrl("http://wlgn.bjut.edu.cn/0.htm"));
        if (test.size() == 0)
        {
            m_netType = UnknownNet;
            msg.append("当前无法访问校园网，请稍候重试。\n");
            return false;
        }
        else
        {
            m_netType = BJUT_WIFI;
            return loginOnWIFI(msg);
        }
    }
    else
    {
        m_netType = BJUT_LAN;
        return loginOnLAN(msg);
    }
    msg.append("Program should not reach here.");
    return false;
}

bool WebLgn::loginOnLAN(QString &msg, LoginType type)
{
    QString url;
    int v46s = 0;
    bool ipv4_login = false;
    bool ipv6_login = false;

    if(type == AutoLoginType)
    {
        type = this->m_loginType;
    }

    switch (type) {
    case AutoLoginType:
    case IPv4:
        v46s = 1;
        url = "https://lgn.bjut.edu.cn";
        if(checkLoginStatus(msg, IPv4))
        {
            return true;
        }
        break;
    case IPv6:
        v46s = 2;
        url = "https://lgn6.bjut.edu.cn";
        if(checkLoginStatus(msg, IPv6))
        {
            return true;
        }
        break;
    case IPv4_6:
        v46s = 0;
        url = "https://lgn6.bjut.edu.cn/V6?https://lgn.bjut.edu.cn";
        ipv4_login = checkLoginStatus(msg, IPv4);
        ipv6_login = checkLoginStatus(msg, IPv6);
        if(!ipv4_login && ipv6_login)
        {
            return loginOnLAN(msg, IPv4);
        }
        if(ipv4_login && !ipv6_login)
        {
            return loginOnLAN(msg, IPv6);
        }
        break;
    default:
        msg.append("无法识别的登录类型\n");
        return false;
    }

    // 设置发送的数据
    QMap<QString, QString> data;
    data.insert("DDDDD", m_strAccount);
    data.insert("upass", m_strPassword);
    data.insert("v46s", QString::number(v46s));
    data.insert("v6ip", "");
    data.insert("f4serip", "172.30.201.10");
    data.insert("0MKKey", "");
    QString content = m_http.postUrlHtml(QUrl(url), data);
    //若返回跳转网页，继续解析跳转网页
    QRegExp regKeyValue("name='?(\\w*)'? value='([:\\w]*)'",  Qt::CaseInsensitive);
    int pos = 0;
    if(0 <= (pos = content.indexOf(regKeyValue)))
    {
        url = "https://lgn.bjut.edu.cn";
        data.clear();
        do
        {
            data.insert(regKeyValue.cap(1), regKeyValue.cap(2));
            ++pos;
            pos = content.indexOf(regKeyValue, pos);
        }while (pos >= 0);
        content = m_http.postUrlHtml(QUrl(url), data);
    }
    if(content.contains("<title>登录成功窗</title>"))
    {
        msg.append("登录成功\n");
        return true;
    }
    if(content.contains("<title>信息返回窗</title>"))
    {
        QRegExp regMsg("Msg=(\\d+);", Qt::CaseInsensitive);
        pos = content.indexOf(regMsg);
        if(pos < 0)
        {
            msg.append("未检测到返回消息\n");
            emit status_update(false, m_nTime, m_nFlow, m_nFee);
            return false;
        }
        QString html_msg = regMsg.cap(1);
        int msgID = html_msg.toInt();
        QRegExp regMsga("msga='(\\w+?)';", Qt::CaseInsensitive);
        pos = content.indexOf(regMsga);
        QString html_msga;
        if(pos > 0)
        {
            html_msga = regMsga.cap(1);
        }
        QString msgIDstr = convertMsg(msgID, html_msga);
        msg.append(msgIDstr);
        return msgID > 10;
    }
    return false;
}

bool WebLgn::loginOnWIFI(QString &msg, LoginType type)
{
    QString url;
    bool ipv6_succ = true;

    if(type == AutoLoginType)
    {
        type = this->m_loginType;
    }

    switch (type) {
    case IPv4:
        url = "https://wlgn.bjut.edu.cn";
        break;
    case IPv6://IPv6不区分有线还是无线
        return loginOnLAN(msg, IPv6);
        break;
    case IPv4_6:
        ipv6_succ = loginOnLAN(msg, IPv6);
        url = "https://wlgn.bjut.edu.cn";
        break;
    default:
        msg.append("无法识别的登录类型");
        return false;
    }

    // 设置发送的数据
    QMap<QString, QString> data;
    data.insert("DDDDD", m_strAccount);
    data.insert("upass", m_strPassword);
    data.insert("6MKKey", "1");
    QString content = m_http.postUrlHtml(QUrl(url), data);
    content = m_http.getUrlHtml(QUrl("https://wlgn.bjut.edu.cn/1.htm"));
    if(content.size() > 0)
    {
        if(content.indexOf("You have successfully logged in"))
        {
            msg.append("IPv4登录成功");
            return ipv6_succ;
        }
    }
    return false;
}

bool WebLgn::logout(QString &msg)
{
    return BJUT_WIFI == m_netType ? logoutOnWIFI(msg) : logoutOnLAN(msg);
}

bool WebLgn::logoutOnLAN(QString &msg, LoginType type)
{
    if(type == AutoLoginType)
    {
        type = this->m_loginType;
    }

    QUrl url6("http://lgn6.bjut.edu.cn/F.htm");
    QUrl url4("http://lgn.bjut.edu.cn/F.htm");
    QString content = m_http.getUrlHtml(IPv6 == type ? url6 : url4);
    QRegExp regMsg("Msg=(\\d+);", Qt::CaseInsensitive);
    QRegExp regTime("time='(\\d+) *';", Qt::CaseInsensitive);
    QRegExp regFlow("flow='(\\d+) *';", Qt::CaseInsensitive);
    QRegExp regFee("fee='(\\d+) *';", Qt::CaseInsensitive);
    int pos = content.indexOf(regMsg);
    if(pos < 0)
    {
        msg.append("未检测到返回消息。\n");
        emit status_update(false, m_nTime, m_nFlow, m_nFee);
        return false;
    }
    QString html_msg = regMsg.cap(1);
    int msgID = html_msg.toInt();
    pos = content.indexOf(regTime);
    if(pos < 0)
    {
        msg.append("未检测到时间。\n");
        QString html_time = regTime.cap(1);
        m_nTime = html_time.toInt();
    }
    pos = content.indexOf(regFlow);
    if(pos < 0)
    {
        msg.append("未检测到流量。\n");
        QString html_flow = regFlow.cap(1);
        m_nFlow = html_flow.toInt();
    }
    pos = content.indexOf(regFee);
    if(pos < 0)
    {
        msg.append("未检测到费用。\n");
        QString html_fee = regFee.cap(1);
        m_nFee = html_fee.toInt() / 100;
    }
    msg.sprintf("已用时间：%.3f小时，已用流量：%.3fMB，剩余金额：%.2f元。\n",
                float(m_nTime) / 60, float(m_nFlow) / 1024, float(m_nFee) / 100);
    emit status_update(false, m_nTime, m_nFlow, m_nFee);
    msg.append(msgID == 14 ? "注销成功" : "注销失败");
    return msgID == 14;
}

bool WebLgn::logoutOnWIFI(QString &msg, LoginType type)
{
    bool ipv6_succ = true;
    if(type == AutoLoginType)
    {
        type = this->m_loginType;
    }

    if(type == IPv4_6)
    {
        ipv6_succ = logoutOnLAN(msg, IPv6);
    }
    else if(type == IPv6)
    {
        return logoutOnLAN(msg, IPv6);
    }

    QUrl url4("http://Wlgn.bjut.edu.cn/F.htm");
    QString content = m_http.getUrlHtml(url4);
    QRegExp regMsg("Msg=(\\d+);", Qt::CaseInsensitive);
    QRegExp regTime("time='(\\d+) *';", Qt::CaseInsensitive);
    QRegExp regFlow("flow='(\\d+) *';", Qt::CaseInsensitive);
    QRegExp regFee("fee='(\\d+) *';", Qt::CaseInsensitive);
    int pos = content.indexOf(regMsg);
    if(pos < 0)
    {
        msg.append("未检测到返回消息。\n");
        emit status_update(false, m_nTime, m_nFlow, m_nFee);
        return false;
    }
    QString html_msg = regMsg.cap(1);
    int msgID = html_msg.toInt();
    pos = content.indexOf(regTime);
    if(pos < 0)
    {
        msg.append("未检测到时间。\n");
        QString html_time = regTime.cap(1);
        m_nTime = html_time.toInt();
    }
    pos = content.indexOf(regFlow);
    if(pos < 0)
    {
        msg.append("未检测到流量。\n");
        QString html_flow = regFlow.cap(1);
        m_nFlow = html_flow.toInt();
    }
    pos = content.indexOf(regFee);
    if(pos < 0)
    {
        msg.append("未检测到费用。\n");
        QString html_fee = regFee.cap(1);
        m_nFee = html_fee.toInt() / 100;
    }
    msg.sprintf("已用时间：%.3f小时，已用流量：%.3fMB，剩余金额：%.2f元。\n",
                float(m_nTime) / 60, float(m_nFlow) / 1024, float(m_nFee) / 100);
    emit status_update(false, m_nTime, m_nFlow, m_nFee);
    msg.append(msgID == 14 ? "注销成功" : "注销失败");
    return ipv6_succ && msgID == 14;
}

bool WebLgn::checkLoginStatus(QString& msg, LoginType type)
{
    msg.clear();
    QUrl url6("http://lgn6.bjut.edu.cn/");
    QUrl url4("http://lgn.bjut.edu.cn/");
    QUrl url4_wifi("http://wlgn.bjut.edu.cn/1.html");

    //解析数据
    QRegExp regTime("time='(\\d+) *';", Qt::CaseInsensitive);
    QRegExp regFlow("flow='(\\d+) *';", Qt::CaseInsensitive);
    QRegExp regFee("fee='(\\d+) *';", Qt::CaseInsensitive);

    if(AutoLoginType == type)
    {
        type = this->m_loginType;
    }

    //分析网页
    if(type == IPv4_6)
    {
        QString content;
        if(200 == m_http.getUrlHtml(url6, content))
        {
            m_netType = BJUT_LAN;
        }
        else
        {
            m_netType = UnknownNet;
            return false;
        }
        int pos = content.indexOf(regTime);
        if(pos < 0)
        {
            msg.append("没有登录网关6，未检测到时间\n");
            m_isOnline = false;
            emit status_update(false, m_nTime, m_nFlow, m_nFee);
            return false;
        }
        if(200 == m_http.getUrlHtml(url4, content))
        {
            m_netType = BJUT_LAN;
        }
        else
        {
            m_netType = UnknownNet;
            return false;
        }
        pos = content.indexOf(regTime);
        if(pos < 0)
        {
            msg.append("没有登录网关4，未检测到时间\n");
            m_isOnline = false;
            emit status_update(false, m_nTime, m_nFlow, m_nFee);
            return false;
        }
        pos = content.indexOf(regFlow);
        if(pos < 0)
        {
            msg.append("没有登录网关4，未检测到流量\n");
            m_isOnline = false;
            emit status_update(false, m_nTime, m_nFlow, m_nFee);
            return false;
        }
        pos = content.indexOf(regFee);
        if(pos < 0)
        {
            msg.append("没有登录网关4，未检测到费用\n");
            m_isOnline = false;
            emit status_update(false, m_nTime, m_nFlow, m_nFee);
            return false;
        }
    }
    else
    {
        QString content;
        if(200 == m_http.getUrlHtml(url4, content))
        {
            m_netType = BJUT_LAN;
        }
        else
        {
            m_netType = UnknownNet;
            return false;
        }
        if(IPv6 == type)
        {
            content = m_http.getUrlHtml(url6);
        }
        QString errmsg(tr("没有登录网关%1，未检测到%2\n"));
#ifdef QT_DEBUG
        qDebug() << content << "\n";
#endif
        int pos = content.indexOf(regTime);
        if(pos < 0)
        {
            msg.append(errmsg.arg(IPv4 == type ? 4 : 6).arg("时间"));
            m_isOnline = false;
            emit status_update(false, m_nTime, m_nFlow, m_nFee);
            return false;
        }
        pos = content.indexOf(regFlow);
        if(pos < 0)
        {
            msg.append(errmsg.arg(IPv4 == type ? 4 : 6).arg("流量"));
            m_isOnline = false;
            emit status_update(false, m_nTime, m_nFlow, m_nFee);
            return false;
        }
        pos = content.indexOf(regFee);
        if(pos < 0)
        {
            msg.append(errmsg.arg(IPv4 == type ? 4 : 6).arg("费用"));
            m_isOnline = false;
            emit status_update(false, m_nTime, m_nFlow, m_nFee);
            return false;
        }
    }
    QString html_time = regTime.cap(1);
    QString html_flow = regFlow.cap(1);
    QString html_fee = regFee.cap(1);
    m_nTime = html_time.toInt();
    m_nFlow = html_flow.toInt();
    m_nFee = html_fee.toInt() / 100;
    msg.sprintf("已用时间：%.3f小时，已用流量：%.3fMB，剩余金额：%.2f元\n",
                float(m_nTime) / 60, float(m_nFlow) / 1024, float(m_nFee) / 100);
    m_isOnline = true;
    emit status_update(true, m_nTime, m_nFlow, m_nFee);

    return true;
}

bool WebLgn::checkNetStatus(QString &msg)
{
    UNUSED(msg);
//    int id = QHostInfo::lookupHost("www.bjut.edu.cn", nullptr, nullptr);
//    QHostInfo info(id);
//    return QHostInfo::NoError == info.error();
    return m_http.getUrlHtml(QUrl("http://www.bjut.edu.cn/404.html")).size() > 0;
}

bool WebLgn::start_monitor()
{
    m_bRun = true;
    if(!this->isRunning())
    {    //启动线程
        this->start(IdlePriority);
        connect(&m_netCfgMan, &QNetworkConfigurationManager::onlineStateChanged, this, &WebLgn::online_status_change);
    }
    return true;
}

bool WebLgn::stop_monitor()
{
    m_bRun = false;
    if(this->isRunning())
    {
        //this->quit();
        this->wait();
        //this->terminate();
        disconnect(&m_netCfgMan, &QNetworkConfigurationManager::onlineStateChanged, this, &WebLgn::online_status_change);
    }
    return true;
}

QString WebLgn::convertMsg(int msg, QString msga)
{
    QString str;
    switch(msg)
    {
    case 1:
        if (msga.length() > 0)
        {
            if (msga == "error0")
                str = "本IP不允许Web方式登录";
            else if (msga == "error1")
                str = "本账号不允许Web方式登录";
            else if (msga == "error2")
                str = "本账号不允许修改密码";
            else
                str = msga;

        }
        else
        {
            str = "账号或密码不对，请重新输入";
        }
        break;
    case 2:
        str = "该账号正在使用中，请您与网管联系";
        break;
    case 3:
        str = "本账号只能在指定地址使用";
        break;
    case 4:
        str = "本账号费用超支或时长流量超过限制";
        break;
    case 5:
        str = "本账号暂停使用";
        break;
    case 6:
        str = "系统缓存已满";
        break;
    case 7:
        str = "";
        break;
    case 8:
        str = "本账号正在使用,不能修改";
        break;
    case 9:
        str = "新密码与确认新密码不匹配,不能修改";
        break;
    case 10:
        str = "密码修改成功";
        break;
    case 11:
        str = "本账号只能在指定地址使用";
        break;
    case 14:
        str = "注销成功";
        break;
    case 15:
        str = "登录成功";
        break;
    default:
        str = "未知的消息内容";
    }
    return str;
}

void WebLgn::online_status_change(bool online)
{
    if(online)
    {
        QString msg;
        bool ret = this->checkLoginStatus(msg);
        emit message(QDateTime::currentDateTime(), msg);
        if(!ret)
        {
            msg.clear();
            this->login(msg);
            emit message(QDateTime::currentDateTime(), msg);
        }
    }
}

void WebLgn::run()
{
    bool is_login = false;
    QDateTime time;
    bool suc = false;
    int sleepsec = 1;
    int slept= 0;
    QString msg;
    while(m_bRun)
    {
        if(slept < sleepsec)
        {
            this->sleep(1);
            ++slept;
            if(slept < sleepsec)
            {
                continue;
            }
            else {
                slept = 0;
            }
        }
        msg.clear();
        suc = this->checkLoginStatus(msg);
        time = QDateTime::currentDateTime();
        if (suc)
        {
            //首次登录时或间隔一段时间输出信息
            if (!is_login)
            {
                emit message(time, msg);
            }
            is_login = true;
            sleepsec = 30;
        }
        else if(UnknownNet == m_netType)//非校园网
        {
            //sleepsec = 1;
            if(sleepsec < 1024)//最大17分钟试一次
            {
                sleepsec *= 2;
            }
        }
        else//校园网未登录
        {
            //登录
            is_login = false;
            emit message(time, msg);
            msg.clear();
            suc = this->login(msg);
            time = QDateTime::currentDateTime();
            emit message(time, msg);
            if (suc)
            {
                sleepsec = 1;
            }
            else
            {
                //this->sleep(sleepsec);
                if(sleepsec < 1024)//最大17分钟试一次
                {
                    sleepsec *= 2;
                }
            }
        }
    }
}

#ifndef BJUTNET_H
#define BJUTNET_H

#include "common.h"
#include <QThread>
#include <QTime>
#include <QtNetwork/QNetworkConfigurationManager>

class BjutNet : public QThread
{
    Q_OBJECT

public:
    //登录类型
    enum LoginType{
        AutoLoginType = 0, IPv4 = 1, IPv6 = 2, IPv4_6 = 3
    };
    enum NetType{
        UnknownNet = 0, BJUT_LAN = 1, BJUT_WIFI = 2, FREE_NET = 3
    };
    //构造函数
    explicit BjutNet();
    //登录网关
    bool login(QString &msg);
    bool loginOnLAN(QString &msg, LoginType type = AutoLoginType);
    bool loginOnWIFI(QString &msg, LoginType type = AutoLoginType);
    //注销网关
    bool logout(QString &msg);
    bool logoutOnLAN(QString &msg, LoginType type = AutoLoginType);
    bool logoutOnWIFI(QString &msg, LoginType type = AutoLoginType);
    //检测网关状态
    bool checkLoginStatus(QString &msg, LoginType type = AutoLoginType);
    //检测网络是否畅通
    bool checkNetStatus(QString &msg);
    //启动监视器
    bool start_monitor();
    //停止监视器
    bool stop_monitor();
    //加载账号信息
    bool load_account(const QString path = "");
    //保存账号信息
    bool save_account(const QString path = "");
    int getTime()
    {
        return m_nTime;
    }
    int getFlow()
    {
        return m_nFlow;
    }
    int getFee()
    {
        return m_nFee;
    }
    QString getAccount()
    {
        return m_strAccount;
    }
    void setAccount(QString account)
    {
        m_strAccount = account;
    }
    QString getPassword()
    {
        return m_strPassword;
    }
    void setPassword(QString password)
    {
        m_strPassword = password;
    }
    LoginType getLoginType()
    {
        return m_loginType;
    }
    void setLoginType(LoginType type)
    {
        m_loginType = type;
    }
    void setLoginType(int type)
    {
        if(type > 0 && type <= 3)
            m_loginType = LoginType(type);
    }
    NetType getNetType()
    {
        return m_netType;
    }
    //获取登录状态，true：在线；false：离线
    bool getLoginStatus()
    {
        return m_isOnline;
    }
protected:
    void run();
    QString getUrl(QUrl url);
    int getUrl(QUrl url, QString &content);
    QString postUrl(QUrl url, QMap<QString, QString> data);
    int postUrl(QUrl url, QMap<QString, QString> data, QString &content);
    QString postUrl(QUrl url, QString arg);
    int postUrl(QUrl url, QString arg, QString &content);

    QString convertMsg(int msg, QString msga = "");
private:
    //账号信息
    QString m_strAccount;
    QString m_strPassword;
    LoginType m_loginType;
    NetType m_netType;
    QNetworkConfigurationManager m_netMan;
    bool m_isOnline = false;
    int m_nTime;//分钟
    int m_nFlow;//KB
    int m_nFee;//分
signals:
    //监视消息
    void message(const QDateTime& time, const QString& info);
    //更新状态（是否登录，已用时间（分钟），已用流量（千字节），剩余金额（分））
    void status_update(bool login, int time, int flow, int fee);
private slots:
    void online_status_change(bool online);
};

#endif // BJUTNET_H

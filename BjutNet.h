#ifndef BJUTNET_H
#define BJUTNET_H

#include "common.h"
#include <QThread>
#include <QTime>
#include <QtNetwork/QNetworkReply>


class BjutNet : public QThread
{
    Q_OBJECT

public:
    //登录类型
    enum LoginType{
        IPv4, IPv6, IPv4_6
    };
    //构造函数
    explicit BjutNet();
    //登录网关
    bool login(QString& msg);
    //注销网关
    bool logout(QString& msg);
    //检测网关状态
    bool check(QString& msg);
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
    LoginType getType()
    {
        return m_type;
    }
    void setType(LoginType type)
    {
        m_type = type;
    }
    void setType(int type)
    {
        if(type >= 0 && type < 3)
            m_type = LoginType(type);
    }
protected:
    void run();
    //QString getUrl(QString url);
    QString getUrl(QUrl url);
    //QString postUrl(QString url, QMap<QString, QString> data);
    QString postUrl(QUrl url, QMap<QString, QString> data);
    //QString postUrl(QString url, QString data);
    QString postUrl(QUrl url, QString arg);
    QString convertMsg(int msg, QString msga = "");
private:
    //账号信息
    QString m_strAccount;
    QString m_strPassword;
    LoginType m_type;
    int m_nTime;//分钟
    int m_nFlow;//KB
    int m_nFee;//分
signals:
    //监视消息
    void message(const QDateTime& time, const QString& info);
    //更新状态（是否登录，已用时间（分钟），已用流量（千字节），剩余金额（分））
    void status_update(bool login, int time, int flow, int fee);
private slots:
};

#endif // BJUTNET_H

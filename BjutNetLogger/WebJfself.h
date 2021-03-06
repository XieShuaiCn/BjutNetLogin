#ifndef WEBJFSELF_H
#define WEBJFSELF_H

#include "common.h"
#include "HttpClient.h"
#include <QDateTime>
#include <QPair>

struct OnlineClientInfo{
    QString strID;
    QString strIPv4;
    QString strIPv6;
    QString strMAC;
};

class WebJfself : public QObject
{
    Q_OBJECT
public:
    WebJfself();
    //登录
    bool login();
    //检查登录状态
    bool checkLogin();
    //刷新账户信息
    bool refreshAccount();
    //刷新在线列表
    bool refreshOnline();
    //刷新套餐
    bool refreshBookService();
    //预约套餐
    bool submitBookService(const QString &id);
    //下线某个设备
    bool toOffline(const QString &id);
    bool toOfflineAll();

    PROPERTY_READ(bool, hasOnline, m_hasOnline)
    PROPERTY_READ(int, TotalFlow, m_nTotalFlow)
    PROPERTY_READ(double, TotalMoney, m_nTotalMoney)
    PROPERTY_READ_CONST(QString, AccountStatus, m_strAccountStatus)
    PROPERTY_READ_CONST(QString, AccountName, m_strAccountName)
    PROPERTY_READ_CONST(QString, ServiceName, m_strService)
    PROPERTY_READ_CONST(QVector<OnlineClientInfo>, OnlineClient, m_lstOnline)
    PROPERTY_READ_CONST(QString, CurrentBookService, m_strCurrentBookService)
    const QVector<QPair<QString, QString>> &getBookServiceList()
    {   return m_lstBookService;    }

    const QString &getAccount() const;
    void setAccount(const QString &account);
    const QString &getPassword() const;
    void setPassword(const QString &password);
signals:
    //监视消息
    void message(const QDateTime& time, const QString& info);
    void online_status_update(const QVector<OnlineClientInfo> &info);
private:
    HttpClient m_http;
    //账号信息
    QString m_strAccount;
    QString m_strPassword;
    //状态
    bool m_hasOnline = false;
    int m_nTotalFlow = 0;//MB
    int m_nTotalMoney = 0;//分
    QString m_strAccountStatus;
    QString m_strAccountName;
    QString m_strService;
    QVector<OnlineClientInfo> m_lstOnline;
    QVector<QPair<QString, QString>> m_lstBookService;
    QString m_strCurrentBookService;
};

inline const QString &WebJfself::getAccount() const
{
    return m_strAccount;
}
inline void WebJfself::setAccount(const QString &account)
{
    m_strAccount = account;
}
inline const QString &WebJfself::getPassword() const
{
    return m_strPassword;
}
inline void WebJfself::setPassword(const QString &password)
{
    m_strPassword = password;
}

#endif // WEBJFSELF_H

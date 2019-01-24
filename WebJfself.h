#ifndef WEBJFSELF_H
#define WEBJFSELF_H

#include "common.h"
#include "HttpClient.h"
#include <QDateTime>

struct OnlineClientInfo{
    QString strID;
    QString strIPv4;
    QString strIpv6;
    QString strMAC;
};

class WebJfself : public QObject
{
    Q_OBJECT
public:
    WebJfself();
    bool login();

    void refreshAccount();
    void refreshOnline();

    PROPERTY_READ(bool, hasOnline, m_hasOnline)
    PROPERTY_READ(int, TotalFlow, m_nTotalFlow)
    PROPERTY_READ(double, TotalMoney, m_nTotalMoney)
    PROPERTY_READ_CONST(QString, AccountStatus, m_strAccountStatus)
    PROPERTY_READ_CONST(QString, AccountName, m_strAccountName)
    PROPERTY_READ_CONST(QString, ServiceName, m_strService)
    PROPERTY_READ_CONST(QVector<OnlineClientInfo>, OnlineClient, m_lstOnline)

    const QString &getAccount() const;
    void setAccount(const QString &account);
    const QString &getPassword() const;
    void setPassword(const QString &password);
signals:
    //监视消息
    void message(const QDateTime& time, const QString& info);
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

#include "WebJfself.h"
#include <QTextCodec>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QRegExp>

WebJfself::WebJfself()
{
    m_http.setCodec("UTF-8");
    m_lstOnline.reserve(2);
}

bool WebJfself::login()
{
    QString content;
    QByteArray data;
    int status = m_http.getUrlHtml(QUrl("https://jfself.bjut.edu.cn/nav_login"), content);
    if(status == 200 || status == 0)
    {
        status = m_http.downUrlData(QUrl(QString("https://jfself.bjut.edu.cn/RandomCodeAction.action?randomNum=%1")
                                    .arg(1.0 * qrand() / RAND_MAX)),
                                    data);
        if(status == 200 || status == 0)
        {
            QRegExp regCheckCode("var checkcode=\"(\\d{4})\";", Qt::CaseInsensitive);
            if (content.indexOf(regCheckCode) <= 0) {
                    return false;
            }
            QString strCheckCode = regCheckCode.cap(1);
            QMap<QString, QString> arg;
            arg.insert("account", m_strAccount);
            arg.insert("password", m_strPassword);
            arg.insert("code", "");
            arg.insert("checkcode", strCheckCode);
            arg.insert("Submit", "%E7%99%BB+%E5%BD%95");
            status = m_http.postUrlHtml(QUrl("https://jfself.bjut.edu.cn/LoginAction.action"), arg,content);
            if(status == 200 || status == 0)
            {
                if(content.indexOf("info_title") > 0)
                {
                    return true;
                }
                else {
                    emit message(QDateTime::currentDateTime(), "Can not find \"info_title\"");
                }
            }
            else {
                emit message(QDateTime::currentDateTime(), "用户自助系统登录失败!");
            }
        }
        else {
            emit message(QDateTime::currentDateTime(), "验证码获取失败!");
        }
    }
    else {
        emit message(QDateTime::currentDateTime(), "用户自助登录信息获取失败!");
    }
    return false;
}

bool WebJfself::checkLogin()
{
    const QByteArray htmlTag("html");
    QByteArray content;
    int status = m_http.downUrlData(QUrl(QString("https://jfself.bjut.edu.cn/refreshaccount?t=%1")
                                        .arg(1.0 * qrand() / RAND_MAX)),
                                    content);
    if(status == 200 || status == 0)
    {
        if(content.indexOf(htmlTag) < 0 && content.indexOf('{') >= 0)
        {
            return true;
        }
    }
    return false;
}

bool WebJfself::refreshAccount()
{
    if(!checkLogin()){
        login();
        if(!checkLogin()) return false;
    }
    QByteArray content;
    int status = m_http.downUrlData(QUrl(QString("https://jfself.bjut.edu.cn/refreshaccount?t=%1")
                                        .arg(1.0 * qrand() / RAND_MAX)),
                                    content);
    if(status == 200 || status == 0)
    {
        QJsonParseError jp_err;
        const QJsonDocument jd = QJsonDocument::fromJson(content, &jp_err);
        if(jp_err.error == QJsonParseError::NoError)
        {
            const QJsonObject jo = jd.object();
            if(jo.contains("date"))
            {
                QString success = jo.value("date").toString();
                if (success != "success")
                {
                    if(jo.contains("outmessage"))
                    {
                        emit message(QDateTime::currentDateTime(),
                                     "Refresh account:"+(jo.value("outmessage").toString()));
                    }
                    emit message(QDateTime::currentDateTime(),
                                  "刷新账户信息失败");
                    return false;
                }
                const auto node = jo.value("note").toObject();
                if(node.contains("leftFlow"))
                {
                    m_nTotalFlow = static_cast<int>(node.value("leftFlow").toString().toDouble());
                }
                if(node.contains("leftmoeny"))
                {
                    auto money = node.value("leftmoeny").toString();
                    m_nTotalMoney = static_cast<int>(money.left(money.size()-1).toDouble() * 100);
                }
                if(node.contains("service"))
                {
                    m_strService = node.value("service").toString();
                }
                if(node.contains("onlinestate"))
                {
                    m_hasOnline = static_cast<bool>(node.value("onlinestate").toString().toInt());
                }
                if(node.contains("status"))
                {
                    m_strAccountStatus = node.value("status").toString();
                }
                if(node.contains("welcome"))
                {
                    auto name = node.value("welcome").toString();
                    int l = name.indexOf('(');
                    int r = name.indexOf(')');
                    m_strAccountName = name.mid(l, r-l);
                }
                return true;
            }
        }
        else {
            emit message(QDateTime::currentDateTime(),
                         "解析账户信息失败:"+jp_err.errorString());
#ifdef QT_DEBUG
            QFile f("refresh_account.json");
            f.open(QIODevice::WriteOnly | QIODevice::Text);
            f.write(content);
            f.close();
#endif
        }
    }
    return false;
}

bool WebJfself::refreshOnline()
{
    if(!checkLogin()){
        login();
        if(!checkLogin()) return false;
    }
    QString content;
    int status = m_http.getUrlHtml(QUrl(QString("https://jfself.bjut.edu.cn/nav_offLine")),
                                   content);
//    QFile f("nav_offline.0.html");
//    f.open(QIODevice::ReadOnly|QIODevice::Text);
//    content = QString(f.readAll());
//    f.close();
//    int status = 200;
    if(status == 200 || status == 0)
    {
        int i1 = content.indexOf("<tbody>", 0, Qt::CaseInsensitive);
        int i2 = content.indexOf("</tbody>", 0, Qt::CaseInsensitive);
        if(i1 >= 0 && i2 >= 0 && i2 > i1)
        {
            m_lstOnline.clear();
            QString tbody = content.mid(i1+7, i2-i1-7);
            i1 = 0;            i2 = 0;
            for(int n = 0; n < 2; ++n){
                i1 = tbody.indexOf("<tr>", i2, Qt::CaseInsensitive);
                i2 = tbody.indexOf("</tr>", i1, Qt::CaseInsensitive);
                if(i1>=0){
                    int j1 = tbody.indexOf("<td>", i1, Qt::CaseInsensitive);
                    int j2 = tbody.indexOf("</td>", j1, Qt::CaseInsensitive);
                    QString ipv4 = tbody.mid(j1+4, j2-j1-4);
                    ipv4 = ipv4.replace("&nbsp;", "").trimmed();
                    j1 = tbody.indexOf("<td>", j2, Qt::CaseInsensitive);
                    j2 = tbody.indexOf("</td>", j1, Qt::CaseInsensitive);
                    QString ipv6 = tbody.mid(j1+4, j2 -j1-4);
                    ipv6 = ipv6.replace("&nbsp;", "").trimmed();
                    j1 = tbody.indexOf("<td>", j2, Qt::CaseInsensitive);
                    j2 = tbody.indexOf("</td>", j1, Qt::CaseInsensitive);
                    QString macad = tbody.mid(j1+4, j2 -j1-4);
                    macad = macad.replace("&nbsp;", "").trimmed();
                    j1 = tbody.indexOf("<td style=\"display:none;\">", j2, Qt::CaseInsensitive);
                    j2 = tbody.indexOf("</td>", j1, Qt::CaseInsensitive);
                    QString id = tbody.mid(j1+26, j2 -j1-26);
                    id = id.replace("&nbsp;", "").trimmed();
                    m_lstOnline.push_back({id, ipv4, ipv6, macad});
                }
                else {
                    break;
                }
            }
            emit online_status_update(m_lstOnline);
            return true;
        }
        else {
            emit message(QDateTime::currentDateTime(),
                         QString("解析在线信息失败:tbody(%1,%2)").arg(i1).arg(i2));
#ifdef QT_DEBUG
            QFile f("nav_offline.html");
            f.open(QIODevice::WriteOnly | QIODevice::Text);
            f.write(content.toLocal8Bit());
            f.close();
#endif
        }
    }
    return false;
}

//刷新套餐
bool WebJfself::refreshBookService()
{
    m_lstBookService.clear();
    m_strCurrentBookService.clear();
    if(!checkLogin()){
        login();
        if(!checkLogin()) return false;
    }
    QString content;
    int status = m_http.getUrlHtml(QUrl(QString("https://jfself.bjut.edu.cn/nav_servicedefaultbook")),
                                   content);
//    QFile f("nav_offline.0.html");
//    f.open(QIODevice::ReadOnly|QIODevice::Text);
//    content = QString(f.readAll());
//    f.close();
//    int status = 200;
    if(status == 200 || status == 0)
    {
        int idx1 = content.indexOf("<form");
        if (idx1 < 0) return false;
        int idx2 = content.indexOf("</form", idx1);
        if (idx2 <= idx1) return false;
        QString table = content.mid(idx1, idx2-idx1);
        int idx_name1= table.indexOf(QChar('['));
        int idx_name2= table.indexOf(QChar(']'), idx_name1);
        if(idx_name1 >= 0 && idx_name2 > idx_name1)
        {
            m_strCurrentBookService = table.mid(idx_name1+1, idx_name2-idx_name1-1);
        }
        int idx_body = table.indexOf("<tbody>");
        if(idx_body > 0)
        {
            int idx_body2 = table.indexOf("</tbody>");
            QString tbody = table.mid(idx_body+7, idx_body2-idx_body-7);
            int i1 = 0, i2 = 0;
            while(true){
                i1 = tbody.indexOf("<tr>", i2, Qt::CaseInsensitive);
                i2 = tbody.indexOf("</tr>", i1, Qt::CaseInsensitive);
                if(i1<0) break;
                int idx_srvvalue = tbody.indexOf("value=\"", i1, Qt::CaseInsensitive);
                int idx_srvname = tbody.indexOf("\">", idx_srvvalue, Qt::CaseInsensitive);
                int idx_srvname2 = tbody.indexOf("</", idx_srvname, Qt::CaseInsensitive);
                m_lstBookService.append({tbody.mid(idx_srvvalue+7, idx_srvname-idx_srvvalue-7).trimmed(),
                                        tbody.mid(idx_srvname+2, idx_srvname2-idx_srvname-2).trimmed()});
            }
        }
        return true;
    }
    return false;
}

//预约套餐
bool WebJfself::submitBookService(const QString &id)
{
    if(!checkLogin()){
        login();
        if(!checkLogin()) return false;
    }
    QString content;
    QMap<QString, QString> arg;
    arg.insert("serid", id);
    int status = m_http.postUrlHtml(QUrl(QString("https://jfself.bjut.edu.cn/selfservicebookAction")),
                                   arg, content);
//    QFile f("nav_offline.0.html");
//    f.open(QIODevice::ReadOnly|QIODevice::Text);
//    content = QString(f.readAll());
//    f.close();
//    int status = 200;
    if(status == 200 || status == 0)
    {
        return true;
    }
    return false;
}

bool WebJfself::toOffline(const QString &id)
{
    if(!checkLogin()){
        login();
        if(!checkLogin()) return false;
    }
    QByteArray content;
    int status = m_http.downUrlData(QUrl(QString("https://jfself.bjut.edu.cn/tooffline?t=%1&fldsessionid=%2")
                                    .arg(1.0 * qrand() / RAND_MAX).arg(id)),
                                    content);
    if(status == 200 || status == 0)
    {
        QJsonParseError jp_err;
        const QJsonDocument jd = QJsonDocument::fromJson(content, &jp_err);
        if(jp_err.error == QJsonParseError::NoError)
        {
            const QJsonObject jo = jd.object();
            if(jo.contains("date"))
            {
                QString success = jo.value("date").toString();
                if (success != "success")
                {
                    if(jo.contains("outmessage"))
                    {
                        emit message(QDateTime::currentDateTime(),
                                     "ToOffline ID:"+(jo.value("outmessage").toString()));
                    }
                    emit message(QDateTime::currentDateTime(),
                                  "强制设备下线失败");
                    return false;
                }
                if(jo.contains("outmessage") && jo.value("outmessage").toString() == "true")
                {
                    emit message(QDateTime::currentDateTime(),
                                  QString("强制设备(%1)下线成功").arg(id));
                    return true;
                }
            }
        }
    }
    return false;
}

bool WebJfself::toOfflineAll()
{
    for(const OnlineClientInfo &it : m_lstOnline)
    {
        toOffline(it.strID);
    }
    return true;
}


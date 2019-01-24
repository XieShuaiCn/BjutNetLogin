#include "WebJfself.h"
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QTextCodec>

WebJfself::WebJfself()
{
    m_http.setCodec("UTF-8");
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

void WebJfself::refreshAccount()
{
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
                    return;
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
                return;
            }
        }
        else {
            emit message(QDateTime::currentDateTime(),
                         "解析账户信息失败:"+jp_err.errorString());
        }
    }
}

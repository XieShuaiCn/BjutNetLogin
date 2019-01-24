#include "Updater.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QUrl>
#include <QDir>
#include <QEventLoop>
#include <QTextCodec>
#include <QStandardPaths>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>


QString RandString(int length)
{
    const QString set("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    QString result;
    for(int i = 0; i < length; ++i) {
        result.append(set[rand() % 36]);
    }
    return result;
}

Updater::Updater(QObject *parent) : QObject(parent),
    m_nNewVersion(0), m_nOldVersion(0),
    m_strNewVersion("0.0.0"), m_strOldVersion("0.0.0")
{
    QFile file("./config.db");
    if(file.exists()) {
        if(file.open(QFile::ReadOnly))
        {
            QJsonParseError jp_err;
            const QJsonDocument jdoc = QJsonDocument::fromJson(file.readAll(), &jp_err);
            file.close();
            if(jp_err.error == QJsonParseError::NoError)
            {
                const QJsonObject jo = jdoc.object();

                if(jo.contains("version")) {
                    m_strOldVersion = (jo["version"].toString());
                }
                if(jo.contains("inner_ver")) {
                    m_nOldVersion = (jo["inner_ver"].toInt());
                }

            }
        }
#ifdef QT_DEBUG
        else {
            qDebug() << file.errorString() << "\n";
        }
#endif
    }
}

bool Updater::checkUpdate()
{
    bool success = false;
    QString content;
    int ret = getUrl(QUrl(m_strHostName + "online/newest.php?os="
#ifdef Q_OS_WIN
                          "Windows"
#elif defined(Q_OS_LINUX)
                          "Linux64"
#endif
                     ), content);
    if(ret != 200){
        return false;
    }

    //{"success":true,"version":"1.4","inner_ver":"7","introduction":"1\u3001\u7a0b\u5e8f\u521d\u59cb\u5316\u5931\u8d25\u65f6\uff0c\u76f4\u63a5\u7ed3\u675f\r\n2\u3001\u6bcf\u9694\u534a\u5c0f\u65f6\u8f93\u51fa\u4e00\u6b21\u8d26\u53f7\u6d41\u91cf\u4fe1\u606f"}
    QJsonParseError jp_err;
    const QJsonDocument jdoc = QJsonDocument::fromJson(content.toLocal8Bit(), &jp_err);
    if(jp_err.error == QJsonParseError::NoError)
    {
        const QJsonObject jo = jdoc.object();
        if(jo.contains("success"))
        {
            success = jo["success"].toBool();
        }
        if(success){
            if(jo.contains("version")) {
                m_strNewVersion = (jo["version"].toString());
            }
            if(jo.contains("inner_ver")) {
                m_nNewVersion = (jo["inner_ver"].toInt());
            }
            if(jo.contains("introduction")){
                m_strNewIntroduction = (jo["introduction"].toString());
            }
        }
    }
    return success;
}

bool Updater::needUpdate()
{
    return m_nNewVersion > m_nOldVersion;
}

int Updater::getUrl(QUrl url, QString &content)
{
    //建立http连接
    QNetworkAccessManager* pManager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    QEventLoop loop;
    request.setUrl(url);
    QNetworkReply *pReply = pManager->get(request);

    //设置请求回应的回调
    connect(pReply , SIGNAL(finished()) , &loop , SLOT(quit()));
    //等待回应
    loop.exec();

    if(pReply->error() != QNetworkReply::NoError)//error
    {
#ifdef QT_DEBUG
        qDebug() << pReply->errorString() << "\n";
#endif
        return pReply->error() + 10000;
    }
    //读取
    QByteArray data = pReply->readAll();
#ifdef QT_DEBUG
    if(data.size() == 0)
        qDebug() << "getUrl Error: " << pReply->errorString();
#endif
    //数据转码
    QTextCodec *codec = QTextCodec::codecForHtml(data);
    content.clear();
    content.append(codec->toUnicode(data));

    int status = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    //QList<QByteArray> ls = pReply->rawHeaderList();
    //int status = pReply->rawHeader("").toInt();

    delete pManager;
    return status;
}

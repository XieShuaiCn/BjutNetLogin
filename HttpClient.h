#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include "common.h"
#include "HNetworkCookieJar.h"
#include <QUrl>
#include <QNetworkAccessManager>

class HttpClient : public QObject
{
public:
    HttpClient();
    //~HttpClient();
    QString getUrlHtml(QUrl url);
    int getUrlHtml(QUrl url, QString &content);
    QString postUrlHtml(QUrl url, QMap<QString, QString> data);
    int postUrlHtml(QUrl url, QMap<QString, QString> data, QString &content);
    QString postUrlHtml(QUrl url, QString arg);
    int postUrlHtml(QUrl url, QString arg, QString &content);
    int downUrlData(QUrl url, QByteArray &content);
    int downUrlData(QUrl url, QByteArray arg, QByteArray &content, bool bPost=true);

    bool setCodec(const QByteArray &codeName);
    bool pop_message(QDateTime& time, QString& info);
    void clear_message();
protected:
    void push_message(const QDateTime& time, const QString& info);
    QList<QPair<QDateTime, QString> > m_arrMessage;
    //
    HNetworkCookieJar *m_netCookie;
    QNetworkAccessManager m_netMan;
    QTextCodec *m_pCodec = nullptr;
};


inline int HttpClient::downUrlData(QUrl url, QByteArray &content)
{
    return downUrlData(url, QByteArray(), content, false);
}

#endif // HTTPCLIENT_H

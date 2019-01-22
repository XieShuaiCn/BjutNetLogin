#ifndef HNETWORKCOOKIEJAR_H
#define HNETWORKCOOKIEJAR_H

#include <QList>
#include <QtNetwork/QNetworkCookie>
#include <QtNetwork/QNetworkCookieJar>

class HNetworkCookieJar : public QNetworkCookieJar
{
public:
    HNetworkCookieJar() = default;
    QList<QNetworkCookie> getCookies();
    QByteArray getCookieBytes();
};

#endif // HNETWORKCOOKIEJAR_H

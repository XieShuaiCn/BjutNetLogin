#include "HNetworkCookieJar.h"

QList<QNetworkCookie> HNetworkCookieJar::getCookies()
{
    return allCookies();
}

QByteArray HNetworkCookieJar::getCookieBytes()
{
    QByteArray data;
    for(const auto &it : allCookies())
    {
        if(data.size())
        {
            data.push_back("; ");
        }
        data.push_back(it.name());
        data.push_back('=');
        data.push_back(it.value());
    }
    return data;
}

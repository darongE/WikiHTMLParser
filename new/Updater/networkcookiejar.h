#ifndef NETWORKCOOKIEJAR_H
#define NETWORKCOOKIEJAR_H

#include <QNetworkCookieJar>
#include <QNetworkCookie>

class NetworkCookieJar : public QNetworkCookieJar
{
public:
    explicit NetworkCookieJar(QObject *parent = nullptr);
    QList<QNetworkCookie> AllCookies();


};

#endif // NETWORKCOOKIEJAR_H

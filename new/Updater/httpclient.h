#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include "ihttpclient.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkReply>
#include <QList>



class HttpClient : public IHttpClient
{
 Q_OBJECT

    QNetworkAccessManager *networkManager;
    bool                   error;
    QString                page_content;




public:
    HttpClient();
};

#endif // HTTPCLIENT_H

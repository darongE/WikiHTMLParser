#include "searchbackend.h"

#include <QNetworkAccessManager>
#include <QDebug>
#include <QDomDocument>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


class SearchBackend::Private
{
public:

    explicit Private()
        :results(),
         netReply(0),
         runningBackend(),
         searchData(),
         errorMessage()

    {

    }

    SearchBackend::SearchResult::List results;
    QNetworkReply*                    netReply;
    QString                           runningBackend;
    QByteArray                        searchData;
    QString                           errorMessage;

};

SearchBackend::SearchBackend(QObject * const parent)
    : QObject(parent),
      d(new Private())
{

}

SearchBackend::~SearchBackend()
{
    delete d;
}


// get keyword from edit, trigger SearchBackend
//https://en.wikipedia.org/wiki/Steve_Jobs
bool SearchBackend::search(const QString &backendName, const QString &searchTerm)
{
    d->searchData.clear();
    d->errorMessage.clear();
    d->results.clear();

    QNetworkAccessManager* const mngr = new QNetworkAccessManager(this);

    connect(mngr, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slotFinished(QNetworkReply*)));

    if(backendName == QLatin1String("wikipedia"))
    {
        d->runningBackend = backendName;

        QUrl netUrl(QLatin1String("https://en.wikipedia.org/wiki/"));
        netUrl = QUrl(netUrl.toString().append(searchTerm));
//        q.addQueryItem(QLatin1String("format"), QLatin1String("xml"));
//        q.addQueryItem(QLatin1String("addressdetails"), QLatin1String("1"));
//        q.addQueryItem(QLatin1String("limit"), QLatin1String("1"));
//        q.addQueryItem(QLatin1String("accept-language"), QLatin1String("en"));

        qDebug() << netUrl;

        QNetworkRequest netRequest(netUrl);
        netRequest.setRawHeader("User-Agent", "Mozilla");
        netRequest.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                                QNetworkRequest::NoLessSafeRedirectPolicy);
        d->netReply = mngr->get(netRequest);

        return true;
    }


   return false;
 }





 void SearchBackend::slotFinished(QNetworkReply* reply)
 {
     if (reply != d->netReply)
     {
         return;
     }

     if (reply->error() != QNetworkReply::NoError)
     {
         d->errorMessage = reply->errorString();
         emit(signalSearchCompleted());
         reply->deleteLater();
         return;
     }

    d->searchData.append(reply->readAll());

    const QString resultString = QString::fromUtf8(d->searchData.constData(), d->searchData.count());

    if(d->runningBackend == QLatin1String("wikipedia"))
    {
        QDomDocument doc;
        doc.setContent(resultString);

        QDomElement docElement = doc.documentElement();

        for(QDomNode resultNode = docElement.firstChild(); !resultNode.isNull(); resultNode = resultNode.nextSibling())
        {
            QDomElement resultElement = resultNode.toElement();

            if(resultElement.isNull())
                continue;

             QString  title = resultElement.attribute(QLatin1String("title"));

            qDebug() << "title" <<title;


            SearchResult  result;

            result.name        = title;

//            if(!placeId.isEmpty())
//                result.internalId = QLatin1String("wikipedia-") + placeId;

//            d->results << result;

        }

    }

    emit signalSearchCompleted();

    reply->deleteLater();
}

 SearchBackend::SearchResult::List SearchBackend::getResults() const
 {
     return d->results;
 }


QString SearchBackend::getErrorMessage() const
{
    return d->errorMessage;
}
















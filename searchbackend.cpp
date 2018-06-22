#include "searchbackend.h"

#include <QNetworkAccessManager>
#include <QDebug>
#include <QDomDocument>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


#define RANGE(x)  x.begin(), x.end()

using namespace EasyGumbo;

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

        //https://en.wikipedia.org/w/index.php?title=Albert_Einstein&printable=yes#bodyContent
        QUrlQuery query;

        QUrl netUrl(QLatin1String("https://en.wikipedia.org/w/index.php?"));
        query.addQueryItem("title",searchTerm);
        query.addQueryItem("printable","yes");

        netUrl.setQuery(query);

        netUrl = QUrl(netUrl.toString().append("#bodyContent"));

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


//hint
//https://stackoverflow.com/questions/1565347/get-first-lines-of-wikipedia-article/19781754
 void SearchBackend::slotFinished(QNetworkReply* reply)
 {
     if (reply != d->netReply)
         return;

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

            qDebug() << resultElement.text();


//            if(resultElement.isNull())
//                continue;

//             QString  title = resultElement.attribute(QLatin1String("title"));

//            qDebug() << "title" <<title;


//            SearchResult  result;

//            result.name        = title;

    //            if(!placeId.isEmpty())
    //                result.internalId = QLatin1String("wikipedia-") + placeId;

    //            d->results << result;

        }

    }


    emit signalSearchCompleted();

    reply->deleteLater();
}

 void SearchBackend::parse(const QByteArray& html)
 {
     Gumbo gumbo(html.data());

     if(gumbo)
     {
         using Iterator = Gumbo::iterator;
         const Iterator end;

         const auto pages = findAll(gumbo.begin(), gumbo.end(), Tag(GUMBO_TAG_P));
         Iterator iter(pages[0]->parent);

         while(iter != end )
         {

             if (iter->type == GUMBO_NODE_TEXT)
             {
               QString data = QString::fromUtf8(iter->v.text.text);
               qDebug() << data;

             }
             ++iter;
         }
     }


 }

 void SearchBackend::extractHTMLTag(GumboNode* node)
 {


 }

 SearchBackend::SearchResult::List SearchBackend::getResults() const
 {
     return d->results;
 }


QString SearchBackend::getErrorMessage() const
{
    return d->errorMessage;
}
















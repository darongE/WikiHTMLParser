#include "searchbackend.h"

#include <QNetworkAccessManager>
#include <QDebug>
#include <QDomDocument>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QXmlStreamReader>


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


 //   QString resultString = QString::fromUtf8(d->searchData.constData(), d->searchData.count());

    parse(d->searchData);
    emit signalSearchCompleted();

    reply->deleteLater();
}


 void SearchBackend::parse(QByteArray& html)
 {
     Gumbo gumbo(html.data());
     QString data, text;
     QStringList stringList;




     if(gumbo)
     {
         using Iterator = Gumbo::iterator;

         const auto pages = findAll(gumbo.begin(), gumbo.end(), Tag(GUMBO_TAG_P));

         QRegExp rx("\\[|\\]");

         for (size_t row = 0; row < pages.size(); ++row)
        {

           Iterator iter(pages[row]), end;


           while(iter != end)
         {

             if (iter->type == GUMBO_NODE_TEXT )
             {
               QString data = QString::fromUtf8(iter->v.text.text);


               int pos = rx.indexIn(data);

               if(pos > -1)
                   data.clear();
               else
               {

                   if(data.length() > 0)
                      text += data;
               }


             }
             else if(iter->type == GUMBO_NODE_ELEMENT
                      && iter->v.element.tag == GUMBO_TAG_P && text.length() > 0)
             {

                  const Element tr(*iter);
                   stringList << text;

                   qDebug() << text;

                   text.clear();

             }


             if(iter->v.element.children.length== 0)
                 break;

                ++iter;


         }
      }

    }
}

SearchBackend::SearchResult::List SearchBackend::getResults() const
{
    return d->results;
}


QString SearchBackend::getErrorMessage() const
{
    return d->errorMessage;
}
















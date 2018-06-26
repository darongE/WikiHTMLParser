#include "searchbackend.h"

#include <QNetworkAccessManager>
#include <QDebug>
#include <QDomDocument>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


#define RANGE(x)  x.begin(), x.end()
#define JSON_PRINT_P(x) QJsonDocument(x).toJson().constData()


class SearchBackend::Private
{
public:

    explicit Private()
        :results(),
         netReply(0),
         runningBackend(),
         searchData(),
         errorMessage(),
         url()

    {

    }

    SearchResult::List                results;
    QNetworkReply*                    netReply;
    QString                           runningBackend;
    QByteArray                        searchData;
    QString                           errorMessage;
    QUrl                              url;

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
  //  d->searchData.clear();
    d->errorMessage.clear();
    //d->results.clear();

    QNetworkAccessManager* const mngr = new QNetworkAccessManager(this);

    connect(mngr, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slotFinished(QNetworkReply*)));

    if(backendName == QLatin1String("wikipedia"))
    {
        d->runningBackend = backendName;

        //https://en.wikipedia.org/w/index.php?title=Albert_Einstein&printable=yes#bodyContent

       // https://en.wikipedia.org/w/api.php?action=parse&page=Thomas_Edison&format=xml
        QUrlQuery query;

        QUrl netUrl(QLatin1String("https://en.wikipedia.org/w/api.php?"));
        query.addQueryItem("action","parse");
        query.addQueryItem("page",searchTerm);
        query.addQueryItem("format","json");

        netUrl.setQuery(query);
        d->url = netUrl;

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

//parse
//title	"Thomas Edison"
//pageid	29778
//revid	846261225

//text	{…}
//langlinks	[…]
//categories	[…]
//links	[…]
//templates	[…]
//images	[…]
//externallinks	[…]
//sections	[…]
//parsewarnings	[]
//displaytitle	"Thomas Edison"
//iwlinks	[…]
//properties	[…]

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

    // read json
    const QString resultString = QString::fromUtf8(d->searchData.constData(), d->searchData.count());


    QJsonDocument doc = QJsonDocument::fromJson(resultString.toUtf8());

     if(doc.isNull()) {
           qDebug() << "Doc is null/invalid in inverts.";
         return;
     }


    QJsonObject jsonObject = doc.object();
    QString htmlData;
    QJsonObject subObj = jsonObject["parse"].toObject()["text"].toObject();
    htmlData = subObj.value("*").toString();

    parse(htmlData.toLatin1());




 //   qDebug() << "html data" << htmlData;
    emit signalSearchCompleted();

    reply->deleteLater();
}

 void SearchBackend::parse(QByteArray bytearray)
 {
     Gumbo gumbo(bytearray.data());
     QString paragraph;

     QRegExp rx("\\[|\\]");

     int index = 0;
     if(gumbo){

          using Iterator = Gumbo::iterator;
          const Iterator end;

          const auto pages = findAll(gumbo.begin(), gumbo.end(), Tag(GUMBO_TAG_P));

          auto tagH2 =findAll( gumbo.begin(), gumbo.end(), Tag(GUMBO_TAG_H2));
          {
              qDebug() << tagH2.size();

               qDebug() << tagH2[0]->v.element.start_pos.offset;
          }



          for (size_t row = 0; row < pages.size(); ++row)
         {

           Iterator iter(pages[row]), end;

          while(iter != end )
          {

              if (iter->type == GUMBO_NODE_TEXT)
               {
                 QString data = QString::fromUtf8(iter->v.text.text);

                 int pos = rx.indexIn(data);

                 if(pos > -1)
                     data.clear();
                 else
                 {
                     if(data.length() > 0)
                        paragraph += data;


                 }

                }
              else if(iter->type == GUMBO_NODE_ELEMENT
                      && paragraph.length() > 0)

             {

                  const Element tr(*iter);
                  SearchResult result;


                  if(iter->v.element.tag == GUMBO_TAG_P)
                 {

                    if(row == 1)
                        result.mUrl = d->url;

                    result.text = paragraph;
                    result.index =  QString::number(++index);

                    // d->results << result;
                    qDebug() << row << paragraph;
                    paragraph.clear();
                    qDebug() << iter->v.element.start_pos.offset;

                  }



             }

              if(iter->v.element.children.length== 0)
                  break;

              ++iter;
          }

       }
     }


 }


// <h2>Contents</h2>
// </div>
// <ul>
// <li class="toclevel-1 tocsection-1"><a href="#Early_life"><span class="tocnumber">1</span> <span class="toctext">Early life</span></a></li>
// <li class="toclevel-1 tocsection-2"><a href="#Telegrapher"><span class="tocnumber">2</span> <span class="toctext">Telegrapher</span></a></li>
// <li class="toclevel-1 tocsection-3"><a href="#Marriages_and_children"><span class="tocnumber">3</span> <span class="toctext">Marriages and children</span></a></li>
// <li class="toclevel-1 tocsection-4"><a href="#Beginning_his_career"><span class="tocnumber">4</span> <span class="toctext">Beginning his career</span></a></li>
// <li class="toclevel-1 tocsection-5"><a href="#Menlo_Park"><span class="tocnumber">5</span> <span class="toctext">Menlo Park</span></a>
// <ul>


 // parsing


SearchBackend::SearchResult::List SearchBackend::getResults() const
{
    return d->results;
}


QString SearchBackend::getErrorMessage() const
{
    return d->errorMessage;
}
















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
       // https://en.wikipedia.org/w/api.php?action=parse&page=Thomas_Edison&format=json
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

    parseAll(htmlData.toLatin1());
  //  parseContents(htmlData.toLatin1());
 //   qDebug() << "html data" << htmlData;
    emit signalSearchCompleted();

    reply->deleteLater();
}

 void SearchBackend::parseAll(const QByteArray &html)
 {
     Gumbo gumbo(html.data());
     if (gumbo) {
         auto iter = std::find_if(RANGE(gumbo), And(Tag(GUMBO_TAG_DIV),
                                                    HasAttribute("class", "mw-parser-output")));
         const Element elPage(*iter);
         parseText(elPage.children()[0]->parent);
        // parseTagAttributes(elPage.children()[0]->parent);

     }
 }

 void SearchBackend::parseText(GumboNode *node)
 {

    using Iterator = Gumbo::iterator;
    Iterator iter(node);
    const Iterator end;

    int index = 0;
    QRegExp rx("\\[|\\]");
    QString paragraph;

    const auto tagP = findAll(iter.fromCurrent(), end, Tag(GUMBO_TAG_P));

    for (size_t row = 0; row < tagP.size(); ++row)
    {

        Iterator iter(tagP[row]);

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
                    && iter->v.element.tag == GUMBO_TAG_P
                    && paragraph.size() > 0)
            {

                SearchResult result;
                if(row == 1)
                    result.mUrl = d->url;
                    
                result.text = paragraph;
                result.index =  QString::number(++index);
                d->results << result;
                paragraph.clear();

               QString endTag = QString::fromUtf8(iter->v.document.system_identifier).left(20);
               findEndTag(endTag,row);

            }

            if(iter->type == GUMBO_NODE_WHITESPACE)
                break;

            iter++;
       }


    }

    qDebug() << "END";

  }



 void SearchBackend::findEndTag(QString &tag, int row)
 {

     qDebug() << "end tag" << tag
              << "row"     << row;

     SearchResult m_search;
     QString endTag1 = "</p>\n<h2>";
     QString endTag2 = "</p><div";
     QString endTag3 = "</p>\n<h3>";


     if(tag.startsWith(endTag1) || tag.startsWith(endTag2) ||
        tag.startsWith(endTag3))
     {
        m_search.text = "title";
        d->results.insert(row,m_search);
     }
 }

 void SearchBackend::parseTagAttributes(GumboNode *node)
 {

    using Iterator = Gumbo::iterator;
    Iterator iter(node);
    const Iterator end;
    QString title;

    auto tagSpan = findAll(iter.fromCurrent(), end, And(Tag(GUMBO_TAG_SPAN),
                                                     HasAttribute("class", "mw-headline")));

    for (size_t row = 0; row < tagSpan.size(); ++row)
    {

        Iterator iter(tagSpan[row]);

        while(iter != end )
        {

           if(iter->type == GUMBO_NODE_TEXT)
           {
             QString data = QString::fromUtf8(iter->v.text.text);

             if(data.length() > 0)
                title = data;

           }
           else if(iter->type == GUMBO_NODE_ELEMENT
                   && title.length() > 0)
           {

              const Element elHeadline(*iter);
              SearchResult result;

              if(QString(elHeadline.attribute("class")->value) == "mw-headline")
              {
               result.contents = title;
               qDebug() << row << title;
              }


           }

         ++iter;
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














#include <QNetworkAccessManager>
#include <QDebug>
#include <QDomDocument>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


#include "searchbackend.h"


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
    filtertag = new FilterTag(this);
}

SearchBackend::~SearchBackend()
{

    delete d;
    delete filtertag;
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
    QJsonObject mainObj = jsonObject["parse"].toObject();
    QJsonObject subObj = mainObj["text"].toObject();
    QJsonArray sectionArray = mainObj["sections"].toArray();

    QString htmlData = subObj.value("*").toString();

    parseIndex(sectionArray);
    parseAll(htmlData.toLatin1());

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
         parseH2(elPage.children()[0]->parent);
         parseH3(elPage.children()[0]->parent);
         parseDIV(elPage.children()[0]->parent);
         parseTable(elPage.children()[0]->parent);

         insertStringList();


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

    //find div

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

                paragraph.clear();

                QString endTag = QString::fromUtf8(iter->v.document.system_identifier).left(12);
                //qDebug() << row << "," << endTag;
                result.endTag = endTag;
                d->results << result;


            }

            if(iter->type == GUMBO_NODE_WHITESPACE)
                break;

            iter++;
       }


    }

    //qDebug() << "END";

  }


// enter title

void SearchBackend::findEndTag()
 {


     SearchResult m_search;

     QRegExp regP("</h2>\n<p>");
//     QRegExp regDIV("</p>\n<div");
//     QRegExp regH3("</p>\n<h3>");

     int i = 0, j = 0;

     int nSize = d->results.size();

     for(j = 0; j < nSize; ++j)
     {
         int pos1 = regP.indexIn(d->results[j].endTag);

         SearchResult m_research;
         m_research.text = "title";

         if(pos1 > -1 )
         {
             d->results.insert(j,m_research);
         }


     }

 }

//"sections": [
//     {
//       "toclevel": 1,
//       "level": "2",
//       "line": "Early life",
//       "number": "1",
//       "index": "1",
//       "fromtitle": "Thomas_Edison",
//       "byteoffset": 5451,
//       "anchor": "Early_life"
//     },
//     {
//       "toclevel": 1,
//       "level": "2",
//       "line": "Telegrapher",
//       "number": "2",
//       "index": "2",
//       "fromtitle": "Thomas_Edison",
//       "byteoffset": 11545,
//       "anchor": "Telegrapher"
//     },
//     {
//       "toclevel": 1,
//       "level": "2",
//       "line": "Marriages and children",
//       "number": "3",
//       "index": "3",
//       "fromtitle": "Thomas_Edison",
//       "byteoffset": 13139,
//       "anchor": "Marriages_and_children"
//     },
//]
void SearchBackend::parseIndex(QJsonArray m_jsonarr)
{

    QJsonArray jsonarr = m_jsonarr;
    int total = 0,i = 0;
    QString title("");
    QString strIndex = 0;

    total = jsonarr.count();

    QRegExp dot("\\.");
    for(i = 0; i < total; ++i)
    {
        QJsonObject sectionObj = jsonarr.at(i).toObject();
        title = sectionObj["line"].toString();
        strIndex = sectionObj["number"].toString();

        // string have '.'
        title = QString("%1 %2").arg(strIndex).arg(title);
        int pos = dot.indexIn(title);

        (pos > -1)? subTitles << title :Titles << title;
     }

}



void SearchBackend::parseH2(GumboNode *node)
{

    using Iterator = Gumbo::iterator;
    Iterator iter(node);
    const Iterator end;
   // QRegExp regH2Ex("</h2>\n<p>");

    auto tagH2 = findAll(iter.fromCurrent(), end, Tag(GUMBO_TAG_H2));

    for (size_t row = 0; row < tagH2.size(); ++row)
    {

        Iterator iter(tagH2[row]);

        while(iter != end )
        {
           if(iter->type == GUMBO_NODE_ELEMENT
                   && iter->v.element.tag == GUMBO_TAG_H2)
           {

              QString endTag = QString::fromUtf8(iter->v.document.system_identifier).left(60);

               tagH2List << endTag;

              qDebug() << row << "," << endTag;

           }

         ++iter;
        }
    }

    tagH2List.pop_front();

}



/// div class=\"thumb tright\"


void SearchBackend::parseH3(GumboNode *node)
{

    using Iterator = Gumbo::iterator;
    Iterator iter(node);
    const Iterator end;
    QRegExp regH3Ex("</h3>\n<p>");

    auto tagH3 = findAll(iter.fromCurrent(), end, Tag(GUMBO_TAG_H3));

    for (size_t row = 0; row < tagH3.size(); ++row)
    {

        Iterator iter(tagH3[row]);

        while(iter != end )
        {
           if(iter->type == GUMBO_NODE_ELEMENT
                   && iter->v.element.tag == GUMBO_TAG_H3)
           {

              QString startTag =QString::fromUtf8(iter->v.document.name).left(45);
              tagH3StartList << startTag;

              QString endTag = QString::fromUtf8(iter->v.document.system_identifier).left(100);
              tagH3List << endTag;

              qDebug() << row << "," << endTag;

           }

         ++iter;
        }
    }

}
void SearchBackend::parseDIV(GumboNode *node)
{

    using Iterator = Gumbo::iterator;
    Iterator iter(node);
    const Iterator end;
    QRegExp regDIV("</div>\n<p>");

    auto attrThumb = findAll(iter.fromCurrent(), end, And(Tag(GUMBO_TAG_DIV),
                                                      HasAttribute("class", "thumb tright")));


    for (size_t row = 0; row < attrThumb.size(); ++row)
    {

        Iterator iter(attrThumb[row]);

        while(iter != end )
        {

           if(iter->type == GUMBO_NODE_ELEMENT)
           {

               Element elDIV(*iter);

               if(QString(elDIV.attribute("class")->value) == "thumb tright")
               {

                QString startTag = QString::fromUtf8(iter->v.document.name).left(60);
                tagDIVStartList << startTag;

                QString endTag = QString::fromUtf8(iter->v.document.system_identifier).left(60);
                int pos = regDIV.indexIn(endTag);

                if(pos > -1)      // get position
                 {
                    endTag.remove(regDIV);
                    tagDIVList << endTag;

                 }

               //  qDebug() << row << "," << endTag;
               }


           }

           if(iter->type == GUMBO_NODE_WHITESPACE)
               break;

         ++iter;
        }
    }

}


/// compare text and subtitles

void SearchBackend::parseTable(GumboNode *node)
{
    using Iterator = Gumbo::iterator;
    Iterator iter(node);
    const Iterator end;

    auto tagTable = findAll(iter.fromCurrent(), end, Tag(GUMBO_TAG_TABLE));


    for (size_t row = 0; row < tagTable.size(); ++row)
    {

        Iterator iter(tagTable[row]);

        while(iter != end )
        {
           if(iter->type == GUMBO_NODE_ELEMENT
                   && iter->v.element.tag == GUMBO_TAG_TABLE)
           {

              QString startTag = QString::fromUtf8(iter->v.document.name).left(45);
              tagTableStartList << startTag;
              QString endTag = QString::fromUtf8(iter->v.document.system_identifier).left(45);
              tagTableEndList << endTag;

              qDebug() << row << "," << endTag;

           }

         ++iter;
        }
    }
}

void SearchBackend::insertStringList()
{
    filtertag->tagH2End = tagH2List;
    filtertag->tagH3Start = tagH3StartList;
    filtertag->tagH3End   = tagH3List;
    filtertag->tagDIVEnd = tagDIVList;
    filtertag->titleList = Titles;
    filtertag->subTitleList = subTitles;
    filtertag->tagTableEndList = tagTableEndList;

    filtertag->compareTag();
 //   pushH2Title();
    pushH3Title();

}


void SearchBackend::pushH2Title()
{
   int nSize = d->results.size();
   int i = 0;

   SearchResult m_search;

      // compare text <p> text and <h3> text

    for(i = 0; i < nSize; ++i)
    {
       for(int j = 0; j < filtertag->tagH2End.size(); ++j)
       {
          QRegExp exp(filtertag->tagH2End[j]);
         if(exp.indexIn(d->results[i].text) > -1)

         {
             m_search.text = "title";
             d->results.insert(i,m_search);

             ++i;

          }
       }
   }

  //replaceHeadlineWithValue();
}

void SearchBackend::pushH3Title()
{
   int nSize = d->results.size();
   int i = 0;

   SearchResult m_search;

   for(i = 0;  i < nSize; ++i)
   {

     for(int j = 0; j < filtertag->tagH3End.size(); ++j)
     {

        QRegExp exp(filtertag->tagH3End[j]);

        if(exp.indexIn(d->results[i].text) > -1)
        {
             m_search.text = "subtitle";
             d->results.insert(i,m_search);
             ++i;
        }
     }

   }



}


/// compare tag
///
///


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














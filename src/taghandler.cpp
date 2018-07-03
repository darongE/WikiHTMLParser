#include <QRegExp>

#include "taghandler.h"

/**
 * @brief TagHandler::TagHandler
 * compare tag and insert title
 */

// const SearchBackend::SearchResult::List searchResults
//= d->searchBackend->getResults();
TagHandler::TagHandler()
{


}

TagHandler::~TagHandler()
{

}

void TagHandler::compareTag()
{
// h2 tag

    int i = 0, j = 0;
    int nH2Tag = tagH2End.size();
    int nDIVTag = tagDIVEnd.size();
    int nH3Tag  = tagH3Start.size();

    QRegExp divExp("</h2>\n<div class=");
    QRegExp pExp("</h2>\n<p>");
    QRegExp h3Exp("</h2>\n<h3><span class=\"mw-headline\"");
    QRegExp trimmedH3(" id=");
    QRegExp tablerole("</h2>\n<table role");
    QRegExp tableclass ("</h2>\n<table class");

    // run loop </h2> ***
    for(i = 0; i < nH2Tag; ++i)
    {
        QString endTag = tagH2End[i];
        if(i == 0)
            tagH2End[0] = tagDIVEnd.front();
        else
        {
           if(pExp.indexIn(endTag) > -1)
              tagH2End[i].remove(pExp);
           else if(h3Exp.indexIn(endTag) > -1)
              tagH2End[i].remove(h3Exp);
        }

        qDebug() << "trimmed text" << tagH2End[i];

    }

    eraseTag(divExp, trimmedH3,tablerole, tableclass);

}


//    </h2>\n<table role=
//    </h2>\n<div class=\


void TagHandler::eraseTag(const QRegExp &divExp,const QRegExp &trimmedH3,
                          const QRegExp &tablerole, const QRegExp &tableclass)
{
    for(i = 0; i < nH2Tag; ++i)
    {
        if(!tagH2End[i].startsWith(divExp.pattern()) &&
                tagH2End[i].left(4) != trimmedH3.pattern() &&
                !tagH2End[i].startsWith(tablerole.pattern()) &&
                !tagH2End[i].startsWith(tableclass.pattern()) )

        {
             tagH2End[i] = titleList[idx++];


        }
        else
        {
            tagH2End.erase(tagH2End.begin() + i);
        }



    }

}

void TagHandler::pushH2Tag()
{

    const SearchBackend::SearchResult::List searchResults = d->searchBackend->getResults();

    int nSize = searchResults.size();


    for(int k = 0; k < nSize; ++k)
    {
//        QRegExp text(tagH2End[i])
//        if(searchResults[i].text)

    }




}

// push tag H2, H3 text



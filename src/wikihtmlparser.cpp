//#include <algorithm>
#include <QDebug>
#include <QString>
#include "wikihtmlparser.h"


using namespace EasyGumbo;
using namespace std;

#define RANGE(x) x->begin(), x->end()


// check "[", "]" and return value


WikiHTMLParser::WikiHTMLParser(const QByteArray &html)
{
   runParse(html);
}



void WikiHTMLParser::runParse(const QByteArray &html)
{
  Gumbo gumbo(html.data());
    if(gumbo){

       using Iterator = Gumbo::iterator;
       const Iterator end;

       const auto pages = findAll(gumbo.begin(), gumbo.end(), Tag(GUMBO_TAG_P));
       Iterator iter(pages[0]->parent);


       while(iter != end )
       {

           if (iter->type == GUMBO_NODE_TEXT)
            {
              QString data = QString::fromUtf8(iter->v.text.text);

               if(iter->v.text.start_pos.offset < 20){
                   nickname = data;
                   break;
               }
           }
           ++iter;
       }
    }
}




bool WikiHTMLParser::captureKeyword(QString &description)
{
    QRegExp rx_timestamp("stylized.*");
    int pos = rx_timestamp.indexIn(description);

     QString figure = "";

    if (pos > -1) {

        QStringList copiedSting = rx_timestamp.cap(0).split(QRegExp("(is)"), QString::SkipEmptyParts);
        QString test = copiedSting.at(0);
       // qDebug() << "stylized name" << test;

        if(test.contains("as"))
            figure = test.remove(QRegExp("stylized as")).trimmed().remove(QRegExp("[)]"));
        else
            figure =test.remove(QRegExp("stylized")).trimmed().remove(QRegExp("[,]"));

        description = figure;

        return true;
    }

    return false;

}

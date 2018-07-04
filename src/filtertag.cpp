#include <QDebug>

#include "filtertag.h"



FilterTag::FilterTag(QObject *parent) : QObject(parent)
{

}

FilterTag::~FilterTag()
{

}

void FilterTag::compareTag()
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

    trimmedH2Tag(divExp, trimmedH3,tablerole, tableclass);

}

// erase these tags
//        "</h2>\n<div class="
//        "</h2>\n<h3><span class=\"mw-headline\"
//        " id="
//        "</h2>\n<table role"
//        "</h2>\n<table class"

void FilterTag::trimmedH2Tag(const QRegExp &divExp,const QRegExp &trimmedH3,
                          const QRegExp &tablerole, const QRegExp &tableclass)
{
    int i = 0, idx = 0;

    QRegExp regexp("<a href=\"/wiki");


    for(i = 0; i < tagH2End.size(); ++i)
    {


        if(tagH2End[i].endsWith(regexp.pattern()))
             tagH2End[i].remove(regexp);


        if(tagH2End[i].startsWith(divExp.pattern()) ||
                tagH2End[i].startsWith(" id") ||
                tagH2End[i].startsWith(tablerole.pattern()) ||
                tagH2End[i].startsWith(tableclass.pattern())   )

        {


            tagH2End.erase(tagH2End.begin() + i);
            --i;
        }

    }

    trimmedH3Tag();
    trimmedTableTag();

}


void FilterTag::trimmedH3Tag()
{
    int i = 0;

    // erase tag H3
    QRegExp regexp("</h3>\n<p>");
    QRegExp h3DIVroleExp("</h3>\n<div role=");
    QRegExp h3DIVclassExp("</h3>\n<div class=\"thumb tright\">");
    QRegExp h3tableExp("</h3>\n<table class=\.*");
    QRegExp  h3hrefclass("href=\"/wiki/.*>(.*)</a>");

    for(i = 0; i < tagH3End.size(); ++i)
    {

        // erase DIV tag
        if(h3DIVroleExp.indexIn(tagH3End[i]) > -1 ||
             h3DIVclassExp.indexIn(tagH3End[i]) > -1 ||
                h3tableExp.indexIn(tagH3End[i]) > -1)
        {

            tagH3End.erase(tagH3End.begin() + i);
            if(i > 0){
                --i;
               continue;
            }


        }

        if(regexp.indexIn(tagH3End[i]) > -1)
            tagH3End[i].remove(regexp);

        int pos = h3hrefclass.indexIn(tagH3End[i]);

        if(pos > -1)
        {
           QString left = tagH3End[i].left(4);

           QString h3href = h3hrefclass.cap(1);
           tagH3End[i] = left + h3href;
        }


        qDebug() << "H3 trimmed text" << tagH3End[i];
    }

}

void FilterTag::trimmedTableTag()
{
  QRegExp tableExp("</table>\n<p>(.*)");
  QRegExp tagBExp("<b>(.*)</b>");
  QRegExp tableRoleTag("</table>\n<table role=\.* | </table>\n<ul>\n"
                       "| </table>\n<div class=\.* | </table>\n</div>\n");

  int i = 0;

  for(i = 0; i < tagTableEndList.size(); ++i)
  {
      if(tableRoleTag.indexIn(tagTableEndList[i]) > -1)
      {

          tagTableEndList.erase(tagTableEndList.begin() + i);
          if(i > 0){
              --i;
             continue;
          }

      }


      if(tableExp.indexIn(tagTableEndList[i]) > -1)
      {
          QString tabletext = tableExp.cap(1);

          if(tagBExp.indexIn(tabletext) > -1)
              tabletext = tagBExp.cap(1);

          tagTableEndList[i] = tabletext;

      }

      qDebug() << "Table trimmed text" << tagTableEndList[i];

  }
}












// push tag H2, H3 text


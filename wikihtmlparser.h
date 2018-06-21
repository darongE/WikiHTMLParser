#ifndef WIKIHTMLPARSER_H
#define WIKIHTMLPARSER_H


#include <QUrl>
#include <QByteArray>
#include <QVector>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include <QString>

#include "../deps/gumbo-parser/gumbo.h"
#include "gumbo.hpp"


//check Square Brace "[" ,"]"
// If value is TRUE ,skip value


class WikiHTMLParser
{

public:

    WikiHTMLParser(const QByteArray &html);
    ~WikiHTMLParser(){ }


   const QString& getnickname()
   {
      return nickname;
   }

   bool captureKeyword(QString &description);

public slots:

   void runParse(const QByteArray& html);
   void parseTitle(EasyGumbo::Gumbo *gumbo);


private:     // data can't show when debugging

    QString nickname;


};

#endif // WIKIHTMLPARSER_H

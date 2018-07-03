#ifndef SEARCHBACKEND_H
#define SEARCHBACKEND_H


#include <QObject>
#include <QNetworkReply>
#include "geocoordinates.h"
#include "gumbo.hpp"
/*
 * This code bring on https://github.com/KDE/digikam/
 *
 */

using namespace EasyGumbo;

class SearchBackend : public QObject
{
    Q_OBJECT

public:

    class SearchResult
    {
    public:
        explicit SearchResult()
        {

        }

        typedef QList<SearchResult> List;


        GeoCoordinates              coordinates;
        QString                     name;

        GeoCoordinates::Pair        boundingBox;
        QString                     index;
        QString                     country;
        QString                     text;
        QString                 contents;
        QString                     endTag;
        QUrl                        mUrl;

    };



public:

    explicit SearchBackend(QObject* const parent);
    ~SearchBackend();


public:



    bool search(const QString& backendName,const QString& searchTerm);

    SearchResult::List getResults() const;
    QString getErrorMessage() const;
    void findEndTag();
    void insertStringList();

    void compareTextAndSubtitles();
    void compareDIVNText();
  //  void replaceHeadlineWithValue();
    void replaceTitleWithValue();

signals:

    void signalSearchCompleted();

public slots:
   void slotFinished(QNetworkReply* reply);


public:
    class FilterTag;
private:
   void parseAll(const QByteArray &html);
   void parseText(GumboNode *node);

   void parseIndex(QJsonArray m_jsonarr);
   void parseH2(GumboNode *node);
   void parseH3(GumboNode *node);
   void parseDIV(GumboNode *node);
   void parseTable(GumboNode *node);


   class Private;
   Private* const d;


   QStringList Titles;
   QStringList subTitles;
   QStringList tagH2List;
   QStringList tagH3List;
   QStringList tagH3StartList;
   QStringList tagDIVStartList;
   QStringList tagDIVList;
   QStringList tagTableStartList;
   QStringList tagTableList;
};

#endif // SEARCHBACKEND_H














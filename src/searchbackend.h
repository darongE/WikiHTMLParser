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
        QUrl                        mUrl;

    };



public:

    explicit SearchBackend(QObject* const parent);
    ~SearchBackend();

    bool search(const QString& backendName,const QString& searchTerm);

    SearchResult::List getResults() const;
    QString getErrorMessage() const;
    void findEndTag(QString &tag, int row);

signals:

    void signalSearchCompleted();

public slots:
   void slotFinished(QNetworkReply* reply);


private:
   void parseAll(const QByteArray &html);
   void parseText(GumboNode *node);
   void parseTagAttributes(GumboNode *node);
   void findH2Tag(GumboNode *node);
private:
    class Private;
    Private* const d;

    QStringList subtitles;
};

#endif // SEARCHBACKEND_H














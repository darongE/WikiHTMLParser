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


signals:

    void signalSearchCompleted();

public slots:
   void slotFinished(QNetworkReply* reply);


private:
   void parse(QByteArray bytearray);
   void parseContents();
   void parseDetails();

private:
    class Private;
    Private* const d;
    std::shared_ptr<Gumbo> m_parser;
};

#endif // SEARCHBACKEND_H














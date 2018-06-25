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
   void parse(QByteArray &html);

private:
    class Private;
    Private* const d;
};

#endif // SEARCHBACKEND_H














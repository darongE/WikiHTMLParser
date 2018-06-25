#ifndef GEOCOORDINATES_H
#define GEOCOORDINATES_H

#include <QObject>
#include <QList>
#include <QPair>

class GeoCoordinates
{

public:

    enum HasFlag
    {
       HasNothing     = 0,
       HasLatitude    = 1,
       HasLongitude   = 2,
       HasCoordinates = 3
    };

    Q_DECLARE_FLAGS(HasFlags, HasFlag)

    typedef QList<GeoCoordinates>                  List;
    typedef QPair<GeoCoordinates, GeoCoordinates> Pair;
    typedef QList<GeoCoordinates::Pair>           PairList;

public:
    GeoCoordinates();
    GeoCoordinates(const double dLat, const double dLong);

    ~GeoCoordinates();

    bool operator ==(const GeoCoordinates& other) const;

    double lat() const;
    double lon() const;

    bool hasCoordinates() const;
    bool hasLatitude() const;
    bool hasLongitude() const;

    void setLatLon(const double dLat, const double dLon );

     HasFlags hasFlags() const;


     QString latString() const;
     QString lonString() const;

private:

     double   m_lat;
     double   m_lon;

    HasFlags m_hasFlags;
};

#endif // GEOCOORDINATES_H



/**

  "boundingbox": [
      "37.4285424",
      "37.7013911",
      "126.7641585",
      "127.1832695"
    ],


    boundingbox have north,

**/

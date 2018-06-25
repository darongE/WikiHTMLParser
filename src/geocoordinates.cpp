#include "geocoordinates.h"

GeoCoordinates::GeoCoordinates()
    : m_lat(0.0),
      m_lon(0.0),
      m_hasFlags(HasNothing)
{
}

GeoCoordinates::GeoCoordinates(const double inLat,
                               const double inLon)
    : m_lat(inLat),
      m_lon(inLon),
      m_hasFlags(HasCoordinates)
{
}

GeoCoordinates::~GeoCoordinates()
{

}


bool GeoCoordinates::operator ==(const GeoCoordinates& other) const
{
    return
            (hasCoordinates() == other.hasCoordinates() ) &&
            (hasCoordinates() ?((lat() == other.lat() ) && (lon() == other.lon() ))
                             :true);

}

double GeoCoordinates::lat() const
{
    return m_lat;
}

double GeoCoordinates::lon() const
{
    return m_lon;
}

bool GeoCoordinates::hasCoordinates() const
{
    return m_hasFlags.testFlag(HasCoordinates);
}

bool GeoCoordinates::hasLatitude() const
{
    return m_hasFlags.testFlag(HasLatitude);

}

bool GeoCoordinates::hasLongitude() const
{
    return m_hasFlags.testFlag(HasLongitude);
}

void GeoCoordinates::setLatLon(const double dLat, const double dLon)
{
    m_lat = dLat;
    m_lon = dLon;
    m_hasFlags |= HasCoordinates;
}


GeoCoordinates::HasFlags GeoCoordinates::hasFlags() const
{
    return m_hasFlags;
}


QString GeoCoordinates::latString() const
{
    return m_hasFlags.testFlag(HasLatitude)  ? QString::number(m_lat, 'g', 12)
                                             : QString();
}

QString GeoCoordinates::lonString() const
{
    return m_hasFlags.testFlag(HasLongitude) ? QString::number(m_lon, 'g', 12)
                                             : QString();
}


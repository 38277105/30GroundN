#include "mapviewtest_xjw.h"

MapViewTest_XJW::MapViewTest_XJW(QWidget *parent):
    mapcontrol::OPMapWidget(parent)
{

}
MapViewTest_XJW::~MapViewTest_XJW()
{

}
void MapViewTest_XJW::setConf(QSettings *conf)
{
    m_conf = conf;

    if( m_conf == NULL ) return;

    // map type & access mode
    {
        MapType::Types              mapType;
        core::AccessMode::Types     accessMode;
        QString                     cacheLocation;

        // load settings
        accessMode    = (core::AccessMode::Types) m_conf->value("mapWidget_accessMode",
                                                                (int)(core::AccessMode::CacheOnly)).toInt();
        mapType       = (MapType::Types) m_conf->value("mapWidget_mapType",
                                                       (int)(MapType::GoogleSatellite)).toInt();
        cacheLocation = m_conf->value("mapWidget_cacheLocation", "./data/").toString();

        // set configurations
        configuration->SetAccessMode(accessMode);
        configuration->SetCacheLocation(cacheLocation);
        SetMapType(mapType);

        // set accessMode actions
//        if( accessMode == core::AccessMode::ServerAndCache ) {
//            m_actMapAccess_ServerAndCache->setChecked(true);
//            m_actMapAccess_Cache->setChecked(false);
//        } else if ( accessMode == core::AccessMode::CacheOnly ) {
//            m_actMapAccess_ServerAndCache->setChecked(false);
//            m_actMapAccess_Cache->setChecked(true);
//        }
    }

    // home & safe area
//    {
//        m_homeShow = m_conf->value("mapWidget_home_show", m_homeShow).toInt();
//        m_homePos.SetLat(m_conf->value("mapWidget_home_lat", m_homePos.Lat()).toDouble());
//        m_homePos.SetLng(m_conf->value("mapWidget_home_lng", m_homePos.Lng()).toDouble());
//        m_homeAlt = m_conf->value("mapWidget_home_alt", m_homeAlt).toDouble();
//        m_homeSafearea = m_conf->value("mapWidget_home_safeArea", m_homeSafearea).toDouble();

//        m_flightHeight = m_conf->value("mapWidget_flightHeight", m_flightHeight).toDouble();

//        if( m_homeShow ) {
//            this->SetShowHome(true);
//            this->Home->SetCoord(m_homePos);
//            this->Home->SetAltitude( (int)(m_homeAlt) );
//            this->Home->SetSafeArea((int)(m_homeSafearea));

//            m_actHome_ShowHide->setChecked(true);
//        } else {
//            m_actHome_ShowHide->setChecked(false);
//        }
//    }
}

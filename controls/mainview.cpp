#include "mainview.h"
#include "mapviewtest_xjw.h"
#include <QHBoxLayout>

MainView::MainView(QWidget *parent) : QWidget(parent)
{
initMap();
}

void MainView::initMap()
{
  m_conf = new QSettings("./data/demo.ini", QSettings::IniFormat);
    // create OPMapWidget
    m_mapView = new MapViewTest_XJW(this);

    double  lat, lng;
    int     zoom;

    lat  = m_conf->value("lastPos_lat", 34.257287).toDouble();
    lng  = m_conf->value("lastPos_lng", 108.888931).toDouble();
    zoom = m_conf->value("lastZoom", 11).toInt();

    internals::PointLatLng p(lat, lng);
    m_mapView->SetCurrentPosition(p);
    m_mapView->SetZoom(zoom);
    m_mapView->setConf(m_conf);

    // overall layout
    QHBoxLayout *hl = new QHBoxLayout(this);
    this->setLayout(hl);

    hl->addWidget(m_mapView, 1);

}

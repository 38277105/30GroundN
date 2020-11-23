
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtWidgets/QCheckBox>
#include "MapWidgetContainer.h"
#include "qmessagebox.h"
#include "opmapcustom/homeeditor.h"
#include "Utils/zygroundglobalconfig.h"

MapWidgetContainer::MapWidgetContainer(QObject *parent) :
    QObject(parent)
{
    m_mapWidget=NULL;
    // create configure obj
    m_conf = new QSettings("data/demo.ini", QSettings::IniFormat);

    // setup layout
    setupLayout();
}

int MapWidgetContainer::setupLayout(void)
{
    // create OPMapWidget
    m_mapWidget = new MapWidget();

    double  lat, lng;
    int     zoom;

    lat  = m_conf->value("lastPos_lat", 39.957287).toDouble();
    lng  = m_conf->value("lastPos_lng", 119.888931).toDouble();
    zoom = m_conf->value("lastZoom", 11).toInt();

    internals::PointLatLng p(lat, lng);
    m_mapWidget->SetCurrentPosition(p);
    m_mapWidget->SetZoom(zoom);
    m_mapWidget->setConf(m_conf);

    m_uav = NULL;

//    //overall layout
//    QHBoxLayout *hl = new QHBoxLayout(this);
//    this->setLayout(hl);
//    hl->addWidget(m_mapWidget, 1);
//    this->startTimer(30);

    return 0;
}

void MapWidgetContainer::ShowUAV(bool value)
{
  m_mapWidget->SetShowUAV(value);
  m_mapWidget->SetUavPic("mapquad.png");
  m_mapWidget->AddUAV(0x0001);
}

MapWidget* MapWidgetContainer::GetWindow()
{
    if(m_mapWidget)
        return m_mapWidget;

}

void MapWidgetContainer::UpdateUAVState(internals::PointLatLng pos,double direction,int height)
{
    UAVItem* currentUAV=  m_mapWidget->GetUAV(0x0001);
    if(!currentUAV)
        currentUAV=m_mapWidget->AddUAV(0x001);

    currentUAV->SetUAVHeading(direction);
    /*********偏移逻辑**************/
//    pos.SetLat(pos.lat+ZYGroundGlobalConfig::offset_lat);
//    pos.SetLng(pos.lng+ZYGroundGlobalConfig::offset_lon);
     /*********偏移逻辑**************/

    currentUAV->SetUAVPos(pos,height);
//    currentUAV->SetUAVPos(pos, 0);
    //m_mapWidget->SetCurrentPosition(pos);//以无人机为中心
    if(ZYGroundGlobalConfig::setUavAsCenter==true)
    {
       m_mapWidget->SetCurrentPosition(pos);
    }
}

 void MapWidgetContainer::ShowMenu(bool value)
 {

 }

void MapWidgetContainer::actFatchMap(void)
{
    internals::RectLatLng  rect;

    rect = m_mapWidget->SelectedArea();
    if( rect.IsEmpty() ) {
        QMessageBox msgbox(QMessageBox::Warning,
                           "Warning", "Please select an area first!");
        msgbox.exec();

        return;
    }

    m_mapWidget->RipMap();
}


void MapWidgetContainer::moveUAVStateChanged(bool state)
{
    if( m_cbMoveUAV->checkState() == Qt::Checked ) {
        m_uav = m_mapWidget->AddUAV(0);
    } else {
        m_mapWidget->DeleteUAV(0);
        m_uav = NULL;
    }
}

void MapWidgetContainer::mapWidget_zoomChanged(int newZoom)
{
    printf(">>> zoom to: %d\n", newZoom);
}

void MapWidgetContainer::mapWidget_mousePressEvent(QMouseEvent *event)
{
    try
    {
        if( m_cbMoveUAV->checkState() == Qt::Checked ) {
            internals::PointLatLng p;

            p = m_mapWidget->currentMousePosition();
            printf("p  = %f %f\n", p.Lat(), p.Lng());

            if( m_uav != NULL ) m_uav->SetUAVPos(p, 460);
        }
    }
    catch(QString exception)
    {
        //QMessageBox::about(this,"Error",exception);
    }
}

void MapWidgetContainer::timerEvent(QTimerEvent *event)
{

}

void MapWidgetContainer::On_HomeSet_Click()
{
    m_mapWidget->actHome_Set();
    //m_mapWidget->actHome_ShowHide();
    homeEditor* editor=new homeEditor(m_mapWidget->Home);
    editor->show();
}

void MapWidgetContainer::slot_updateMapType(core::MapType::Types newMap){
    m_mapWidget->SetMapType(newMap);
}


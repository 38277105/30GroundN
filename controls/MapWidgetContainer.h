#ifndef QTESTWIN_MapWidgetContainer
#define QTESTWIN_MapWidgetContainer

#include <QWidget>
#include <QPushButton>
#include <QComboBox>

#include "opmapcontrol_ex/mapwidget/opmapwidget.h"
#include "MapWidget.h"
#include "internals/pointlatlng.h"


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class MapWidgetContainer : public QObject
{
    Q_OBJECT

public:
    explicit MapWidgetContainer(QObject *parent = 0);

    virtual int setupLayout(void);
    void ShowUAV(bool value);

    void ShowMenu(bool value);

    MapWidget* GetWindow();
    
signals:
    
public slots:
    void actFatchMap(void);

    void UpdateUAVState(internals::PointLatLng pos,double direction,int height);

    void moveUAVStateChanged(bool state);

    void mapWidget_zoomChanged(int newZoom);
    void mapWidget_mousePressEvent(QMouseEvent *event);

    virtual void timerEvent(QTimerEvent *event);

    void On_HomeSet_Click();

    void slot_updateMapType(core::MapType::Types newMap);

public:
    QSettings                   *m_conf;
    MapWidget                   *m_mapWidget;
    QPushButton                 *m_btnFatchMap;
    QCheckBox                   *m_cbMoveUAV;

    mapcontrol::UAVItem         *m_uav;
};

#endif // QTESTWIN_H

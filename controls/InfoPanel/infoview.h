#ifndef INFOVIEW_H
#define INFOVIEW_H

#include <QWidget>
#include "../flowlayout.h"
#include "vehicles/vehicle.h"
#include "infoviewitem.h"

class InfoView : public QWidget
{
    Q_OBJECT
public:
    explicit InfoView(QWidget *parent = 0);
    void resetXY();
signals:

public slots:
    void update(VehicleState& state);


private:
    FlowLayout*   m_flowLayout;

    InfoViewItem* m_lontitudeItem;
    InfoViewItem* m_latitudeItem;
    InfoViewItem* m_altitudeItem;
    InfoViewItem* m_x;
    InfoViewItem* m_y;
    InfoViewItem* m_z;
    InfoViewItem* m_rollItem;
    InfoViewItem* m_yawItem;
    InfoViewItem* m_pitchItem;
    InfoViewItem* m_satelliteCountItem;

    InfoViewItem* m_groundspeedItem;
    InfoViewItem* m_climbspeedItem;
    float st_X;
    float st_Y;
    float m_lastX;
    float m_lastY;
};

#endif // INFOVIEW_H

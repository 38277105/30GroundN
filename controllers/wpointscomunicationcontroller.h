#ifndef WPOINTSCOMUNICATIONCONTROLLER_H
#define WPOINTSCOMUNICATIONCONTROLLER_H

#include <QObject>
#include "connections/CommonStru.h"
#include "connections/mavlinkmessageinterface.h"
#include "vehicles/vehiclemanager.h"
#include "vehicles/vehicle.h"

class WPointsComunicationController : public QObject
{

    Q_OBJECT
public:
    explicit WPointsComunicationController(QObject *parent = 0);

signals:
   void On_Read_WayPoints(std::vector<mavlink_mission_item_int_t*> items);
public slots:
    void Write_WayPoints(std::vector<mavlink_mission_item_int_t*> items);
    void ReadWayPoints();

private:
//---------------------����Ϊ��·���յ��߼�-------------------------
    void MavMessageHandler(mavlink_message_t message);
    void send_WP_Request_List();
    void recv_MissionCount(mavlink_message_t msg);
    void send_WP_Request_Item_Int(int index);
    void recv_WP_Item_Int(mavlink_message_t msg);
    void send_WP_MissionAck();
//---------------------����Ϊ��·������߼�-------------------------
//---------------------����Ϊ��·���͵��߼�-------------------------

    void send_WP_MissionCount();
    void send_WP_Mission_int(int curIndex);

private:
    std::vector<mavlink_mission_item_int_t*> recv_items;
    std::vector<mavlink_mission_item_int_t*> send_items;
    int WP_TotalNum;
    int WP_CurrentIndex;
    int __targetSystem;
    int __targetComponent;
    int __systemId;
    int __componentId ;

    MavLinkMessageInterface* m_mavLinkMessageInterface;
};

#endif // WPOINTSCOMUNICATIONCONTROLLER_H

#ifndef MAVLINKMISSIONEXECUTANT_H
#define MAVLINKMISSIONEXECUTANT_H

#include <QObject>
#include <qcoreevent.h>
#include "connections/CommonStru.h"
#include <vector>
#include <qtimer.h>

class MavLinkMissionExecutant:public QObject
{
    Q_OBJECT
public:
    MavLinkMissionExecutant();

public:
    //读航路点列表
    void readMavWaypointList();
    //写航路点列表
   // int writeMavWaypointList(std::vector<mavlink_mission_item_int_t*> items);
    int writeMavWaypointList(std::vector<mavlink_mission_item_int_t*> items);

    //清除航路点列表
    void clearMavWaypointlist();
    //设置新的当前航路点
    void setNewCurrentMavWaypoit();

    //航路点数量
    void missionCount(mavlink_mission_count_t& count);
    //当前请求序号
    int getCurrentRequestSeq();
    //请求航路点
    void missionRequest();
    void missionRequest(mavlink_mission_request_int_t& request);
    //任务item
    void missionItem(mavlink_mission_item_int_t& item);
    void missionItem(uint16_t seq, mavlink_mission_item_int_t& item);

    void missionItem_int(mavlink_mission_item_int_t& item);
    void missionItem_int(uint16_t seq, mavlink_mission_item_int_t& item);
    //任务ack
    void missionAck(mavlink_mission_ack_t& ack);
    void missionAck();
    //当前任务
    void missionCurrent(mavlink_mission_current_t current);
signals:
    void onMissionList(std::vector<mavlink_mission_item_int_t*> items);
    void onMissionCurrent(uint16_t seq);
private slots:
    void __timerOut();
\
private:
    int __missionCount;
    int __currentRequestSeq;

    std::vector<mavlink_mission_item_int_t*> __items;
    QTimer __timer;
private:
    void __destroyItems();
};

#endif // MAVLINKMISSIONEXECUTANT_H

#include "mavlinkmissionexecutant.h"
#include <QDebug>

MavLinkMissionExecutant::MavLinkMissionExecutant()
{
    __currentRequestSeq = 0;
    connect(&__timer, SIGNAL(timeout()), this, SLOT(__timerOut()));
}

//
//读航路点列表
//
void MavLinkMissionExecutant::readMavWaypointList(){
    __timer.start(1000);
}

//
//写航路点列表
//
int MavLinkMissionExecutant::writeMavWaypointList(std::vector<mavlink_mission_item_int_t*> items){
    __items.clear();
    __currentRequestSeq = 0;
    std::copy(items.begin(), items.end(), std::back_inserter(__items));
    __timer.start(1000);
    return __missionCount = (int)items.size();
}

//
//清除航路点列表
//
void MavLinkMissionExecutant::clearMavWaypointlist(){

}

//
//设置新的当前航路点
//
void MavLinkMissionExecutant::setNewCurrentMavWaypoit(){

}

void MavLinkMissionExecutant::__timerOut(){
    std::vector<mavlink_mission_item_int_t*> items;
    mavlink_mission_item_int_t item;
    item.command = 0;
    items.push_back(&item);
//    emit onMissionList(__items);

    __timer.stop();
    __currentRequestSeq = 0;
    __destroyItems();
}

//
//航路点数量
//
void MavLinkMissionExecutant::missionCount(mavlink_mission_count_t& mmc){
    __missionCount = mmc.count;
    __timer.start();
}

//
//当前请求序号
//
int MavLinkMissionExecutant::getCurrentRequestSeq(){
    if( __missionCount <= __currentRequestSeq )
        return -1;
    return __currentRequestSeq;
}

//
//请求航路点
//
void MavLinkMissionExecutant::missionRequest(){

}
void MavLinkMissionExecutant::missionRequest(mavlink_mission_request_int_t& request){
    __currentRequestSeq = request.seq;
    __timer.start();
}

//
//任务item
//
void MavLinkMissionExecutant::missionItem(mavlink_mission_item_int_t& item){
    mavlink_mission_item_int_t* t = new mavlink_mission_item_int_t;

    t->param1 = item.param1;
    t->param2 = item.param2;
    t->param3 = item.param3;
    t->param4 = item.param4;
    t->x = item.x;
    t->y = item.y;
    t->z = item.z;
    t->seq = item.seq;
    t->command = item.command;
    t->target_system = item.target_system;
    t->target_component = item.target_component;
    t->frame = item.frame;
    t->current = item.current;
    t->autocontinue = item.autocontinue;
    __items.push_back(t);

    ++__currentRequestSeq;

    __timer.start();
    //qDebug("==============%d ==========(%f,%f,%f)",item.seq, item.x, item.y, item.z);
}
void MavLinkMissionExecutant::missionItem(uint16_t seq, mavlink_mission_item_int_t& item){
    if(seq>=__missionCount)return;
    item.param1 = __items[seq]->param1;
    item.param2 = __items[seq]->param2;
    item.param3 = __items[seq]->param3;
    item.param4 = __items[seq]->param4;
    item.x = __items[seq]->x;
    item.y = __items[seq]->y;
    item.z = __items[seq]->z;
    item.seq = __items[seq]->seq;
    item.command = __items[seq]->command;
    item.frame = __items[seq]->frame;
    item.current = __items[seq]->current;
    item.autocontinue = __items[seq]->autocontinue;
    __timer.start();
}


//任务ack
void MavLinkMissionExecutant::missionAck(mavlink_mission_ack_t& ack){
    //emit onMissionList(__items);
    __currentRequestSeq = 0;

    __destroyItems();
    __timer.stop();
}
void MavLinkMissionExecutant::missionAck(){
    emit onMissionList(__items);

    for(int i = 0 ; i < __items.size(); ++i){
        //qDebug("====%d======(%f,%f,%f)\n",__items.size(),__items[i]->x,__items[i]->y,__items[i]->z);
    }
    __currentRequestSeq = 0;
    __destroyItems();
    __timer.stop();
}

//当前任务
void MavLinkMissionExecutant::missionCurrent(mavlink_mission_current_t current){
    emit onMissionCurrent(current.seq);
}

//
//销毁__items;
//
void MavLinkMissionExecutant::__destroyItems(){
    mavlink_mission_item_int_t* item;
    for(int i = 0 ; i < __items.size(); ++i){
        item = __items[i];
        if( NULL != item){
            delete item;
            item = NULL;
        }
    }
    __items.clear();
}

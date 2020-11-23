#include "wpointscomunicationcontroller.h"

WPointsComunicationController::WPointsComunicationController(QObject *parent) : QObject(parent)
{
      WP_TotalNum=0;
      WP_CurrentIndex=0;

      __targetSystem=1;
      __targetComponent=0;

      __systemId = 0;
      __componentId = 0;
}

void WPointsComunicationController::MavMessageHandler(mavlink_message_t message)
{
    __targetSystem=message.sysid;
    __targetComponent=message.compid;

    switch (message.msgid)
    {
        //---------------------以下航路接收------------------------------
            case MAVLINK_MSG_ID_MISSION_COUNT:
            {//航路点数量
                recv_MissionCount(message);
                break;
            }
            case MAVLINK_MSG_ID_MISSION_ITEM_INT:
            {//航路点
                recv_WP_Item_Int(message);
                break;
            }
        //---------------------以上航路接收------------------------------
        //---------------------以下航路发送------------------------------
            case MAVLINK_MSG_ID_MISSION_ACK:{//航路点ack
                //停止发送
                break;
            }
            case MAVLINK_MSG_ID_MISSION_REQUEST:{
                mavlink_mission_request_t request;
                mavlink_msg_mission_request_decode(&message, &request);
                send_WP_Mission_int(request.seq);
                break;
            }
        //---------------------以上航路发送------------------------------
            default:
            {
                break;
            }
    }
}

void WPointsComunicationController::send_WP_Request_List()
{
    mavlink_mission_request_list_t mmrl;
    mmrl.target_system = __targetSystem;
    mmrl.target_component =__targetComponent;
    mavlink_message_t msg;
    mavlink_msg_mission_request_list_encode(__systemId, __componentId, &msg, &mmrl);
    m_mavLinkMessageInterface->sendMessage(msg);
}

void WPointsComunicationController::recv_MissionCount(mavlink_message_t msg)
{
    mavlink_mission_count_t mmc;
    mavlink_msg_mission_count_decode(&msg, &mmc);
    this->WP_TotalNum=mmc.count;

    recv_items.clear();//------------------------

    send_WP_Request_Item_Int(0);
}

void WPointsComunicationController::send_WP_Request_Item_Int(int index)
{
    if(index>=this->WP_TotalNum){
        send_WP_MissionAck();
        emit On_Read_WayPoints(recv_items);
        return;
    }
    mavlink_mission_request_t mmr;
    mmr.target_system =    __targetSystem;
    mmr.target_component = __targetComponent;
    mmr.seq = index;
    mavlink_message_t msg;
    mavlink_msg_mission_request_encode(__systemId, __componentId, &msg, &mmr);
    m_mavLinkMessageInterface->sendMessage(msg);
}

void WPointsComunicationController::recv_WP_Item_Int(mavlink_message_t msg)
{
    mavlink_mission_item_int_t missionItem;
    mavlink_msg_mission_item_int_decode(&msg, &missionItem);
    recv_items.push_back(&missionItem);

    WP_CurrentIndex++;//请求下一个航路点
    send_WP_Request_Item_Int(WP_CurrentIndex);
}

void WPointsComunicationController::send_WP_MissionAck()
{
    mavlink_mission_ack_t ack;
    ack.target_component = __targetComponent;
    ack.target_system = __targetSystem;
    ack.type = MAV_MISSION_ACCEPTED;
    mavlink_message_t msg;
    mavlink_msg_mission_ack_encode(__systemId, __componentId, &msg, &ack);
    m_mavLinkMessageInterface->sendMessage(msg);
}
//------------------------------以上为航路请求的逻辑-----------------------------------------------
//------------------------------以下为发送航路的逻辑-----------------------------------------------

void WPointsComunicationController::send_WP_MissionCount()
{
    mavlink_mission_count_t mmc;
    mmc.target_system = __targetSystem;
    mmc.target_component = __targetComponent;
    mmc.count = send_items.size();
    mavlink_message_t msg;
    mavlink_msg_mission_count_encode(__systemId, __componentId, &msg, &mmc);
    m_mavLinkMessageInterface->sendMessage(msg);
}

void WPointsComunicationController::send_WP_Mission_int(int curIndex)
{
     mavlink_mission_item_int_t* item= send_items[curIndex];
     item->target_system = __targetSystem;
     item->target_component = __targetComponent;
     mavlink_message_t msg;
     mavlink_msg_mission_item_int_encode(__systemId, __componentId, &msg, item);
     m_mavLinkMessageInterface->sendMessage(msg);
}
//------------------------------以上为发送航路的逻辑-----------------------------------------------

void WPointsComunicationController::Write_WayPoints(std::vector<mavlink_mission_item_int_t*> items)
{
    send_items.clear();
    WP_CurrentIndex = 0;
    std::copy(items.begin(), items.end(), std::back_inserter(send_items));
    send_WP_MissionCount();
}

void WPointsComunicationController::ReadWayPoints()
{
   send_WP_Request_List();
}

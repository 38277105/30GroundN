#ifndef VEHICLE_H
#define VEHICLE_H

#include <QObject>
#include "vehicles/vehiclestate.h"
#include "connections/communicationinterface.h"
//#include <common/mavlink.h>
#include "connections/CommonStru.h"
#include <connections/CommonStru.h>
#include "connections/mavlinkmessageinterface.h"
#include "connections/generalprotocolinterface.h"
class Vehicle : public QObject
{
    Q_OBJECT
public:
    explicit Vehicle(QObject *handle);
    ~Vehicle();

    void setLink(CommunicationInterface* Mci,CommunicationInterface* Sci);
    void checkConn();

    int sendMessage(const mavlink_message_t &message);

    int getVehicleState(VehicleState& state);

    void connectSlots(QObject* handle);
public:
     bool m_IsActive;
     VehicleState m_State;
     QString getName();
     bool    m_bUsedMainCi;
public:
     MavLinkMessageInterface mavLinkMessageInterface;
     //GeneralProtocolInterface* mavLinkInterface;
//
//信号槽形式的协议调用
//
private slots:
     //
     //数据流-------------------------------------------------------------------------------------------------
     //
     //所有数据
     void requestDataStreamAll(uint8_t start, uint16_t rate);
     //姿态原始数据
     void requestDataStreamRawSensors(uint8_t start, uint16_t rate);
     //请求 1.状态 24.GPS_ROW 42.当前任务 62.导航控制器输出
     void requestDataStreamExtendedStatus(uint8_t start, uint16_t rate);
     //MAV_DATA_STREAM_RC_CHANNELS
     void requestDataStreamRcChannels(uint8_t start, uint16_t rate);
     //MAV_DATA_STREAM_RAW_CONTROLLER
     void requestDataStreamRawController(uint8_t start, uint16_t rate);
     //位置
     void requestDataStreamPosition(uint8_t start, uint16_t rate);
     //MAV_DATA_STREAM_EXTRA1
     void requestDataStreamExtra1(uint8_t start, uint16_t rate);
     //MAV_DATA_STREAM_EXTRA2
     void requestDataStreamExtra2(uint8_t start, uint16_t rate);
     //MAV_DATA_STREAM_EXTRA3
     void requestDataStreamExtra3(uint8_t start, uint16_t rate);

     //飞行模式
     void setFlightMode(const uint8_t baseMode, const uint32_t customMode);


     //
     //命令----------------------------------------------------------------------------------------------------------
     //
     //命令-绕圈
     void commandLoiterUnlim(uint8_t autocontinue,float radius, float yaw, float longitude, float latitude , float altitude);
     void commandLoiterTurns(uint8_t confirmation,float truns, float radius, float yaw, float longitude, float latitude , float altitude);
     //命令-指导
     //命令-跟随
     //命令-自动
     //命令-返回起飞点
     void commandReturnToLaunch(uint8_t confirmation);
     //命令-前往设定高度
     void commandConditionChangeALT(uint8_t confirmation, float descent, float finishAlt );
     //命令-设定家位置
     void commandDoSetHome(uint8_t confirmation,float location, float longitude, float latitude , float altitude);
     //命令-获取家位置
     void commandGetHomePosition(uint8_t confirmation);
     //命令-设置参数
     void commandDoSetParameter(uint8_t confirmation,float paramNum, float paramValue);
     //命令-设定点着陆
     void commandNavLand(uint8_t confirmation,float yaw, float longitude, float latitude , float altitude);
     //命令-设定本地位置着陆点
     void commandNavLandLocal(uint8_t confirmation,float targetNumber, float maxOffset, float descend, float yaw, float x, float y, float z );
     //命令-引导限制
     void commandGuidedLimits(uint8_t confirmation,float timeout, float minAlt, float maxAlt, float 	horizontalLimit);
     //命令-航前设备校准
     void commandTriggerCalibration(uint8_t confirmation,float gyro, float magnetometer, float groundPressure, float radio, float accelerometer );
     //命令-改变速度
     void commandChangeSpeed(uint8_t confirmation,float type, float speed, float throttle);

     //参数请求读
     void paramRequestRead(char* paramId, int16_t index = -1);
     //参数请求列表
     void paramRequestList();
     //参数设置
     void paramSet(float paramValue, char* paramId, uint8_t paramType);

     //
     //航路点---------------------------------------------------------------------
     //
     //读航路点列表
     void readMavWaypointList();
     //写航路点列表
     void writeMavWaypointList(std::vector<mavlink_mission_item_int_t*> items);

     //清除航路点列表
     void clearMavWaypointlist();
     //设置新的当前航路点
     void setNewCurrentMavWaypoit( uint16_t seq );
};

#endif // VEHICLE_H

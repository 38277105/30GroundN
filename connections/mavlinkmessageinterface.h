#ifndef MAVLINKMESSAGEINTERFACE_H
#define MAVLINKMESSAGEINTERFACE_H

//#include <common/mavlink.h>
#include "connections/CommonStru.h"
#include <QDebug>
#include <list>
#include <time.h>
#include "connections/CommonStru.h"
#include "communicationinterface.h"
#include "mavlinkcommandexecutant.h"
#include "mavlinkmissionexecutant.h"
#include "generalprotocolinterface.h"


class MavLinkMessageInterface:public GeneralProtocolInterface
{
    Q_OBJECT
public:
    MavLinkMessageInterface();
    //
    //数据流-------------------------------------------------------------------------------------------------
    //
    //所有数据
    void requestDataStreamAll(uint8_t start, uint16_t rate);
    //#27 #29 #116
    void requestDataStreamRawSensors(uint8_t start, uint16_t rate);
    //#1 #24 #42 #62 #125
    void requestDataStreamExtendedStatus(uint8_t start, uint16_t rate);
    //#35 #36
    void requestDataStreamRcChannels(uint8_t start, uint16_t rate);
    //MAV_DATA_STREAM_RAW_CONTROLLER
    void requestDataStreamRawController(uint8_t start, uint16_t rate);
    //#33
    void requestDataStreamPosition(uint8_t start, uint16_t rate);
    //#30
    void requestDataStreamExtra1(uint8_t start, uint16_t rate);
    //#74
    void requestDataStreamExtra2(uint8_t start, uint16_t rate);
    //#2 #241
    void requestDataStreamExtra3(uint8_t start, uint16_t rate);


    //飞行模式
    void setFlightMode(const uint8_t& baseMode=NULL, const uint32_t& customMode=NULL);
    void getFlightMode(uint8_t& baseMode, uint32_t& customMode);

    bool getArmedState();


    //心跳
    void msgHeartbeatDecode(mavlink_message_t message);
    void msgHeartbeatEncode();

    //
    //命令----------------------------------------------------------------------------------------------------------
    //
    //命令-航路点
    uint16_t commandNavWaypoint(uint8_t autocontinue, float holdTime, float radius, float through, float yaw, float lat, float lon, float alt);
    //命令-绕圈
    uint16_t commandLoiterUnlim(uint8_t autocontinue,float radius, float yaw, float longitude, float latitude , float altitude);
    uint16_t commandLoiterTurns(uint8_t confirmation,float truns, float radius, float yaw, float longitude, float latitude , float altitude);
    //命令-指导
    //命令-跟随
    //命令-自动
    //命令-返回起飞点
    uint16_t commandReturnToLaunch(uint8_t confirmation);
    //命令-前往设定高度
    uint16_t commandConditionChangeALT(uint8_t confirmation, float descent, float finishAlt );
    //命令-设定家位置
    uint16_t commandDoSetHome(uint8_t confirmation,float location, float longitude, float latitude , float altitude);
    //命令-获取家位置
    uint16_t commandGetHomePosition(uint8_t confirmation);
    //命令-设置参数
    uint16_t commandDoSetParameter(uint8_t confirmation,float paramNum, float paramValue);
    //命令-设定点着陆
    uint16_t commandNavLand(uint8_t confirmation,float yaw, float longitude, float latitude , float altitude);
    //命令-设定本地位置着陆点
    uint16_t commandNavLandLocal(uint8_t confirmation,float targetNumber, float maxOffset, float descend, float yaw, float x, float y, float z );
    //命令-起飞、改变航向、改变高度
    uint16_t commandTakeOff(uint8_t confirmation,float yaw, float altitude);//
    //命令-悬停
    uint16_t commandLoiter(uint8_t confirmation,float yaw, float altitude);//
    //命令-引导限制
    uint16_t commandGuidedLimits(uint8_t confirmation,float timeout, float minAlt, float maxAlt, float 	horizontalLimit);


    void startCompassMotCalibration();

    //命令-航前设备校准
    uint16_t commandTriggerCalibration(uint8_t confirmation,float gyro, float magnetometer, float groundPressure, float radio, float accelerometer );
    //命令-改变速度
    uint16_t commandChangeSpeed(uint8_t confirmation,float type, float speed, float throttle);
    //命令-arm/disarm 上锁/解锁
    uint16_t commandComponentArmDisarm(uint8_t confirmation, float arm);
    //命令-设置模式
    uint16_t commandDoSetMode(uint8_t confirmation, float mode, float customMode, float customSubMode);
    //设置引导模式
    void setGuidedMode();
    //设置航向角
    void setGrdYaw(float yaw);
    //设置模式land
    void setLandMode();
    //命令-ACK
    void commandAck(mavlink_message_t message);

    void doCommand(MAV_CMD cmd,float p1, float p2, float p3, float p4, float p5, float p6, float p7);

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

    //--------------------SETMODE-----------------------------

    void setMode(uint8_t newBaseMode, uint32_t newCustomMode);


    void setModeArm(uint8_t newBaseMode, uint32_t newCustomMode);

    //新卓翼飞控--解锁加锁 1：解锁 0：加锁

    void setModeArm(int flag);

    //--------------------SETMODE----------------------------

    //发送摇杆控制数据
    int sendRC_channels_override(int ch1, int ch2, int ch3, int ch4,int ch5,int ch8);
    void setRCType(int flag);

    //Common Method
    void sendMessage(mavlink_message_t msg);

    //请求日志
    void logRequestList();
    //请求日志数据
    void logRequestData(uint16_t id, uint32_t ofs, uint32_t count);

    void getSystemStatus(uint32_t& sys_sts,uint16_t& gps_sts,uint16_t& rtk_sts,uint16_t& psirtk_sts);
private:
    //航路点数量
    void __waypointCount(mavlink_message_t& msg);
    void __WaypointCount(uint16_t count);
    //请求航路点
    void __waypointRequest();
    void __waypointRequest(mavlink_message_t& msg);
    //航路点
    void __waypoint(mavlink_message_t& msg);

    void __waypoint(uint16_t seq);
    //航路点ack
    void __waypointAck(mavlink_message_t& msg);
    void __waypointAck();
    //当前航路点
    void __waypointCurrent(mavlink_message_t& msg);

signals:

    void emit_mavlinkMessage(mavlink_message_t message);
	void onParamValue(mavlink_param_value_t paramValue);
    void updateCritActInfo(char *id, float value);
    void onStatusText(mavlink_statustext_t text);
    void onHomePosition(mavlink_home_position_t position);

    void calibrate_raw_imu_recved(mavlink_raw_imu_t raw_imu_data);
    void calibrate_scale_imu_recved(mavlink_scaled_imu_t raw_imu_data);
    void calibrate_scale_imu2_recved(mavlink_scaled_imu2_t raw_imu_data);
    void calibrate_scale_imu3_recved(mavlink_scaled_imu3_t raw_imu_data);

    void handleRCCalibration(mavlink_message_t msg);

    void logEntry_recved(mavlink_log_entry_t);
    void logData_recved(mavlink_log_data_t);
    void runninglog_recved(mavlink_message_t);
private slots:
    void switchMavlinkMsg(mavlink_message_t message);
public://协议外提供给vehicle的方法
    //位置
    void getPosition( float& latitude,float& lontitude,float& height);
    //位置
    void getRTKPosition( float& X,float& Y,float& Z);
    //姿态
    void getAttitude( float& roll, float& yaw, float& pitch);
    void getAttitude( float& roll, float& yaw, float& pitch,float& rollrate, float& yawrate, float& pitchrate);
    //获取信号强度
    __int64 getSignalIntensity();
    //获得VFR_HUD
    void getVfrHud(float& airSpeed, float& groundSpeed, int16_t& heading, uint16_t& throttle, float& alt, float& climb);

    void getGPSRawData(uint8_t& satelliteNum,uint8_t& fix_type,int32_t& lat,int32_t& lon,int32_t&alt);

    void getBatteryVolt(float& volt,float& batteryRemaining);
    void getExtentdStatus(float& MainVoltage,float& BakVoltage,float& Temperature,float ESC_Current[12]);
    void getConnectState(uint16_t& remoteConnectState);

    //2:QUAD(4axis)  13:HEXAROTOR(6axis) 14:OCTOROTOR(4axis8jiang)
    void getUAVType(uint8_t& type);
    void getRCType(bool& flag);//使用摇杆：flag= 1， 未使用摇杆，flag = 0
public:
    void link(bool bUseMain=true);
    void setLink(CommunicationInterface* mci,CommunicationInterface* sci);
    MavLinkCommandExecutant* getCommandHandle();
    MavLinkMissionExecutant* getMissionHandle();
    void sendHeartBeat();
    bool                _isCompassCalibration;
//mavlink property
private:
    MavlinkMessages __currentMessages;

    uint8_t             __baseMode;
    uint32_t            __customMode;
    bool                receivedMode;

    uint8_t             __systemId;
    uint8_t             __componentId;

    uint8_t             __targetSystem;
    uint8_t             __targetComponent;

    uint8_t             __systemStatus;
    uint8_t             __autopilotType;

    uint8_t             __mavlinkVersion;
    uint8_t             __type;

    uint8_t             __satelliteNum;
    float               __vspeed;


    CommunicationInterface* __Mainci;  //Main Link
    CommunicationInterface* __Slaveci; //Slave Link;
    MavLinkCommandExecutant __commandExecutant;
    MavLinkMissionExecutant __missionExecutant;

    time_t __signalIntensity; //信号强度
//mavlink用到的一些方法
private:

    void __messageCopy( mavlink_message_t& targetMsg, mavlink_message_t& sourceMsg);
    void __updateMavlinkProperty(mavlink_message_t msg);
    void __paramValue(mavlink_message_t msg);
    void __infoCritAct(mavlink_message_t msg);
    void __statusText(mavlink_message_t msg);
    void __homePosition(mavlink_message_t message);
    void __gpsRawData(mavlink_message_t message);

};

#endif // MAVLINKMESSAGEINTERFACE_H

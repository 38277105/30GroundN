#include "vehicle.h"
#include "qmath.h"
#include "../Utils/zygroundglobalconfig.h"

#define MAX_OUTTIME 5
int i = qRegisterMetaType<mavlink_param_value_t>("mavlink_param_value_t");

int j = qRegisterMetaType<mavlink_statustext_t>("mavlink_statustext_t");
Vehicle::Vehicle(QObject *handle)
{
   m_IsActive=false;
}

Vehicle::~Vehicle()
{

}



void Vehicle::setLink(CommunicationInterface *Mci, CommunicationInterface *Sci)
{
    mavLinkMessageInterface.setLink(Mci,Sci);
    m_bUsedMainCi=true;
}

//
//
//
void Vehicle::connectSlots(QObject* handle){

    //所有数据
    connect(handle, SIGNAL(requestDataStreamAll(uint8_t, uint16_t)), this, SLOT(requestDataStreamAll(uint8_t, uint16_t)));
    //姿态原始数据
    connect(handle, SIGNAL(requestDataStreamRawSensors(uint8_t, uint16_t)), this, SLOT(requestDataStreamRawSensors(uint8_t, uint16_t)));
    //请求 1.状态 24.GPS_ROW 42.当前任务 62.导航控制器输出
    connect(handle, SIGNAL(requestDataStreamExtendedStatus(uint8_t, uint16_t)), this, SLOT(requestDataStreamExtendedStatus(uint8_t, uint16_t)));
    //MAV_DATA_STREAM_RC_CHANNELS
    connect(handle, SIGNAL(requestDataStreamRcChannels(uint8_t, uint16_t)), this, SLOT(requestDataStreamRcChannels(uint8_t, uint16_t)));
    //MAV_DATA_STREAM_RAW_CONTROLLER
    connect(handle, SIGNAL(requestDataStreamRawController(uint8_t, uint16_t)), this, SLOT(requestDataStreamRawController(uint8_t, uint16_t)));
    //位置
    connect(handle, SIGNAL(requestDataStreamPosition(uint8_t, uint16_t)), this, SLOT(requestDataStreamPosition(uint8_t, uint16_t)));
    //MAV_DATA_STREAM_EXTRA1
    connect(handle, SIGNAL(requestDataStreamExtra1(uint8_t, uint16_t)), this, SLOT(requestDataStreamExtra1(uint8_t, uint16_t)));
    //MAV_DATA_STREAM_EXTRA2
    connect(handle, SIGNAL(requestDataStreamExtra2(uint8_t, uint16_t)), this, SLOT(requestDataStreamExtra2(uint8_t, uint16_t)));
    //MAV_DATA_STREAM_EXTRA3
    connect(handle, SIGNAL(requestDataStreamExtra3(uint8_t, uint16_t)), this, SLOT(requestDataStreamExtra3(uint8_t, uint16_t)));
    //飞行模式
    connect(handle, SIGNAL(setFlightMode(const uint8_t, const uint32_t)), this, SLOT(setFlightMode(const uint8_t, const uint32_t)));
    //命令-绕圈
    connect(handle, \
            SIGNAL(commandLoiterUnlim(uint8_t,float, float, float, float, float)), \
            this, \
            SLOT(commandLoiterUnlim(uint8_t,float, float, float, float, float)));
    connect(handle, \
            SIGNAL(commandLoiterTurns(uint8_t,float, float, float, float, float, float)),
            this, \
            SLOT(commandLoiterTurns(uint8_t,float, float, float, float, float, float)));
    //命令-返回起飞点
    connect(handle, SIGNAL(commandReturnToLaunch(uint8_t)), this, SLOT(commandReturnToLaunch(uint8_t)));
    //命令-前往设定高度
    connect(handle, SIGNAL(commandConditionChangeALT(uint8_t, float, float)), this, SLOT(commandConditionChangeALT(uint8_t, float, float)));
    //命令-设定家位置
    connect(handle, \
            SIGNAL(commandDoSetHome(uint8_t ,float, float, float, float)), \
            this, \
            SLOT(commandDoSetHome(uint8_t ,float, float, float, float)));
    //命令-获取家位置
    connect(handle, SIGNAL(commandGetHomePosition(uint8_t)), this, SLOT(commandGetHomePosition(uint8_t)));
    //命令-设置参数
    connect(handle, \
            SIGNAL(commandDoSetParameter(uint8_t,float, float)), \
            this, \
            SLOT(commandDoSetParameter(uint8_t,float, float)));
    //命令-设定点着陆
    connect(handle, \
            SIGNAL(commandNavLand(uint8_t,float, float, float, float)), \
            this, \
            SLOT(commandNavLand(uint8_t,float, float, float, float)));
    //命令-设定本地位置着陆点
    connect(handle, \
            SIGNAL(commandNavLandLocal(uint8_t,float, float, float, float, float, float, float)), \
            this, \
            SLOT(commandNavLandLocal(uint8_t,float, float, float, float, float, float, float)));
    //命令-起飞（手抛/地面起飞）
    connect(handle, \
            SIGNAL(commandTakeOff(uint8_t,float, float, float, float, float)), \
            this, \
            SLOT(commandTakeOff(uint8_t,float, float, float, float, float)));
    //命令-引导限制
    connect(handle, \
            SIGNAL(commandGuidedLimits(uint8_t,float, float, float, float)), \
            this, \
            SLOT(commandGuidedLimits(uint8_t,float, float, float, float)));
    //命令-航前设备校准
    connect(handle, \
            SIGNAL(commandTriggerCalibration(uint8_t,float, float, float, float, float)),\
            this,\
            SLOT(commandTriggerCalibration(uint8_t,float, float, float, float, float)));
    //命令-改变速度
    connect(handle, \
            SIGNAL(commandChangeSpeed(uint8_t,float, float, float)),\
            this, \
            SLOT(commandChangeSpeed(uint8_t,float, float, float)));
    //参数请求读
    connect(handle, \
            SIGNAL(paramRequestRead(char*, int16_t)), \
            this, \
            SLOT(paramRequestRead(char*, int16_t)));
    //参数请求列表
    connect(handle, SIGNAL(paramRequestList()), this, SLOT(paramRequestList()));
    //参数设置
    connect(handle, \
            SIGNAL(paramSet(float, char*, uint8_t)), \
            this, \
            SLOT(paramSet(float, char*, uint8_t)));
    //读航路点列表
    connect(handle, SIGNAL(readMavWaypointList()), this, SLOT(readMavWaypointList()));
    //写航路点列表
    connect(handle, SIGNAL(writeMavWaypointList(std::vector<mavlink_mission_item_int_t*>)), \
            this, SLOT(writeMavWaypointList(std::vector<mavlink_mission_item_int_t*>)));
    //清除航路点列表
    connect(handle, SIGNAL(clearMavWaypointlist()), this, SLOT(clearMavWaypointlist()));
    //设置新的当前航路点
    connect(handle, SIGNAL(setNewCurrentMavWaypoit( uint16_t)), this, SLOT(setNewCurrentMavWaypoit( uint16_t)));
}


QString Vehicle::getName()
{
    QString str_Name;

    str_Name= str_Name.sprintf("%s,%03d\n", "无人机", m_State.m_Id);
//    str_Name += QString::fromLocal8Bit("无人机");
//    str_Name += QString::number(m_State.m_Id);
    return str_Name;
}

void Vehicle::checkConn()
{
    if(!ZYGroundGlobalConfig::m_bUsedSlavePort)
        return;
    int dwMS=mavLinkMessageInterface.getSignalIntensity();
    //qDebug()<<"get main port dw="<<dwMS;
    if(m_bUsedMainCi && dwMS>MAX_OUTTIME) //切换到备用串口
    {
        qDebug()<<"change slave port";
        mavLinkMessageInterface.link(false);
        m_bUsedMainCi=false;
    }
    else if(!m_bUsedMainCi && dwMS<MAX_OUTTIME)//恢复到主串口
    {
        qDebug()<<"change main port";
        mavLinkMessageInterface.link();
        m_bUsedMainCi=true;
    }
}

//
//得到飞机状态
//
int Vehicle::getVehicleState(VehicleState& state){
    float latitude, lontitude,height;
    mavLinkMessageInterface.getPosition(latitude,lontitude,height);//接口已经修改，
    latitude=latitude/10000000;
    lontitude=lontitude/10000000;
    height=height/1000;

    uint8_t starNum;
    uint8_t fixType;
    int32_t GPS_lat;
    int32_t GPS_lon;
    int32_t GPS_alt;
    mavLinkMessageInterface.getGPSRawData(starNum,fixType, GPS_lat, GPS_lon, GPS_alt);
    state.fix_type=fixType;
//    state.X =(double)GPS_lat;
//    state.Y =(double)GPS_lon;
//    state.Z =(double)GPS_alt;
    float RTK_X;
    float RTK_Y;
    float RTK_Z;
    mavLinkMessageInterface.getRTKPosition(RTK_X, RTK_Y, RTK_Z);
    state.X =(double)RTK_X;
    state.Y =(double)RTK_Y;
    state.Z =(double)RTK_Z;

    float roll, yaw, pitch;
    mavLinkMessageInterface.getAttitude(roll,yaw,pitch);

    roll=roll*180/M_PI;
    yaw=yaw*180/M_PI;
    pitch=pitch*180/M_PI;


    state.latitude = latitude;
    state.lontitude = lontitude;
    state.height = height;//相对高度

    state.roll = roll;
    state.yaw = yaw;
    state.pitch = pitch;

    state.starNum=starNum;


    float airspeed,groundspeed,alt,climb;
    int16_t heading;
    uint16_t throttle;

    mavLinkMessageInterface.getVfrHud(airspeed,groundspeed,heading,throttle,alt,climb);

    state.airspeed=airspeed;
    state.groundspeed=groundspeed;
    state.heading  =heading;
    state.throttle =throttle;
    //state.height   =alt;//海拔高度
    state.climb    =climb;
    state.vspeed=climb;

    float voltage,batteryRemaining;
    mavLinkMessageInterface.getBatteryVolt(voltage,batteryRemaining);
    mavLinkMessageInterface.getRCType(state.isJS);
    state.batteryVolt=voltage;
    state.batterypercent=batteryRemaining; 
    state.signalValue=mavLinkMessageInterface.getSignalIntensity();

    mavLinkMessageInterface.getExtentdStatus(state.MainVoltage,state.BakVoltage,state.Temperature,state.ESC_Current);
    return 0;
}

//
//data stream--------------------------------------------------------------------------------------------------
//
//所有数据
void Vehicle::requestDataStreamAll(uint8_t start, uint16_t rate){
    mavLinkMessageInterface.requestDataStreamAll(start,rate);
}
//姿态原始数据
void Vehicle::requestDataStreamRawSensors(uint8_t start, uint16_t rate){
    mavLinkMessageInterface.requestDataStreamRawSensors(start,rate);
}
//请求 1.状态 24.GPS_ROW 42.当前任务 62.导航控制器输出
void Vehicle::requestDataStreamExtendedStatus(uint8_t start, uint16_t rate){
    mavLinkMessageInterface.requestDataStreamExtendedStatus(start,rate);
}
//MAV_DATA_STREAM_RC_CHANNELS
void Vehicle::requestDataStreamRcChannels(uint8_t start, uint16_t rate){
    mavLinkMessageInterface.requestDataStreamRcChannels(start,rate);
}
//MAV_DATA_STREAM_RAW_CONTROLLER
void Vehicle::requestDataStreamRawController(uint8_t start, uint16_t rate){
    mavLinkMessageInterface.requestDataStreamRawController(start,rate);
}
//位置
void Vehicle::requestDataStreamPosition(uint8_t start, uint16_t rate){
    mavLinkMessageInterface.requestDataStreamPosition(start,rate);
}
//MAV_DATA_STREAM_EXTRA1
void Vehicle::requestDataStreamExtra1(uint8_t start, uint16_t rate){
    mavLinkMessageInterface.requestDataStreamExtra1(start,rate);
}
//MAV_DATA_STREAM_EXTRA2
void Vehicle::requestDataStreamExtra2(uint8_t start, uint16_t rate){
    mavLinkMessageInterface.requestDataStreamExtra2(start,rate);
}
//MAV_DATA_STREAM_EXTRA3
void Vehicle::requestDataStreamExtra3(uint8_t start, uint16_t rate){
    mavLinkMessageInterface.requestDataStreamExtra3(start,rate);
}

//飞行模式
void Vehicle::setFlightMode(const uint8_t baseMode, const uint32_t customMode){
    mavLinkMessageInterface.setFlightMode(baseMode,customMode);
}

//
//命令----------------------------------------------------------------------------------------------------------
//
//命令-绕圈
void Vehicle::commandLoiterUnlim(uint8_t autocontinue,float radius, float yaw, float longitude, float latitude , float altitude){
    mavLinkMessageInterface.commandLoiterUnlim(autocontinue, radius, yaw, longitude, latitude , altitude);
}
void Vehicle::commandLoiterTurns(uint8_t confirmation,float truns, float radius, float yaw, float longitude, float latitude , float altitude){
    mavLinkMessageInterface.commandLoiterTurns( confirmation, truns, radius, yaw, longitude, latitude , altitude);
}
//命令-返回起飞点
void Vehicle::commandReturnToLaunch(uint8_t confirmation){
    mavLinkMessageInterface.commandReturnToLaunch(confirmation);
}
//命令-前往设定高度
void Vehicle::commandConditionChangeALT(uint8_t confirmation, float descent, float finishAlt ){
    mavLinkMessageInterface.commandConditionChangeALT( confirmation, descent, finishAlt );
}
//命令-设定家位置
void Vehicle::commandDoSetHome(uint8_t confirmation,float location, float longitude, float latitude , float altitude){
    mavLinkMessageInterface.commandDoSetHome(confirmation,location, longitude, latitude , altitude);
}
//命令-获取家位置
void Vehicle::commandGetHomePosition(uint8_t confirmation){
    mavLinkMessageInterface.commandGetHomePosition(confirmation);
}
//命令-设置参数
void Vehicle::commandDoSetParameter(uint8_t confirmation,float paramNum, float paramValue){
    mavLinkMessageInterface.commandDoSetParameter(confirmation, paramNum, paramValue);
}
//命令-设定点着陆
void Vehicle::commandNavLand(uint8_t confirmation,float yaw, float longitude, float latitude , float altitude){
    mavLinkMessageInterface.commandNavLand(confirmation, yaw, longitude, latitude , altitude);
}
//命令-设定本地位置着陆点
void Vehicle::commandNavLandLocal(uint8_t confirmation,float targetNumber, float maxOffset, float descend, float yaw, float x, float y, float z ){
    mavLinkMessageInterface.commandNavLandLocal( confirmation, targetNumber, maxOffset, descend, yaw, x, y, z );
}

//命令-引导限制
void Vehicle::commandGuidedLimits(uint8_t confirmation,float timeout, float minAlt, float maxAlt, float 	horizontalLimit){
    mavLinkMessageInterface.commandGuidedLimits(confirmation, timeout, minAlt, maxAlt, horizontalLimit);
}
//命令-航前设备校准
void Vehicle::commandTriggerCalibration(uint8_t confirmation,float gyro, float magnetometer, float groundPressure, float radio, float accelerometer ){
    mavLinkMessageInterface.commandTriggerCalibration( confirmation, gyro, magnetometer, groundPressure, radio, accelerometer );
}
//命令-改变速度
void Vehicle::commandChangeSpeed(uint8_t confirmation,float type, float speed, float throttle){
    mavLinkMessageInterface.commandChangeSpeed( confirmation, type, speed, throttle);
}

//参数请求读
void Vehicle::paramRequestRead(char* paramId, int16_t index){
    mavLinkMessageInterface.paramRequestRead( paramId, index);
}
//参数请求列表
void Vehicle::paramRequestList(){
    mavLinkMessageInterface.paramRequestList();
}
//参数设置
void Vehicle::paramSet(float paramValue, char* paramId, uint8_t paramType){
    mavLinkMessageInterface.paramSet( paramValue, paramId, paramType);
}

//
//航路点---------------------------------------------------------------------
//
//读航路点列表
void Vehicle::readMavWaypointList(){
    mavLinkMessageInterface.readMavWaypointList();
}
//写航路点列表
void Vehicle::writeMavWaypointList(std::vector<mavlink_mission_item_int_t*> items){
    mavLinkMessageInterface.writeMavWaypointList(items);
}

//清除航路点列表
void Vehicle::clearMavWaypointlist(){
    mavLinkMessageInterface.clearMavWaypointlist();
}
//设置新的当前航路点
void Vehicle::setNewCurrentMavWaypoit( uint16_t seq ){
    mavLinkMessageInterface.setNewCurrentMavWaypoit( seq );
}

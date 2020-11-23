#include "mavlinkmessageinterface.h"
#include "loghelper/logger.h"
#include "controllers/frmmaincontroller.h"
#include "./Utils/zygroundglobalconfig.h"

MavLinkMessageInterface::MavLinkMessageInterface()
{
    __Mainci = NULL;
    __Slaveci = NULL;
    __systemId = 0;
    __componentId = 0;
    __targetSystem = 1;
    __targetComponent = 0;

    __signalIntensity = time(NULL);
    receivedMode=false;
    _isCompassCalibration = false;
    memset(&__currentMessages,0,sizeof(MavlinkMessages));
}

void MavLinkMessageInterface::switchMavlinkMsg(mavlink_message_t message){

    emit emit_mavlinkMessage(message);
    //qDebug("==============message id %i\n",message.msgid);
    //emit runninglog_recved(message);
//    if(_isCompassCalibration){
//        if(message.msgid == MAVLINK_MSG_ID_RAW_IMU | message.msgid == MAVLINK_MSG_ID_SCALED_IMU |\
//                message.msgid == MAVLINK_MSG_ID_SCALED_IMU2 | message.msgid == MAVLINK_MSG_ID_SCALED_IMU3){

//        }else{
//            return;
//        }
//    }
    //qDebug("==============message id %i\n",message.msgid);
    //qDebug("==============message id %i\n",message.msgid);
    __signalIntensity = time(NULL);
    switch (message.msgid){
        //FrmMainController::Instance()->m_lockData->reset();
        case MAVLINK_MSG_ID_HEARTBEAT:
        {
            mavlink_heartbeat_t heartbeat;
            mavlink_msg_heartbeat_decode(&message, &heartbeat);
            __updateMavlinkProperty(message);
            break;
        }

        case MAVLINK_MSG_ID_SYS_STATUS:
        {
           // printf("MAVLINK_MSG_ID_SYS_STATUS\n");
            mavlink_msg_sys_status_decode(&message, &(__currentMessages.sys_status));
            break;
        }

        case MAVLINK_MSG_ID_BATTERY_STATUS:
        {
          //  printf("MAVLINK_MSG_ID_BATTERY_STATUS\n");
            //mavlink_msg_battery_status_decode(&message, &(current_messages.battery_status));
            //current_messages.time_stamps.battery_status = get_time_usec();
            //this_timestamps.battery_status = current_messages.time_stamps.battery_status;
            break;
        }

        case MAVLINK_MSG_ID_RADIO_STATUS:
        {
           // printf("MAVLINK_MSG_ID_RADIO_STATUS\n");
            //mavlink_msg_radio_status_decode(&message, &(current_messages.radio_status));
            //current_messages.time_stamps.radio_status = get_time_usec();
            //this_timestamps.radio_status = current_messages.time_stamps.radio_status;
            break;
        }

        case MAVLINK_MSG_ID_LOCAL_POSITION_NED:
        {
           // printf("MAVLINK_MSG_ID_LOCAL_POSITION_NED\n");
            mavlink_msg_local_position_ned_decode(&message, &(__currentMessages.local_position_ned));
            break;
        }

        case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
        {
            //printf("MAVLINK_MSG_ID_GLOBAL_POSITION_INT\n");
            mavlink_msg_global_position_int_decode(&message, &(__currentMessages.global_position_int));
            break;
        }

        case MAVLINK_MSG_ID_POSITION_TARGET_LOCAL_NED:
        {
           // printf("MAVLINK_MSG_ID_POSITION_TARGET_LOCAL_NED\n");
            //mavlink_msg_position_target_local_ned_decode(&message, &(current_messages.position_target_local_ned));
            //current_messages.time_stamps.position_target_local_ned = get_time_usec();
            //this_timestamps.position_target_local_ned = current_messages.time_stamps.position_target_local_ned;
            break;
        }

        case MAVLINK_MSG_ID_POSITION_TARGET_GLOBAL_INT:
        {
           // printf("MAVLINK_MSG_ID_POSITION_TARGET_GLOBAL_INT\n");
            //mavlink_msg_position_target_global_int_decode(&message, &(current_messages.position_target_global_int));
            //current_messages.time_stamps.position_target_global_int = get_time_usec();
            //this_timestamps.position_target_global_int = current_messages.time_stamps.position_target_global_int;
            break;
        }
        case MAVLINK_MSG_ID_NAV_CONTROLLER_OUTPUT :{//导航控制器输出
            break;
        }
        case MAVLINK_MSG_ID_HIGHRES_IMU:
        {
            //  printf("MAVLINK_MSG_ID_HIGHRES_IMU\n");
            //mavlink_msg_highres_imu_decode(&message, &(current_messages.highres_imu));
            //current_messages.time_stamps.highres_imu = get_time_usec();
            //this_timestamps.highres_imu = current_messages.time_stamps.highres_imu;
            break;
        }
        case MAVLINK_MSG_ID_ATTITUDE://姿态
        {
           // printf("MAVLINK_MSG_ID_ATTITUDE\n");
            mavlink_msg_attitude_decode(&message, &(__currentMessages.attitude));
            break;
        }
        case MAVLINK_MSG_ID_COMMAND_INT:{
           // printf("MAVLINK_MSG_ID_COMMAND_INT\n");
            mavlink_command_int_t command;
            mavlink_msg_command_int_decode(&message, &command);
            break;
        }
        case MAVLINK_MSG_ID_COMMAND_LONG:
        {
            mavlink_command_long_t command;
            mavlink_msg_command_long_decode(&message, &command);
            qDebug()<<"recv long cmd="<<command.command<<command.param1<<command.param2;
            break;
        }
        case MAVLINK_MSG_ID_COMMAND_ACK:{
            commandAck(message);
            break;
        }
        case MAVLINK_MSG_ID_MISSION_COUNT:{//航路点数量
            __waypointCount(message);
//            qDebug("==============message id %i\n",message.msgid);
            break;
        }
        case MAVLINK_MSG_ID_MISSION_ITEM_INT:{//航路点
           __waypoint(message);
          //   qDebug("==============message id %i\n",message.msgid);
            break;
        }
        case MAVLINK_MSG_ID_MISSION_ACK:{//航路点ack
            __waypointAck(message);
          //   qDebug("==============message id %i\n",message.msgid);
            break;
        }
        case MAVLINK_MSG_ID_MISSION_REQUEST:{
            __waypointRequest(message);
           //  qDebug("==============message id %i\n",message.msgid);
            break;
        }
    case MAVLINK_MSG_ID_MISSION_REQUEST_INT:{
        __waypointRequest(message);
       //  qDebug("==============message id %i\n",message.msgid);
        break;
    }
        case MAVLINK_MSG_ID_MISSION_CURRENT:{//当前任务
            __waypointCurrent(message);
           //  qDebug("==============message id %i\n",message.msgid);
        }

        case MAVLINK_MSG_ID_RAW_IMU:{
            //__messageCopy(__currentMessages.scaledImu, message);
            mavlink_raw_imu_t raw_imu;
            mavlink_msg_raw_imu_decode(&message,&raw_imu);
            emit calibrate_raw_imu_recved(raw_imu);
            break;
        }
        case MAVLINK_MSG_ID_SCALED_IMU:{
            //__messageCopy(__currentMessages.scaledImu, message);
            //qDebug() << "scal emit";
            mavlink_scaled_imu_t scaled_imu;
            mavlink_msg_scaled_imu_decode(&message,&scaled_imu);
            emit calibrate_scale_imu_recved(scaled_imu);
            break;
        }
        case MAVLINK_MSG_ID_SCALED_IMU2:{
            //__messageCopy(__currentMessages.scaledImu, message);
            mavlink_scaled_imu2_t scaled_imu;
            mavlink_msg_scaled_imu2_decode(&message,&scaled_imu);
            emit calibrate_scale_imu2_recved(scaled_imu);
            break;
        }
        case MAVLINK_MSG_ID_SCALED_IMU3:{
            //__messageCopy(__currentMessages.scaledImu, message);
            mavlink_scaled_imu3_t scaled_imu;
            mavlink_msg_scaled_imu3_decode(&message,&scaled_imu);
            emit calibrate_scale_imu3_recved(scaled_imu);
            break;
        }
        case MAVLINK_MSG_ID_RC_CHANNELS_RAW:
        {
            emit handleRCCalibration(message);
            break;
        }
        case MAVLINK_MSG_ID_PARAM_VALUE :{//参数值
			/*if (g_bcritActInfo)
			{*/
                __infoCritAct(message);

			/*}
			else
			{*/
				__paramValue(message);
			/*}*/
            break;
        }
        case MAVLINK_MSG_ID_VFR_HUD:{//VFR HUD
            //__messageCopy(__currentMessages.vfrHud, message);
            mavlink_msg_vfr_hud_decode(&message,&__currentMessages.vfr_hud);
            break;
        }
        case MAVLINK_MSG_ID_STATUSTEXT:{//
            __statusText(message);
            break;
        }
        case MAVLINK_MSG_ID_HOME_POSITION:{//家位置
            __homePosition(message);
            break;
        }
        case MAVLINK_MSG_ID_GPS_RAW_INT:
        {
            //  qDebug("=GPS_RAW======message id %i\n",message.msgid);
            __gpsRawData(message);
            break;
        }
        case MAVLINK_MSG_ID_LOG_ENTRY:{//日志请求list响应
            mavlink_log_entry_t logEntry;
            mavlink_msg_log_entry_decode(&message, &logEntry);
            //qDebug() << logEntry.id << "/" << logEntry.num_logs << "/" << logEntry.last_log_num << "/" << logEntry.time_utc << "/" << logEntry.size;
            emit logEntry_recved(logEntry);
            break;
        }
        case MAVLINK_MSG_ID_LOG_DATA:{
            mavlink_log_data_t logData;
            mavlink_msg_log_data_decode(&message, &logData);
            emit logData_recved(logData);
            break;
        }
        case MAVLINK_MSG_ID_ZY_SENSORS:
        {
            mavlink_msg_zy_sensors_decode(&message,&(__currentMessages.zy_sensor_status));
        }
        break;
        case MAVLINK_MSG_ID_SERVO_OUTPUT_RAW:
        {
            mavlink_servo_output_raw_t tmp;
            mavlink_msg_servo_output_raw_decode(&message,&(tmp));
            if(tmp.port==0)
                __currentMessages.servo_output_raw=tmp;
            else if(tmp.port==1)
                __currentMessages.servo_output_raw1=tmp;
        }
        break;
        case MAVLINK_MSG_ID_RC_CHANNELS_SCALED:{
            mavlink_msg_rc_channels_scaled_decode(&message,&(__currentMessages.rc_channels_scaled));
        }
        break;
        case MAVLINK_MSG_ID_LOCAL_POSITION_NED_SYSTEM_GLOBAL_OFFSET:
        {
            mavlink_msg_local_position_ned_system_global_offset_decode(&message,&(__currentMessages.position_RTK));
        }
        break;
        default:{
            //            printf("Warning, did not handle message id %i\n",message.msgid);\
            //qDebug("==============Warning, did not handle message id %i\n",message.msgid);
            break;
        }
    }
}


//
//参数值
//
void MavLinkMessageInterface::__paramValue(mavlink_message_t msg){
    mavlink_param_value_t value;
    mavlink_msg_param_value_decode(&msg,&value);
    if(value.param_type == MAV_PARAM_TYPE_INT32){
        memcpy(&value.param_value, (void*)msg.payload64,sizeof(value.param_value));
    }
    emit onParamValue(value);
}

void MavLinkMessageInterface::__infoCritAct(mavlink_message_t msg){
	mavlink_param_value_t value;
	mavlink_msg_param_value_decode(&msg, &value);
	if (value.param_type == MAV_PARAM_TYPE_INT32){
		memcpy(&value.param_value, (void*)msg.payload64, sizeof(value.param_value)); 
		char srcArray[4];
		memcpy(srcArray, &value.param_value, sizeof(int));
		value.param_value = *((int*)srcArray);
	}
	emit updateCritActInfo(value.param_id, value.param_value);
}

//
//状态文本
//
void MavLinkMessageInterface::__statusText(mavlink_message_t msg){
    mavlink_statustext_t text;
    mavlink_msg_statustext_decode(&msg, &text);
    emit onStatusText(text);
}

//位置
void MavLinkMessageInterface::getRTKPosition(float& X,float& Y,float& Z)
{
  X=__currentMessages.position_RTK.x;
  Y=__currentMessages.position_RTK.y;
  Z=__currentMessages.position_RTK.z;
}
//
//家位置
//
void MavLinkMessageInterface::__homePosition(mavlink_message_t msg){
    mavlink_home_position_t position;
    mavlink_msg_home_position_decode(&msg, &position);
    emit onHomePosition(position);
}
void MavLinkMessageInterface::__gpsRawData(mavlink_message_t msg)
{
    mavlink_gps_raw_int_t gps_raw_data;
    mavlink_msg_gps_raw_int_decode(&msg,&gps_raw_data);
    __currentMessages.gps_raw_int_data.alt=gps_raw_data.alt;
    __currentMessages.gps_raw_int_data.lon=gps_raw_data.lon;
    __currentMessages.gps_raw_int_data.lat=gps_raw_data.lat;
    __currentMessages.gps_raw_int_data.satellites_visible=gps_raw_data.satellites_visible;
    __currentMessages.gps_raw_int_data.time_usec=gps_raw_data.time_usec;
    __currentMessages.gps_raw_int_data.cog=gps_raw_data.cog;
    __currentMessages.gps_raw_int_data.eph=gps_raw_data.eph;
    __currentMessages.gps_raw_int_data.epv=gps_raw_data.epv;
    __currentMessages.gps_raw_int_data.vel=gps_raw_data.vel;
     __currentMessages.gps_raw_int_data.fix_type=gps_raw_data.fix_type;
}
//
//更新飞机属性
//
void MavLinkMessageInterface::__updateMavlinkProperty(mavlink_message_t heartbeat){
    __targetSystem = heartbeat.sysid;
    //qDebug()<<"************************sysid"<<__targetSystem<<"*****************";
    __targetComponent = heartbeat.compid;

    __baseMode = mavlink_msg_heartbeat_get_base_mode(&heartbeat);
    __customMode = mavlink_msg_heartbeat_get_custom_mode(&heartbeat);

    //qDebug()<<"baseMode:"<<__baseMode<<" customMode:"<<__customMode;

    __systemStatus = mavlink_msg_heartbeat_get_system_status(&heartbeat);
    __autopilotType = mavlink_msg_heartbeat_get_autopilot(&heartbeat);

    __mavlinkVersion = mavlink_msg_heartbeat_get_mavlink_version(&heartbeat);
    __type = mavlink_msg_heartbeat_get_type(&heartbeat);

    receivedMode=true;//link lost set to false;
}

//
//得到命令句柄
//
MavLinkCommandExecutant* MavLinkMessageInterface::getCommandHandle(){
    return &__commandExecutant;
}

//
//得到任务句柄
//
MavLinkMissionExecutant* MavLinkMessageInterface::getMissionHandle(){
    return &__missionExecutant;
}

//
//所有数据
//
void MavLinkMessageInterface::requestDataStreamAll(uint8_t start, uint16_t rate){
    mavlink_message_t message;
    mavlink_request_data_stream_t stream;
    // Select the message to request from now on
    stream.req_stream_id = MAV_DATA_STREAM_ALL;
    // Select the update rate in Hz the message should be send
    stream.req_message_rate = rate;
    // Start / stop the message
    stream.start_stop = start;
    // The system which should take this command
    stream.target_system = __targetSystem;
    // The component / subsystem which should take this command
    stream.target_component = __targetComponent;
    // Encode and send the message
    mavlink_msg_request_data_stream_encode(__systemId, __componentId, &message, &stream);
    sendMessage(message);
}

//
//请求位置传输
//
void MavLinkMessageInterface::requestDataStreamPosition(uint8_t start, uint16_t rate){
    mavlink_request_data_stream_t stream;
    mavlink_message_t message;
    // Select the message to request from now on
    stream.req_stream_id = MAV_DATA_STREAM_POSITION;
    // Select the update rate in Hz the message should be send
    stream.req_message_rate = rate;
    // Start / stop the message
    stream.start_stop = start;
    // The system which should take this command
    stream.target_system = __targetSystem;
    // The component / subsystem which should take this command
    stream.target_component = 0;__targetComponent;
    // Encode and send the message
    mavlink_msg_request_data_stream_encode(__systemId, __componentId, &message, &stream);
    sendMessage(message);
}

//
//MAV_DATA_STREAM_EXTRA1
//
void MavLinkMessageInterface::requestDataStreamExtra1(uint8_t start, uint16_t rate){
    mavlink_message_t message;
    mavlink_request_data_stream_t stream;
    // Select the message to request from now on
    stream.req_stream_id = MAV_DATA_STREAM_EXTRA1;

    // Select the update rate in Hz the message should be send
    stream.req_message_rate = rate;
    // Start / stop the message
    stream.start_stop = start;
    // The system which should take this command
    stream.target_system = __targetSystem;
    // The component / subsystem which should take this command
    stream.target_component = MAV_COMP_ID_ALL;
    // Encode and send the message
    mavlink_msg_request_data_stream_encode(__systemId, __componentId, &message, &stream);
    sendMessage(message);
}

//
//MAV_DATA_STREAM_EXTRA2
//
void MavLinkMessageInterface::requestDataStreamExtra2(uint8_t start, uint16_t rate){
    mavlink_message_t message;
    mavlink_request_data_stream_t stream;
    // Select the message to request from now on
    stream.req_stream_id = MAV_DATA_STREAM_EXTRA2;

    // Select the update rate in Hz the message should be send
    stream.req_message_rate = rate;
    // Start / stop the message
    stream.start_stop = start;
    // The system which should take this command
    stream.target_system = __targetSystem;
    // The component / subsystem which should take this command
    stream.target_component = __targetComponent;
    // Encode and send the message
    mavlink_msg_request_data_stream_encode(__systemId, __componentId, &message, &stream);
    sendMessage(message);
}

//
//MAV_DATA_STREAM_EXTRA3
//
void MavLinkMessageInterface::requestDataStreamExtra3(uint8_t start, uint16_t rate){
    mavlink_message_t message;
    mavlink_request_data_stream_t stream;
    // Select the message to request from now on
    stream.req_stream_id = MAV_DATA_STREAM_EXTRA3;

    // Select the update rate in Hz the message should be send
    stream.req_message_rate = rate;
    // Start / stop the message
    stream.start_stop = start;
    // The system which should take this command
    stream.target_system = __targetSystem;
    // The component / subsystem which should take this command
    stream.target_component = __targetComponent;
    // Encode and send the message
    mavlink_msg_request_data_stream_encode(__systemId, __componentId, &message, &stream);
    sendMessage(message);
}


//
//原始传感器
//
void MavLinkMessageInterface::requestDataStreamRawSensors(uint8_t start, uint16_t rate){
    mavlink_message_t message;
    mavlink_request_data_stream_t stream;
    // Select the message to request from now on
    stream.req_stream_id = MAV_DATA_STREAM_RAW_SENSORS;

    // Select the update rate in Hz the message should be send
    stream.req_message_rate = rate;
    // Start / stop the message
    stream.start_stop = start;
    // The system which should take this command
    stream.target_system = __targetSystem;
    // The component / subsystem which should take this command
    //revise  from MAV_COMP_ID_IMU to 0 by xjw;
    stream.target_component = 0;
    // Encode and send the message
    mavlink_msg_request_data_stream_encode(__systemId, __componentId, &message, &stream);
    sendMessage(message);
}

//
//请求 1.状态 24.GPS_ROW 42.当前任务 62.导航控制器输出
//
void MavLinkMessageInterface::requestDataStreamExtendedStatus(uint8_t start, uint16_t rate){
    mavlink_message_t message;
    mavlink_request_data_stream_t stream;
    // Select the message to request from now on
    stream.req_stream_id = MAV_DATA_STREAM_EXTENDED_STATUS;

    // Select the update rate in Hz the message should be send
    stream.req_message_rate = rate;
    // Start / stop the message
    stream.start_stop = start;
    // The system which should take this command
    stream.target_system = __targetSystem;
    // The component / subsystem which should take this command
    stream.target_component = 0;
    // Encode and send the message
    mavlink_msg_request_data_stream_encode(__systemId, __componentId, &message, &stream);
    sendMessage(message);
}

//
//MAV_DATA_STREAM_RC_CHANNELS
//
void MavLinkMessageInterface::requestDataStreamRcChannels(uint8_t start, uint16_t rate){
    mavlink_message_t message;
    mavlink_request_data_stream_t stream;
    // Select the message to request from now on
    stream.req_stream_id = MAV_DATA_STREAM_RC_CHANNELS;

    // Select the update rate in Hz the message should be send
    stream.req_message_rate = rate;
    // Start / stop the message
    stream.start_stop = start;
    // The system which should take this command
    stream.target_system = __targetSystem;
    // The component / subsystem which should take this command
    stream.target_component = 0;
    // Encode and send the message
    mavlink_msg_request_data_stream_encode(__systemId, __componentId, &message, &stream);
    sendMessage(message);
}

//
//MAV_DATA_STREAM_RAW_CONTROLLER
//
void MavLinkMessageInterface::requestDataStreamRawController(uint8_t start, uint16_t rate){
    mavlink_message_t message;
    mavlink_request_data_stream_t stream;
    // Select the message to request from now on
    stream.req_stream_id = MAV_DATA_STREAM_RAW_CONTROLLER;

    // Select the update rate in Hz the message should be send
    stream.req_message_rate = rate;
    // Start / stop the message
    stream.start_stop = start;
    // The system which should take this command
    stream.target_system = __targetSystem;
    // The component / subsystem which should take this command
    stream.target_component = 0;
    // Encode and send the message
    mavlink_msg_request_data_stream_encode(__systemId, __componentId, &message, &stream);
    sendMessage(message);
}

//
//得到飞行模式
//
void MavLinkMessageInterface::getFlightMode(uint8_t& baseMode, uint32_t& customMode){
    baseMode = __baseMode;
    customMode = __customMode;
}

//
//设置飞行模式
//
void MavLinkMessageInterface::setFlightMode(const uint8_t& baseMode, const uint32_t& customMode){
    mavlink_message_t message;
    mavlink_set_mode_t msm;
    msm.custom_mode |= customMode;
    msm.target_system = __targetSystem;
    msm.base_mode = (__baseMode & ~MAV_MODE_FLAG_DECODE_POSITION_CUSTOM_MODE)|baseMode;
    mavlink_msg_set_mode_encode(__systemId, __componentId, &message,&msm);
    sendMessage(message);
}

bool MavLinkMessageInterface::getArmedState()
{
    bool currentlyArmed = __baseMode & MAV_MODE_FLAG_DECODE_POSITION_SAFETY;

    return currentlyArmed;
}

void MavLinkMessageInterface::__messageCopy( mavlink_message_t& targetMsg, mavlink_message_t& sourceMsg ){
    targetMsg.checksum = sourceMsg.checksum;
    targetMsg.magic = sourceMsg.magic;
    targetMsg.len = sourceMsg.len;
    targetMsg.seq = sourceMsg.seq;
    targetMsg.sysid = sourceMsg.sysid;
    targetMsg.compid = sourceMsg.compid;
    targetMsg.msgid = sourceMsg.msgid;

    memcpy(targetMsg.payload64, sourceMsg.payload64, (MAVLINK_MAX_PAYLOAD_LEN+MAVLINK_NUM_CHECKSUM_BYTES+7)/8);

}

//
//得到/设置链路
//
void MavLinkMessageInterface::setLink(CommunicationInterface *mci, CommunicationInterface *sci)
{
    __Mainci=mci;
    __Slaveci=sci;
    link();
}

void MavLinkMessageInterface::link(bool bUseMain)
{
    if(bUseMain)
    {
        if(__Slaveci)
            disconnect(__Slaveci, SIGNAL(onMavLinkMessage(mavlink_message_t)), this, SLOT(switchMavlinkMsg(mavlink_message_t)));
        connect(__Mainci, SIGNAL(onMavLinkMessage(mavlink_message_t)), this, SLOT(switchMavlinkMsg(mavlink_message_t)));
    }
    else if(__Slaveci)
    {
        disconnect(__Mainci, SIGNAL(onMavLinkMessage(mavlink_message_t)), this, SLOT(switchMavlinkMsg(mavlink_message_t)));
        connect(__Slaveci, SIGNAL(onMavLinkMessage(mavlink_message_t)), this, SLOT(switchMavlinkMsg(mavlink_message_t)));
    }
}


//
//命令-航路点
//
uint16_t MavLinkMessageInterface::commandNavWaypoint(uint8_t autocontinue, float holdTime, float radius, float through, float yaw, float lat, float lon, float alt){
    mavlink_command_long_t mclt;
    mclt.param1 = holdTime;
    mclt.param2 = radius;
    mclt.param3 = through;
    mclt.param4 = yaw;
    mclt.param5 = lat;
    mclt.param6 = lon;
    mclt.param7 = alt;
    mclt.command = MAV_CMD_NAV_WAYPOINT;
    mclt.target_system = __targetSystem;
    mclt.target_component = __targetComponent;
    mclt.confirmation = autocontinue;

    mavlink_message_t msg;
    mavlink_msg_command_long_encode(__systemId, __componentId, &msg, &mclt);
    __commandExecutant.longCommand(mclt);
    sendMessage(msg);
    return MAV_CMD_NAV_WAYPOINT;
}

//
//持续盘旋
//
uint16_t MavLinkMessageInterface::commandLoiterUnlim(uint8_t confirmation,float radius, float yaw, float longitude, float latitude , float altitude){
    mavlink_command_long_t mclt;
    mclt.param1 = 0;
    mclt.param2 = 0;
    mclt.param3 = radius;
    mclt.param4 = yaw;
    mclt.param5 = latitude;
    mclt.param6 = longitude;
    mclt.param7 = altitude;
    mclt.command = MAV_CMD_NAV_LOITER_UNLIM;
    mclt.target_system = __targetSystem;
    mclt.target_component = __targetComponent;
    mclt.confirmation = confirmation;

    mavlink_message_t msg;
    mavlink_msg_command_long_encode(__systemId, __componentId, &msg, &mclt);
    __commandExecutant.longCommand(mclt);
    sendMessage(msg);
    return MAV_CMD_NAV_LOITER_UNLIM;
}

//
//在航点盘旋 N 圈
//
uint16_t MavLinkMessageInterface::commandLoiterTurns(uint8_t confirmation,float truns, float radius, float yaw, float longitude, float latitude , float altitude){
    mavlink_command_long_t mclt;
    mclt.param1 = truns;
    mclt.param2 = 0;
    mclt.param3 = radius;
    mclt.param4 = yaw;
    mclt.param5 = latitude;
    mclt.param6 = longitude;
    mclt.param7 = altitude;
    mclt.command = MAV_CMD_NAV_LOITER_TURNS;
    mclt.target_system = __targetSystem;
    mclt.target_component = __targetComponent;

    mclt.confirmation = confirmation;

    mavlink_message_t msg;
    mavlink_msg_command_long_encode(__systemId, __componentId, &msg, &mclt);
    __commandExecutant.longCommand(mclt);
    sendMessage(msg);
    return MAV_CMD_NAV_LOITER_TURNS;
}

//
//命令-返回起飞点
//
uint16_t MavLinkMessageInterface::commandReturnToLaunch(uint8_t confirmation){
    mavlink_command_long_t mclt;
    mclt.param1 = 0;
    mclt.param2 = 0;
    mclt.param3 = 0;
    mclt.param4 = 0;
    mclt.param5 = 0;
    mclt.param6 = 0;
    mclt.param7 = 0;

    mclt.command = MAV_CMD_NAV_RETURN_TO_LAUNCH;
    mclt.target_system = __targetSystem;
    mclt.target_component = __targetComponent;

    mclt.confirmation = confirmation;

    mavlink_message_t msg;
    mavlink_msg_command_long_encode(__systemId, __componentId, &msg, &mclt);
    __commandExecutant.longCommand(mclt);
    sendMessage(msg);
    return MAV_CMD_NAV_RETURN_TO_LAUNCH;
}

//
//命令-前往设定高度
//
uint16_t MavLinkMessageInterface::commandConditionChangeALT(uint8_t confirmation,float descent, float finishAlt ){
    mavlink_command_long_t mclt;
    mclt.param1 = descent;
    mclt.param2 = 0;
    mclt.param3 = 0;
    mclt.param4 = 0;
    mclt.param5 = 0;
    mclt.param6 = 0;
    mclt.param7 = finishAlt;

    mclt.command = MAV_CMD_CONDITION_CHANGE_ALT;
    mclt.target_system = __targetSystem;
    mclt.target_component = __targetComponent;

    mclt.confirmation= confirmation;

    mavlink_message_t msg;
    mavlink_msg_command_long_encode(__systemId, __componentId, &msg, &mclt);
    __commandExecutant.longCommand(mclt);
    sendMessage(msg);
    return MAV_CMD_CONDITION_CHANGE_ALT;
}

//
//命令-设定家位置
//
uint16_t MavLinkMessageInterface::commandDoSetHome(uint8_t confirmation,float location, float longitude, float latitude , float altitude){
    mavlink_command_long_t mclt;
    mclt.param1 = location;
    mclt.param2 = 0;
    mclt.param3 = 0;
    mclt.param4 = 0;
    mclt.param5 = latitude;
    mclt.param6 = longitude;
    mclt.param7 = altitude;

    mclt.command = MAV_CMD_DO_SET_HOME;
    mclt.target_system = __targetSystem;
    mclt.target_component = __targetComponent;

    mclt.confirmation = confirmation;

    mavlink_message_t msg;
    mavlink_msg_command_long_encode(__systemId, __componentId, &msg, &mclt);
    __commandExecutant.longCommand(mclt);
    sendMessage(msg);
    return MAV_CMD_DO_SET_HOME;
}

//命令-获取家位置
uint16_t MavLinkMessageInterface::commandGetHomePosition(uint8_t confirmation){
    mavlink_command_long_t mclt;
    mclt.param1 = 0;
    mclt.param2 = 0;
    mclt.param3 = 0;
    mclt.param4 = 0;
    mclt.param5 = 0;
    mclt.param6 = 0;
    mclt.param7 = 0;

    mclt.command = MAV_CMD_GET_HOME_POSITION;
    mclt.target_system = __targetSystem;
    mclt.target_component = __targetComponent;

    mclt.confirmation = confirmation;

    mavlink_message_t msg;
    mavlink_msg_command_long_encode(__systemId, __componentId, &msg, &mclt);
    __commandExecutant.longCommand(mclt);
    sendMessage(msg);
    return MAV_CMD_GET_HOME_POSITION;
}

//命令-设置参数
uint16_t MavLinkMessageInterface::commandDoSetParameter(uint8_t confirmation,float paramNum, float paramValue){
    mavlink_command_long_t mclt;
    mclt.param1 = paramNum;
    mclt.param2 = paramValue;
    mclt.param3 = 0;
    mclt.param4 = 0;
    mclt.param5 = 0;
    mclt.param6 = 0;
    mclt.param7 = 0;

    mclt.command = MAV_CMD_DO_SET_PARAMETER;
    mclt.target_system = __targetSystem;
    mclt.target_component = __targetComponent;
    mclt.confirmation = 0;

    mavlink_message_t msg;
    mavlink_msg_command_long_encode(__systemId, __componentId, &msg, &mclt);
    __commandExecutant.longCommand(mclt);
    sendMessage(msg);
    return MAV_CMD_DO_SET_PARAMETER;
}

//
//命令-起飞（手抛/地面起飞）
//
uint16_t MavLinkMessageInterface::commandTakeOff(uint8_t confirmation,float yaw, float altitude){
    if(!isCommConnected)
    {
        return -1;
    }

    mavlink_command_long_t mclt;
    mclt.param1 = 0;
    mclt.param2 = 0;
    mclt.param3 = 0;
    mclt.param4 = yaw;
    mclt.param5 = NAN;
    mclt.param6 = NAN;
    mclt.param7 = altitude;

    mclt.command = MAV_CMD_NAV_TAKEOFF;
    mclt.target_system = __targetSystem;
    mclt.target_component = __targetComponent;

    mclt.confirmation = confirmation;

    mavlink_message_t msg;
    mavlink_msg_command_long_encode(__systemId, __componentId, &msg, &mclt);
    __commandExecutant.longCommand(mclt);
    sendMessage(msg);
    return MAV_CMD_NAV_TAKEOFF;
}

//
//命令-悬停
//
uint16_t MavLinkMessageInterface::commandLoiter(uint8_t confirmation,float yaw, float altitude){
    mavlink_command_long_t mclt;
    mclt.param1 = 0;
    mclt.param2 = 0;
    mclt.param3 = 0;
    mclt.param4 = yaw;
    mclt.param5 = NAN;
    mclt.param6 = NAN;
    mclt.param7 = altitude;

    mclt.command = MAV_CMD_NAV_LOITER_UNLIM;
    mclt.target_system = __targetSystem;
    mclt.target_component = __targetComponent;

    mclt.confirmation = confirmation;

    mavlink_message_t msg;
    mavlink_msg_command_long_encode(__systemId, __componentId, &msg, &mclt);
    __commandExecutant.longCommand(mclt);
    sendMessage(msg);
    return MAV_CMD_NAV_TAKEOFF;
}

//
//命令-设定点着陆
//
uint16_t MavLinkMessageInterface::commandNavLand(uint8_t confirmation,float yaw, float longitude, float latitude , float altitude){
    mavlink_command_long_t mclt;
    mclt.param1 = 0;
    mclt.param2 = 0;
    mclt.param3 = 0;
    mclt.param4 = yaw;
    mclt.param5 = latitude;
    mclt.param6 = longitude;
    mclt.param7 = altitude;

    mclt.command = MAV_CMD_NAV_LAND;
    mclt.target_system = __targetSystem;
    mclt.target_component = __targetComponent;

    mclt.confirmation = confirmation;

    mavlink_message_t msg;
    mavlink_msg_command_long_encode(__systemId, __componentId, &msg, &mclt);
    __commandExecutant.longCommand(mclt);
    sendMessage(msg);
    return MAV_CMD_NAV_LAND;
}

//
//命令-设定本地位置着陆点
//
uint16_t MavLinkMessageInterface::commandNavLandLocal(uint8_t confirmation,float targetNumber, float maxOffset, float descend, float yaw, float x, float y, float z ){
    mavlink_command_long_t mclt;
    mclt.param1 = targetNumber;
    mclt.param2 = maxOffset;
    mclt.param3 = descend;
    mclt.param4 = yaw;
    mclt.param5 = x;
    mclt.param6 = y;
    mclt.param7 = z;

    mclt.command = MAV_CMD_NAV_LAND_LOCAL;
    mclt.target_system = __targetSystem;
    mclt.target_component = __targetComponent;

    mclt.confirmation = confirmation;

    mavlink_message_t msg;
    mavlink_msg_command_long_encode(__systemId, __componentId, &msg, &mclt);
    __commandExecutant.longCommand(mclt);
    sendMessage(msg);
    return MAV_CMD_NAV_LAND_LOCAL;
}

//
//命令-引导限制
//
uint16_t MavLinkMessageInterface::commandGuidedLimits(uint8_t confirmation,float timeout, float minAlt, float maxAlt, float horizontalLimit){
    mavlink_command_long_t mclt;
    mclt.param1 = timeout;
    mclt.param2 = minAlt;
    mclt.param3 = maxAlt;
    mclt.param4 = horizontalLimit;
    mclt.param5 = 0;
    mclt.param6 = 0;
    mclt.param7 = 0;

    mclt.command = 	MAV_CMD_DO_GUIDED_LIMITS;
    mclt.target_system = __targetSystem;
    mclt.target_component = __targetComponent;

    mclt.confirmation = confirmation;

    mavlink_message_t msg;
    mavlink_msg_command_long_encode(__systemId, __componentId, &msg, &mclt);
    __commandExecutant.longCommand(mclt);
    sendMessage(msg);
    return MAV_CMD_DO_GUIDED_LIMITS;
}

void MavLinkMessageInterface::startCompassMotCalibration()
{
    mavlink_message_t msg;
    // Param 1: gyro cal, param 2: mag cal, param 3: pressure cal, Param 4: radio, Param5: Accel Calib Param 6: Compass Mot
    mavlink_msg_command_long_pack(__systemId, __componentId, &msg, 1, 0, MAV_CMD_PREFLIGHT_CALIBRATION, 1, 0, 0, 0, 0, 0, 1, 0);
    sendMessage(msg);
}


//命令-航前设备校准
uint16_t MavLinkMessageInterface::commandTriggerCalibration(uint8_t confirmation,float gyro, float magnetometer, float groundPressure, float radio, float accelerometer ){
    mavlink_command_long_t mclt;
    mclt.param1 = gyro;
    mclt.param2 = magnetometer;
    mclt.param3 = groundPressure;
    mclt.param4 = radio;
    mclt.param5 = accelerometer;
    mclt.param6 = 0;
    mclt.param7 = 0;

    mclt.command = 	MAV_CMD_PREFLIGHT_CALIBRATION;
    mclt.target_system = __targetSystem;
    mclt.target_component = __targetComponent;

    mclt.confirmation = confirmation;

    mavlink_message_t msg;
    mavlink_msg_command_long_encode(__systemId, __componentId, &msg, &mclt);
    __commandExecutant.longCommand(mclt);
    sendMessage(msg);
    return MAV_CMD_PREFLIGHT_CALIBRATION;
}

//
//命令-改变速度
//
uint16_t MavLinkMessageInterface::commandChangeSpeed(uint8_t confirmation,float type, float speed, float throttle){
    mavlink_command_long_t mclt;
    mclt.param1 = type;
    mclt.param2 = speed;
    mclt.param3 = throttle;
    mclt.param4 = 0;
    mclt.param5 = 0;
    mclt.param6 = 0;
    mclt.param7 = 0;

    mclt.command = 	MAV_CMD_DO_CHANGE_SPEED;
    mclt.target_system = __targetSystem;
    mclt.target_component = __targetComponent;

    mclt.confirmation = confirmation;

    mavlink_message_t msg;
    mavlink_msg_command_long_encode(__systemId, __componentId, &msg, &mclt);
    __commandExecutant.longCommand(mclt);
    sendMessage(msg);
    return MAV_CMD_DO_CHANGE_SPEED;
}

//
//命令-arm/disarm 上锁/解锁
//
uint16_t MavLinkMessageInterface::commandComponentArmDisarm(uint8_t confirmation, float arm){
    mavlink_command_long_t mclt;
    mclt.param1 = arm;
    mclt.param2 = 21196.0;//这个是国岐协议里的数字，猜测这个位置飞控不用
    mclt.param3 = 0;
    mclt.param4 = 0;
    mclt.param5 = 0;
    mclt.param6 = 0;
    mclt.param7 = 0;

    mclt.command = MAV_CMD_COMPONENT_ARM_DISARM;
    mclt.target_system = __targetSystem;
    mclt.target_component = 1;

    mclt.confirmation = confirmation;

    mavlink_message_t msg;
    mavlink_msg_command_long_encode(__systemId, __componentId, &msg, &mclt);
    __commandExecutant.longCommand(mclt);
    sendMessage(msg);
    return MAV_CMD_COMPONENT_ARM_DISARM;
}

//
//命令-设置模式
//
uint16_t MavLinkMessageInterface::commandDoSetMode(uint8_t confirmation, float mode, float customMode, float customSubMode){
    mavlink_command_long_t mclt;
    mclt.param1 = mode;
    mclt.param2 = customMode;
    mclt.param3 = customSubMode;
    mclt.param4 = 0;
    mclt.param5 = 0;
    mclt.param6 = 0;
    mclt.param7 = 0;

    mclt.command = MAV_CMD_DO_SET_MODE;
    mclt.target_system = __targetSystem;
    mclt.target_component = 1;

    mclt.confirmation = confirmation;

    mavlink_message_t msg;
    mavlink_msg_command_long_encode(__systemId, __componentId, &msg, &mclt);
    __commandExecutant.longCommand(mclt);
    sendMessage(msg);
    return MAV_CMD_DO_SET_MODE;
}

//
//设置引导模式
//
void MavLinkMessageInterface::setGuidedMode(){
    //setFlightMode(__baseMode,4);
    commandDoSetMode(0,__baseMode,4,0);
}

//
//设置模式land
//
void MavLinkMessageInterface::setLandMode(){
    //setFlightMode(__baseMode,9);
    commandDoSetMode(0,__baseMode,9,0);
}

//
//设置航向角
//
void MavLinkMessageInterface::setGrdYaw(float yaw){
    paramSet(yaw, (char*)"A_GRD_YAW", MAV_PARAM_TYPE_REAL32);
}

//
//命令-ACK
//
void MavLinkMessageInterface::commandAck(mavlink_message_t message){
    mavlink_command_ack_t commandAck;
    mavlink_msg_command_ack_decode(&message, &commandAck);
    __commandExecutant.commandAck(commandAck);
}

void MavLinkMessageInterface::doCommand(MAV_CMD cmd,float p1, float p2, float p3, float p4, float p5, float p6, float p7)
{
    mavlink_command_long_t command;
    command.param1 = p1;
    command.param2 = p2;
    command.param3 = p3;//0xaa;
    command.param4 = p4;//0;
    command.param5 = p5;//0;
    command.param6 = p6;//0;
    command.param7 = p7;//0;
    command.command = cmd;
    command.target_system = __targetSystem;
    command.target_component = 0;
    command.confirmation = 0;

    mavlink_message_t msg;
    mavlink_msg_command_long_encode(__systemId, __componentId, &msg, &command);
    __commandExecutant.longCommand(command);
    sendMessage(msg);
}



//
//参数请求读
//
void MavLinkMessageInterface::paramRequestRead(char* paramId, int16_t index){
    mavlink_param_request_read_t mprr;
    mprr.param_index = index;
    mprr.target_system = __targetSystem;
    mprr.target_component = __targetComponent;
    if( NULL != paramId){
        memcpy(mprr.param_id,paramId,16*sizeof(char));
    }
    mavlink_message_t msg;
    mavlink_msg_param_request_read_encode(__systemId, __componentId, &msg, &mprr);
    sendMessage(msg);
}

//
//参数请求列表
//
void MavLinkMessageInterface::paramRequestList(){
    mavlink_param_request_list_t mprl;
    mprl.target_system = __targetSystem;
    mprl.target_component = __targetComponent;

    mavlink_message_t msg;
    mavlink_msg_param_request_list_encode(__systemId, __componentId, &msg, &mprl);
    sendMessage(msg);
}

//
//参数设置
//
void MavLinkMessageInterface::paramSet(float paramValue, char* paramId, uint8_t paramType){
    mavlink_param_set_t mps;
    mps.param_value = paramValue;
    mps.target_system = __targetSystem;
    mps.target_component = __targetComponent;
    memcpy(mps.param_id, paramId, 16*sizeof(char));
    mps.param_type = paramType;
    mavlink_message_t msg;
    memset(msg.payload64, 0, sizeof(msg.payload64));
    mavlink_msg_param_set_encode(__systemId, __componentId, &msg, &mps);
    sendMessage(msg);
    qDebug() << "paramId:" << paramId << " val:" << paramValue <<" msgid:" << msg.msgid;
    //Logger::Instance()->LogStruct(msg);
}

//
//读航路点列表
//
void MavLinkMessageInterface::readMavWaypointList(){
    mavlink_mission_request_list_t mmrl;
    mmrl.target_system = __targetSystem;

    mmrl.target_component = __targetComponent;

    mavlink_message_t msg;
    mavlink_msg_mission_request_list_encode(__systemId, __componentId, &msg, &mmrl);
    qDebug()<<"************************tarsysid"<<__targetSystem<<"*****************";
    __missionExecutant.readMavWaypointList();
    sendMessage(msg);
}

//
//写航路点列表
//
void MavLinkMessageInterface::writeMavWaypointList(std::vector<mavlink_mission_item_int_t*> items){
    int count = __missionExecutant.writeMavWaypointList(items);
    __WaypointCount(count);
}

//
//清除航路点列表
//
void MavLinkMessageInterface::clearMavWaypointlist(){
    mavlink_mission_clear_all_t clear;
    clear.target_system = __targetSystem;
    clear.target_component = __targetComponent;

    mavlink_message_t msg;
    mavlink_msg_mission_clear_all_encode(__systemId, __componentId, &msg, &clear);
    sendMessage(msg);
}

//
//设置新的当前航路点
//
void MavLinkMessageInterface::setNewCurrentMavWaypoit( uint16_t seq){
    mavlink_mission_set_current_t current;
    current.target_system = __targetSystem;
    current.target_component = __targetComponent;
    current.seq = seq;
    mavlink_message_t msg;
    mavlink_msg_mission_set_current_encode(__systemId, __componentId, &msg, &current);
    sendMessage(msg);
}




//--------------------SETMODE-----------------------------

void MavLinkMessageInterface::setMode(uint8_t newBaseMode, uint32_t newCustomMode)
{
    if (receivedMode)
    {
        //base_mode = mode; //no call assignament, update receive message from UAS

        // Strip armed / disarmed call for safety reasons, this is not relevant for setting the mode
        newBaseMode &= ~MAV_MODE_FLAG_SAFETY_ARMED;
        // Now set current state (request no change)
        newBaseMode |= this->__baseMode & MAV_MODE_FLAG_SAFETY_ARMED;

        setModeArm(newBaseMode, newCustomMode);
    }
    else
    {
        //qDebug() << "WARNING: setMode called before base_mode bitmask was received from UAS, new mode was not sent to system";
    }
}


void MavLinkMessageInterface::setModeArm(uint8_t newBaseMode, uint32_t newCustomMode)
{
    if (receivedMode)
    {
        mavlink_message_t msg;
        mavlink_msg_set_mode_pack(__systemId, __componentId, &msg, (uint8_t)1, newBaseMode, newCustomMode);
        //qDebug() << "mavlink_msg_set_mode_pack 1";
        sendMessage(msg);

    }
    else
    {
        //qDebug() << "WARNING: setModeArm called before base_mode bitmask was received from UAS, new mode was not sent to system";
    }
}

//--------------------SETMODE----------------------------

void MavLinkMessageInterface::setModeArm(int flag){
    mavlink_command_long_t command;
    command.param1 = flag;
    command.param2 = 0;
    command.param3 = 0;
    command.param4 = 0;
    command.param5 = 0;
    command.param6 = 0;
    command.param7 = 0;
    command.command = MAV_CMD_COMPONENT_ARM_DISARM;
    command.target_system = __targetSystem;
    command.target_component = 0;
    command.confirmation = 0;

    mavlink_message_t msg;
    mavlink_msg_command_long_encode(__systemId, __componentId, &msg, &command);
    sendMessage(msg);
}


//chan1_raw（1000：最大速度向前飞，1500: 不动，2000：最大速度向后飞）
//chan2_raw （1000：最大速度向左飞，1500: 不动，2000：最大速度向右飞）
//chan3_raw（1000：最大速度下降，1500: 不动，2000：最大速度上升）
//chan4_raw（1000：最大速度逆时针旋转，1500: 不动，2000：最大速度顺时针旋转）

int MavLinkMessageInterface::sendRC_channels_override(int ch1, int ch2, int ch3, int ch4,int ch5,int ch8){
    mavlink_rc_channels_override_t rc_channels_override_t;
    rc_channels_override_t.chan1_raw = ch1;
    rc_channels_override_t.chan2_raw = ch2;
    rc_channels_override_t.chan3_raw = ch3;
    rc_channels_override_t.chan4_raw = ch4;
    rc_channels_override_t.chan5_raw = ch5;
    rc_channels_override_t.chan6_raw = 0;
    rc_channels_override_t.chan7_raw = 0;
    rc_channels_override_t.chan8_raw = ch8;
    rc_channels_override_t.target_system = __targetSystem;
    rc_channels_override_t.target_component = 0;

    mavlink_message_t msg;
    mavlink_msg_rc_channels_override_encode(__systemId, __componentId, &msg, &rc_channels_override_t);
    sendMessage(msg);
    return true;
}

//启用摇杆 ：1 ， 禁用摇杆：0
void MavLinkMessageInterface::setRCType(int flag){
    qDebug() << "&&&&&&&&&&&&&&&&&& : " << flag;
    mavlink_command_long_t command;
    command.param1 = flag;
    command.param2 = 0;
    command.param3 = 0;
    command.param4 = 0;
    command.param5 = 0;
    command.param6 = 0;
    command.param7 = 0;
    command.command = MAV_CMD_USER_1;
    command.target_system = __targetSystem;
    command.target_component = 0;
    command.confirmation = 0;

    mavlink_message_t msg;
    mavlink_msg_command_long_encode(__systemId, __componentId, &msg, &command);
    sendMessage(msg);
}

//航路点数量
void MavLinkMessageInterface::__waypointCount(mavlink_message_t& msg){
    mavlink_mission_count_t mmc;
    mavlink_msg_mission_count_decode(&msg, &mmc);
    __missionExecutant.missionCount(mmc);

    //请求航路点
    __waypointRequest();
}
void MavLinkMessageInterface::__WaypointCount(uint16_t count){
    mavlink_mission_count_t mmc;
    mmc.target_system = __targetSystem;
    mmc.target_component = __targetComponent;
    mmc.count = count;
    mavlink_message_t msg;
    mavlink_msg_mission_count_encode(__systemId, __componentId, &msg, &mmc);
    sendMessage(msg);
}

//请求航路点
void MavLinkMessageInterface::__waypointRequest(){
    mavlink_mission_request_t mmr;
    mmr.target_system = __targetSystem;
    mmr.target_component = __targetComponent;
    int seq = __missionExecutant.getCurrentRequestSeq();
    if(seq<0){//航路点读取完成
        __waypointAck();
        return;
    }
    mmr.seq = seq;

    //qDebug("==============req waypoint seq %d\n",mmr.seq);
    mavlink_message_t msg;
    mavlink_msg_mission_request_encode(__systemId, __componentId, &msg, &mmr);
    __missionExecutant.missionRequest();
    sendMessage(msg);
}
void MavLinkMessageInterface::__waypointRequest(mavlink_message_t& msg){
    mavlink_mission_request_int_t request;
    mavlink_msg_mission_request_int_decode(&msg, &request);
    __missionExecutant.missionRequest(request);
    //qDebug("==============req waypoint seq %d\n",request.seq);
    __waypoint(request.seq);
}

//
//航路点
//
void MavLinkMessageInterface::__waypoint(mavlink_message_t& msg){
    mavlink_mission_item_int_t mmii;
    mavlink_msg_mission_item_int_decode(&msg, &mmii);
    __missionExecutant.missionItem(mmii);

    __waypointRequest();
}

void MavLinkMessageInterface::__waypoint(uint16_t seq){
    mavlink_mission_item_int_t item;
    item.target_system = __targetSystem;
    item.target_component = __targetComponent;
    __missionExecutant.missionItem(seq, item);

    mavlink_message_t msg;
    mavlink_msg_mission_item_int_encode(__systemId, __componentId, &msg, &item);
    qDebug("==============send waypoint seq: %d (%f,%f,%f)\n",item.seq,item.x,item.y,item.z);
    sendMessage(msg);
}

//
//航路点ack
//
void MavLinkMessageInterface::__waypointAck(){
    mavlink_mission_ack_t ack;
    ack.target_component = __targetComponent;
    ack.target_system = __targetSystem;
    ack.type = MAV_MISSION_ACCEPTED;
    mavlink_message_t msg;
    mavlink_msg_mission_ack_encode(__systemId, __componentId, &msg, &ack);
    sendMessage(msg);
    __missionExecutant.missionAck();
}
void MavLinkMessageInterface::__waypointAck(mavlink_message_t& msg){
    mavlink_mission_ack_t ack;
    mavlink_msg_mission_ack_decode(&msg, &ack);
    __missionExecutant.missionAck(ack);
}

//
//当前航路点
//
void MavLinkMessageInterface::__waypointCurrent(mavlink_message_t& msg){
    mavlink_mission_current_t current;
    mavlink_msg_mission_current_decode(&msg, &current);
}

//
//位置
//
void MavLinkMessageInterface::getPosition( float& lat,float& lon,float& alt){
    lat = __currentMessages.global_position_int.lat;
    lon = __currentMessages.global_position_int.lon;
    alt = __currentMessages.global_position_int.alt;
}

//姿态
void MavLinkMessageInterface::getAttitude( float& roll, float& yaw, float& pitch){
    roll = __currentMessages.attitude.roll;
    pitch = __currentMessages.attitude.pitch;
    yaw = __currentMessages.attitude.yaw;
}

void MavLinkMessageInterface::getAttitude(float &roll, float &yaw, float &pitch, float &rollrate, float &yawrate, float &pitchrate)
{
    roll = __currentMessages.attitude.roll;
    pitch = __currentMessages.attitude.pitch;
    yaw = __currentMessages.attitude.yaw;
    rollrate = __currentMessages.attitude.rollspeed;
    pitchrate = __currentMessages.attitude.pitchspeed;
    yawrate = __currentMessages.attitude.yawspeed;
}
void MavLinkMessageInterface::getGPSRawData(uint8_t& satelliteNum,uint8_t& fix_type,int32_t& lat,int32_t& lon,int32_t&alt){
   satelliteNum = __currentMessages.gps_raw_int_data.satellites_visible;
   fix_type=__currentMessages.gps_raw_int_data.fix_type;
   lat = __currentMessages.local_position_ned.x;
   lon = __currentMessages.local_position_ned.y;
   alt = __currentMessages.local_position_ned.z;
  // qDebug() << " local_position_ned: " << __currentMessages.local_position_ned.x << " : " << __currentMessages.local_position_ned.y;
}
//
//获取信号强度
//
__int64 MavLinkMessageInterface::getSignalIntensity(){
    return time(NULL) - __signalIntensity;
}

//
//获得VFR_HUD
//
void MavLinkMessageInterface::getVfrHud(float& airSpeed, float& groundSpeed, int16_t& heading, uint16_t& throttle, float& alt, float& climb){
//    airSpeed = mavlink_msg_vfr_hud_get_airspeed(&__currentMessages.vfrHud);
//    groundSpeed = mavlink_msg_vfr_hud_get_groundspeed(&__currentMessages.vfrHud);
//    heading = mavlink_msg_vfr_hud_get_heading(&__currentMessages.vfrHud);
//    throttle = mavlink_msg_vfr_hud_get_throttle(&__currentMessages.vfrHud);
//    alt = mavlink_msg_vfr_hud_get_alt(&__currentMessages.vfrHud);
//    climb = mavlink_msg_vfr_hud_get_climb(&__currentMessages.vfrHud);
    airSpeed = __currentMessages.vfr_hud.airspeed;
    groundSpeed = __currentMessages.vfr_hud.groundspeed;
    heading = __currentMessages.vfr_hud.heading;
    throttle = __currentMessages.vfr_hud.throttle;
    alt = __currentMessages.vfr_hud.alt;
    climb = __currentMessages.vfr_hud.climb;
}
void MavLinkMessageInterface::getExtentdStatus(float& MainVoltage,float& BakVoltage,float& Temperature,float ESC_Current[12])
{
    MainVoltage=__currentMessages.zy_sensor_status.main_voltage;
    BakVoltage=__currentMessages.zy_sensor_status.bak_voltage;
    Temperature=__currentMessages.zy_sensor_status.motor_temperature1;

    ESC_Current[0]=__currentMessages.servo_output_raw.servo1_raw;
    ESC_Current[1]=__currentMessages.servo_output_raw.servo2_raw;
    ESC_Current[2]=__currentMessages.servo_output_raw.servo3_raw;
    ESC_Current[3]=__currentMessages.servo_output_raw.servo4_raw;
    ESC_Current[4]=__currentMessages.servo_output_raw.servo5_raw;
    ESC_Current[5]=__currentMessages.servo_output_raw.servo6_raw;
    ESC_Current[6]=__currentMessages.servo_output_raw.servo7_raw;
    ESC_Current[7]=__currentMessages.servo_output_raw.servo8_raw;
    ESC_Current[8]=__currentMessages.servo_output_raw1.servo1_raw;
    ESC_Current[9]=__currentMessages.servo_output_raw1.servo2_raw;
    ESC_Current[10]=__currentMessages.servo_output_raw1.servo3_raw;
    ESC_Current[11]=__currentMessages.servo_output_raw1.servo4_raw;
}

void MavLinkMessageInterface::getConnectState(uint16_t& remoteConnectState)
{
   //255未连接 100已连接
  remoteConnectState=__currentMessages.rc_channels_scaled.rssi;//radio
}

void MavLinkMessageInterface::getUAVType(uint8_t& type)
{
    type=__type;
}

void MavLinkMessageInterface::sendMessage(mavlink_message_t msg)
{
    if(__Mainci)
        __Mainci->writeMessage(msg);
    if(__Slaveci)
        __Slaveci->writeMessage(msg);
}

void MavLinkMessageInterface::logRequestList(){
    mavlink_message_t message_t;
    mavlink_log_request_list_t logRequestL;
    logRequestL.target_system = __targetSystem;
    logRequestL.target_component = __targetComponent;
    logRequestL.start = 0;
    logRequestL.end = 0xffff;
    mavlink_msg_log_request_list_encode(__targetSystem, __targetComponent, &message_t, &logRequestL);
    sendMessage(message_t);
}

void MavLinkMessageInterface::logRequestData(uint16_t id, uint32_t ofs, uint32_t count){
    mavlink_message_t message_t;
    mavlink_log_request_data_t logRequestD;
    logRequestD.target_system = __targetSystem;
    logRequestD.target_component = __targetComponent;
    logRequestD.id = id;
    logRequestD.ofs = ofs;
    logRequestD.count = count;
    mavlink_msg_log_request_data_encode(__targetSystem, __targetComponent, &message_t, &logRequestD);
    sendMessage(message_t);
}

void MavLinkMessageInterface::sendHeartBeat()
{
    mavlink_message_t message_t;
    mavlink_heartbeat_t heartbeat;
    heartbeat.type=0;
    mavlink_msg_heartbeat_encode(__targetSystem,__targetComponent,&message_t,&heartbeat);
    sendMessage(message_t);
}
void MavLinkMessageInterface::getBatteryVolt(float& volt,float& batteryRemaining)
{
   volt=__currentMessages.sys_status.voltage_battery;
   batteryRemaining=__currentMessages.sys_status.battery_remaining;
}
//使用摇杆：flag= 1， 未使用摇杆，flag = 0
void MavLinkMessageInterface::getRCType(bool& flag){
    uint16_t mtp=__currentMessages.sys_status.errors_count4>>8;
    flag = mtp;
}

void MavLinkMessageInterface::getSystemStatus(uint32_t& sys_sts,uint16_t& gps_sts,uint16_t& rtk_sts,uint16_t& psirtk_sts)
{
    sys_sts= __currentMessages.sys_status.onboard_control_sensors_health;
    gps_sts= __currentMessages.sys_status.errors_count1;
    rtk_sts= __currentMessages.sys_status.errors_count2;
    psirtk_sts= __currentMessages.sys_status.errors_count3;
}

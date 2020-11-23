#ifndef VEHICLESTATE
#define VEHICLESTATE

#include "../opmapcontrol_ex/internals/pointlatlng.h"
//#include <common/mavlink.h>
#include "connections/CommonStru.h"

typedef struct  VehicleDataSTRUCT{
    //常用属性
    int m_Id;
    float X;
    float Y;
    float Z;
    float latitude;
    float lontitude;
    float height;

    float roll;
    float yaw;
    float pitch;

    uint16_t throttle;     // 油门

    float airspeed;
    float groundspeed;
    float distance;     // 行驶距离--如何计算待定

    float batterypercent;
    float batteryVolt;

    int starNum;
    float vspeed;

    int16_t heading;
    float climb;

    long signalValue;
    int  fix_type;
    float MainVoltage;     //主电电压
    float BakVoltage;     //备电电压
    float Temperature;    //主板温度
    float ESC_Current[12];//电机转速
    bool  isJS; //是否使用摇杆，1为使用遥感，0为使用遥控

    internals::PointLatLng homepos;// 家的坐标,初始起飞第一次定位获取的坐标位置。
    VehicleDataSTRUCT(){
        m_Id = 0;
        X = 0;
        Y = 0;
        Z = 0;
        latitude = 0;
        lontitude = 0;
        height = 0;

        roll = 0;
        yaw = 0;
        pitch = 0;

        throttle = 0;     // 油门

        airspeed = 0;
        groundspeed = 0;
        distance = 0;     // 行驶距离--如何计算待定

        batterypercent = 0;
        batteryVolt = 0;
        signalValue=0;
        fix_type=0;
        MainVoltage = 0;
        BakVoltage = 0;
        Temperature=0;

		for(int i = 0; i < 8; i++){
            ESC_Current[i] = 0;
        }
        isJS = 0;
    }
}VehicleState;

#define LOG_MAGIC           0xA395
#define LOG_TYPE_Message            0xa1
#define LOG_TYPE_ShareObjects       0xa2
#define LOG_MAX_MSG_LEN     244
#define LOG_DIR             "./data/vehicleLog/"
typedef struct _LogData{
    uint16_t magic;
    uint16_t index;
    uint16_t size;
    uint8_t type;
    uint8_t count;
    uint32_t time_stamp;
    uint8_t msg[LOG_MAX_MSG_LEN];
    _LogData(){
        memset(msg, 0, LOG_MAX_MSG_LEN);
    }
}LogData;

#endif // VEHICLESTATE

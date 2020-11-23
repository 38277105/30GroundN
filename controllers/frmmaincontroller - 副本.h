#ifndef FRMMAINCONTROLLER_H
#define FRMMAINCONTROLLER_H

#include <QObject>
#include <frmmain.h>

#include <connections/linkmanager.h>
#include <vehicles/vehiclemanager.h>
#include <vehicles/vehicle.h>
#include "vehicles/vehiclestate.h"
#include <connections/communicationinterface.h>
#include <QTimer>
#include "controls/commoncontrol/croundprogressbar.h"
#include "controls/MapWidget.h"
#include "controllers/parametercontroller.h"
#include  "controllers/runninglog.h"
class NetControlTo206;
class FrmMainController : public QObject
{
    Q_OBJECT
public:
    explicit FrmMainController(QObject *parent = 0);

    ~FrmMainController();

    void SetControl(frmMain* m_control);

    void SetPortName(QString PortName,int BaudRate);

    VehicleManager* getVehicleManager();

    void stop();

    void run();

    static FrmMainController* Instance();

    frmMain*                m_frmMain;

    enum StreamSpeedType
    {
        Fast,
        Slow,
        Stop
    };
signals:
    void updateAttitude(double roll, double pitch, double yaw, quint64 timestamp);

    void speedChanged(double groundspeed, double airspeed);

    void altitudeChanged(double altitudeAMSL, double altitudeRelative, double climbRate);

    void dataparam_update(VehicleState& data);

    //更新UAV在地图上的位置
    void UpdateUAVState(internals::PointLatLng pos,double direction,int height);

    //更新航路点
    int signal_showWPtoMap(const QList<ProtocolDataType>& wList);

    //更新Home
    void HomePos_Updated(internals::PointLatLng pos,double height);

    //校准Magnify
    void showCmdInfo(int cmd,QString text);
    void textMessageReceived(int uasid, int componentid, int severity, QString text);

    void remoteControlChannelRawChanged(int chan,uint16_t val);


public slots:

    void Timer_Tick();

    /***************CMD******************/



    void btn_send_land_cmd_click();

    void btn_send_setalt_cmd_click();

    void btn_send_setRTL_click();

    void btn_send_getwp_cmd_click();

    void btn_send_gethome();

    void btn_set_speed();

    void set_home(internals::PointLatLng pos,int height);

    void btn_send_wayPoints_click(const QList<ProtocolDataType>& wpItems);

    void btn_send_disarmed_click();

    void request_parameterList();

    void set_parameter(char* param_id,float param_value,MAV_PARAM_TYPE para_type);

    /************DATA_RECV***************/

    void onMissionList(std::vector<mavlink_mission_item_int_t*> items);


    void onStatusText(mavlink_statustext_t text);

    void onHomePosition(mavlink_home_position_t position);

    void onCommandAck(uint16_t cmd, uint8_t res);

    void setMode(int mode);

    void setRCType(int flag);

    void delay_requestStream();

    /********************************/
    //校准Compass

    void startCompassMotCalibration();

    void cancelCompassMotCalibration();

    void SendCommandACK(uint16_t cmd, uint8_t res);

    void handleRCCalibration(mavlink_message_t msg);

    /********启用电量*********/
    void EnableBatteryMot(bool enable);

    //下载日志
    void DownloadLog_slot();
    void requestLogData_slot(uint16_t,uint32_t,uint32_t);
private:
    QString                 m_PortName;
    int                     m_BaudRate;

    QTimer*                 m_timer;
    QTimer*                 m_delay_timer;
//    QTimer*                 m_homeget_timer;
    QTimer*                 m_paramFirstLoadTimer;

     int                    UPDATE_INTERVAL;
     int                    SKIP_COUNT;
     int                    SKIP_INDEX;

     int                    SKIP_HEARTBEAT_COUNT;
     int                    SKIP_HEARTBEAT_INDEX;
    //----------------------------------
     QList<MultiData>*      m_replacelist;

    //-----------快捷指令----------------
     bool IsArmed;
    //----------------------------------

    //206所网络数据发送
    NetControlTo206*         m_pNet206;
public:
    LinkManager*            __linkManager;
    VehicleManager*         __vehicheManager;
    CommunicationInterface* __communicationInterface1;
    CommunicationInterface* __communicationInterface2;
    Vehicle*                __vehicle;
    ParameterController*    m_paramcontroller;
    runningLog              m_runninglog;
    //-----------串口连接超时锁-----------------------
    // MutexData* m_lockData;

     void     __requestDataStream(StreamSpeedType speed);

     // static void      SerialOpentimeOuted();

    //---------------------------------
private:
    void     __InitialLink();
    Vehicle* __switchVehicle(int id = 0);

    void     __connectSignal();
};

#endif // FRMMAINCONTROLLER_H

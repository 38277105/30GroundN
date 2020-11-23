#ifndef FRMMAIN_H
#define FRMMAIN_H

//#ifndef AIRSENSOR_ENABLED
//#define AIRSENSOR_ENABLED
//#endif

//#ifndef AERIALVideo_ENABLED
//#define AERIALVideo_ENABLED
//#endif

#ifndef ZYFlightMode_ENABLED
#define ZYFlightMode_ENABLED
#endif

#ifndef ZYMAVLinkTest_ENABLED
#define ZYMAVLinkTest_ENABLED
#endif

#include <QWidget>
#include <QKeyEvent>
#include "controls/PrimaryFlightDisplay.h"
#include <QList>
#include "settingcontrols/settingmanager.h"
#include "vehicles/vehiclestate.h"
#include "controls/paramrightcontrol.h"
#include <QComboBox>
#include <QPushButton>
#include "controls/Sensor/SensorStruct.h"
#include "controls/mapcontrols/showcurrentwaypoints.h"
#include "QDesktopWidget"
#include "controls/AerialVideo/commonvideoplayer.h"
#include "controls/AerialVideo/camera.h"
#include "controls/InfoPanel/statusmonitor.h"
#include "controls/customsettings.h"


class DataParameterView;
class ParameterTableView;
class QwtPlot;
class QwtPlotCurve;
class FrmMainController;
class MapWidgetContainer;
class MapWidgetEditor;
class CurveController;
class DataBoxResolver;
class DataParameterView;
class SensorTableView;
class SerialPortFinder;
class OutPutLogger;
class CalibrateForm;
class SignalControl;
class SatelliteControl;
class BatteryControl;
class RefreshControl;
class WarningForm;
namespace Ui {
class frmMain;
}


enum copterMode {
STABILIZE   = 0,   // hold level position
ACRO        = 1,   // rate control
ALT_HOLD    = 2,   // AUTO control
AUTO        = 3,   // AUTO control
GUIDED      = 4,   // AUTO control
LOITER      = 5,   // Hold a single location
RTL         = 6,   // AUTO control
CIRCLE      = 7,   // AUTO control
POSITION    = 8,   // AUTO control
LAND        = 9,   // AUTO control
OF_LOITER   = 10,  // Hold a single location using optical flow
                   // sensor
DRIFT       = 11,  // Drift 'Car Like' mode
RESERVED_12 = 12,  // RESERVED FOR FUTURE USE
SPORT       = 13,  // [TODO] Verify this is correct.
FLIP        = 14,
AUTOTUNE    = 15,
POS_HOLD    = 16, // HYBRID LOITER.
BRAKE       = 17
};
enum zyFlightMode{
MAIN_STATE_MANUAL = 0, //no use
MAIN_STATE_ALTCTL = 1,
MAIN_STATE_POSCTL = 2,
MAIN_STATE_AUTO_MISSION = 3,
MAIN_STATE_AUTO_LOITER = 4,
MAIN_STATE_AUTO_RTL = 5,
MAIN_STATE_ACRO = 6,    //no use
MAIN_STATE_OFFBOARD = 7, //no use
MAIN_STATE_STAB = 8,
MAIN_STATE_RATTITUDE =9,    //no use
MAIN_STATE_AUTO_TAKEOFF =10,
MAIN_STATE_AUTO_LAND =11,
MAIN_STATE_AUTO_FOLLOW_TARGET =12,  //no use
MAIN_STATE_MAX =13  //no use
};



class frmMain : public QWidget
{
    Q_OBJECT

public:
    explicit frmMain(QWidget *parent = 0);
    ~frmMain();

    Ui::frmMain *ui;
    //controls
    //姿态控件
    PrimaryFlightDisplay * m_flightdisplay;

    //姿态相关数据的显示控件
    DataParameterView* m_datapview;

    //参数设置控件
    ParameterTableView* m_paramView;

    ParamRightControl* m_paramRightControl;

    CustomSettings*    m_settingControl;

    CalibrateForm* m_calibrateForm;


    FrmMainController* m_FrmMainController;

    MapWidgetContainer* m_mapcontrol;

    MapWidgetEditor* m_mapEditor;

    CommonVideoPlayer* m_videoPlayer;

    Camera*            m_camera;

    CurveController* m_curveController;

    SerialPortFinder* m_serialFinder;

    OutPutLogger*     m_outputLoger;

    SignalControl*    m_signal_c;
    SignalControl*    m_flymode_c;
    SignalControl*    m_volt_c;
    SatelliteControl* m_sate_c;
    SatelliteControl* m_sate_fix_type_c;
    BatteryControl*   m_battery_c;
    RefreshControl*   m_refresh_c;
    SignalControl*    m_lockedstate_c;
    WarningForm*    m_warningForm;
    /***传感器**/
    QList<UAVDdataStruct>* _datalist;
    QList<AIRSENSORSTR>*   _sensordatalist;
    DataBoxResolver*       _resolver;
    SensorTableView*       _sensorView;
    QComboBox *            cbx_sensor_ports;
    QComboBox *            cbx_sensor_ports_baudrate;
    QPushButton *          btn_sensor_connect;
    int                    index_saveTag;
    int updatetag;//write to csv tag
     /***传感器**/
    StatusMonitor*     m_statusMonitor;
public Q_SLOTS:

    void sensor_connect_clicked();

    void data_recv(AIRSENSORSTR& airSensorStr, UAVDATASTR& uavDataStr);

    void deleteOneWP_slots(int index);

    void updateOneWP_slots(const int& row, const MapPointData& oneWP);

    //显示FlightMode
    void UpdateFlightMode(int mode);


    void showFlyControl();
    void showFlyPlan();
    void showDataCurve();
    void showParam();
    void updateRCType(bool flag);//是否启用摇杆,1为启用，0为未启用

    void keyReleaseEvent(QKeyEvent *event);
private:
    bool  m_isPositionMode;
    QRect location;
    QList<QObject*> m_resourcelst;


    /************图表控件*************/
     QwtPlot* plot;
     QwtPlotCurve* curve;
    /*************图标控件************/

     ShowCurrentWayPoints* m_newSC;

     QRect m_WinRect;


     //初始化样式
    void InitStyle();

    void initialMenu();

    void InitialStatusControl();

    void initPortInfo();

    void initPortBaudList(QComboBox* m_box);

    //初始化飞行模式到下拉列表
    void InitFlightMode();

    //初始化遥控器类型下拉列表
    void InitRCType();

    //加载飞行控制布局
    void LoadLayout();

    //加载飞行计划布局
    void PlanLoadLayout();

    //加载数据分析布局
    void DataLoadLayout();

    //加载vlc视频影响
    void LoadVideoLayout();

    //初始化配置页面
    void ConfigLoadLayout();

    void LoadSensorLayout();

    void LoadCalibrate();
    //初始化固件烧写页面
    void LoadFirmWare();

    //初始化mavlink测试页面
    void LoadMavTest();
    //删除指定tab页
    void removeTab(const QString& tabName);

    //公用方法，为控件添加背景颜色
    void setBackground(QWidget* wid,QColor color);

    void setBackgroundImage(QWidget* control,QString path,int width,int height);

    void setOpacity(QWidget* control);
private slots:
    void upOneWayPointData_slots(int row);
    void downOneWayPointData_slots(int row);
    void on_cbx_flightMode_currentIndexChanged(int index);

    void on_cbx_rcType_currentIndexChanged(int index);

signals:
    void signal_updateShowList(const QList<ProtocolDataType>&);
    void signal_throttleChanged(qint32 throttle);


public:
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    HANDLE hOnlyOnceMutex;//保证程序只能启动一次

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent * event);



public slots:
public Q_SLOTS:
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();
    void OnFlushFile();
    void setTabEnabled(bool bIsTabEnabled);

};

extern bool isCommConnected;//串口是否连接
#endif // FRMMAIN_H

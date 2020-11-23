#include "frmmaincontroller.h"
#include "controls/dataparameterview.h"
#include "ui_frmmain.h"
#include "ui_dataparameterview.h"
#include "controls/MapWidgetEditor.h"
#include "controls/paramrightcontrol.h"
#include "ui_paramrightcontrol.h"
#include "connections/mavlinkmessageinterface.h"
#include <QDebug>
#include "controls/MapWidgetContainer.h"
#include "controls/parametertableview.h"
#include "controllers/curvecontroller.h"
#include "loghelper/outputlogger.h"
#include "controls/statuscontrol/signalcontrol.h"
#include "controls/statuscontrol/satellitecontrol.h"
#include "controls/statuscontrol/batterycontrol.h"
#include "controls/statuscontrol/warningform.h"
#include "Utils/zygroundglobalconfig.h"
#include "controls/statuscontrol/refreshcontrol.h"
#include "ui_refreshcontrol.h"
#include "../connections/UDP/netcontrolto206.h"
#include "controls/Joystick/joysticktoflycontroller.h"
#include "controls/joystick/zystick.h"

static FrmMainController* m_instance=NULL;

static internals::PointLatLng homePoint;

FrmMainController::FrmMainController(QObject *parent) : QObject(parent)
{
    __linkManager=NULL;
    m_frmMain=NULL;
    m_timer=NULL;

    __vehicheManager=NULL;
    __communicationInterface1=NULL;
    __communicationInterface2=NULL;
    __vehicle=NULL;
    m_paramcontroller=NULL;
    m_replacelist=NULL;
    IsArmed=false;

    m_instance=this;
//    if(ZYGroundGlobalConfig::RESOLUTION_1920)
//    {
//        UPDATE_INTERVAL = 200;
//    }
//    else
//    {
//        UPDATE_INTERVAL = 500;
//    }

    UPDATE_INTERVAL = 200;
     //m_homeget_timer=NULL;
    m_delay_timer=NULL;
    m_paramFirstLoadTimer=NULL;
    //m_lockData=new MutexData();
    m_pNet206=NULL;

    bool bUsedMyStick=true;
    if(ZYGroundGlobalConfig::m_sStickPortName=="")
        bUsedMyStick=false;
    if(bUsedMyStick)
    {
        m_pZYStick=new ZYStick();
        m_pJoystick=NULL;
    }
    else
    {
        m_pZYStick=NULL;
        m_pJoystick=new JoyStickToFlyController();
    }
}

FrmMainController::~FrmMainController()
{
    if(m_pJoystick)
    {
        delete m_pJoystick;
        m_pJoystick=NULL;
    }
    if(m_pZYStick)
    {
        delete m_pZYStick;
        m_pZYStick=NULL;
    }
    __vehicheManager->removeAllVehicles();

}

void FrmMainController::__InitialLink(){
    __linkManager = LinkManager::getInstance();
    __vehicheManager = VehicleManager::getInstance();

    __vehicle = __vehicheManager->addVehicle(this);
    __communicationInterface1 = __linkManager->addLink(SerialLinkType);
    ZYGroundGlobalConfig::m_SerialIsOpen = __communicationInterface1->linkOpen(m_PortName, m_BaudRate);
    qDebug()<<"ci1 open="<<ZYGroundGlobalConfig::m_SerialIsOpen;
    bool bRet=true;
    if(ZYGroundGlobalConfig::m_bUsedSlavePort)
    {
        __communicationInterface2 = __linkManager->addLink(SerialLinkType);
        bRet=__communicationInterface2->linkOpen(ZYGroundGlobalConfig::m_sSlavePortName, ZYGroundGlobalConfig::m_sSlaveBaudRate);
        qDebug()<<"ci2 open="<<bRet;
        ZYGroundGlobalConfig::m_bUsedSlavePort=bRet;
    }
    __vehicle->setLink(__communicationInterface1,__communicationInterface2);

    if(!bRet)
        QMessageBox::warning(NULL,"Open",QString::fromLocal8Bit("打开备用串口失败"));

    if(m_paramcontroller!=NULL)
    {
        delete m_paramcontroller;
        m_paramcontroller=NULL;
    }
     m_paramcontroller=new ParameterController(__vehicle);

     m_paramcontroller->set_callback(this->m_frmMain->m_paramView);

     m_paramcontroller->SetControl(m_frmMain);
     connect(__vehicle->mavLinkMessageInterface.getCommandHandle(),SIGNAL(onCommandAck(uint16_t, uint8_t)),this,SLOT(onCommandAck(uint16_t, uint8_t)));

     connect(&__vehicle->mavLinkMessageInterface,SIGNAL(onHomePosition(mavlink_home_position_t)),this,SLOT(onHomePosition(mavlink_home_position_t)));
     //connect(&__vehicle->mavLinkMessageInterface,SIGNAL(onStatusText(mavlink_statustext_t)),this,SLOT(onStatusText(mavlink_statustext_t)),Qt::AutoConnection);
     connect(&__vehicle->mavLinkMessageInterface,SIGNAL(handleRCCalibration(mavlink_message_t)),this,SLOT(handleRCCalibration(mavlink_message_t)));
     //connect(&__vehicle->mavLinkMessageInterface, SIGNAL(logEntry_recved(mavlink_log_entry_t)), this->m_frmMain->m_curveController, SLOT(rcv_logEntry(mavlink_log_entry_t)));
     //connect(&__vehicle->mavLinkMessageInterface, SIGNAL(logData_recved(mavlink_log_data_t)), this->m_frmMain->m_curveController, SLOT(rcv_logData(mavlink_log_data_t)));
}

Vehicle* FrmMainController::__switchVehicle(int id){
    return __vehicheManager->getActiveVehicle();
 }

void FrmMainController::onCommandAck(uint16_t cmd, uint8_t res)
{
     //this->m_frmMain->m_outputLoger->getBrowser()->append(QString::asprintf("received->>>cmd:%d|res:%d",cmd,res));
    switch(res)
    {
        case MAV_RESULT_ACCEPTED:
        {
            emit textMessageReceived(1, 0, 0, tr("SUCCESS: Executed CMD: %1").arg(cmd));
            emit showCmdInfo(cmd,tr("SUCCESS: Executed CMD: %1").arg(cmd));
        }
            break;
        case MAV_RESULT_TEMPORARILY_REJECTED:
        {
            emit textMessageReceived(1, 0, 0, tr("FAILURE: Temporarily rejected CMD: %1").arg(cmd));
            emit showCmdInfo(cmd,tr("FAILURE: Temporarily rejected CMD: %1").arg(cmd));
        }
            break;
        case MAV_RESULT_DENIED:
        {
            emit textMessageReceived(1, 0, 0, tr("FAILURE: Denied CMD: %1").arg(cmd));
            emit showCmdInfo(cmd,tr("FAILURE: Denied CMD: %1").arg(cmd));
        }
            break;
        case MAV_RESULT_UNSUPPORTED:
        {
            emit textMessageReceived(1, 0, 0, tr("FAILURE: Unsupported CMD: %1").arg(cmd));
            emit showCmdInfo(cmd,tr("FAILURE: Unsupported CMD: %1").arg(cmd));
        }
            break;
        case MAV_RESULT_FAILED:
        {
            emit textMessageReceived(1, 0, 0, tr("FAILURE: Failed CMD: %1").arg(cmd));
            emit showCmdInfo(cmd,tr("FAILURE: Failed CMD: %1").arg(cmd));
        }
            break;
    }
}

void FrmMainController::SetControl(frmMain* m_control)
{
    this->m_frmMain=m_control;

    //飞行控制 Events
    connect(this,SIGNAL(updateAttitude(double,double,double,quint64)),this->m_frmMain->m_flightdisplay,SLOT(updateAttitude(double,double,double,quint64)));
    connect(this,SIGNAL(altitudeChanged(double,double,double)),this->m_frmMain->m_flightdisplay,SLOT(altitudeChanged(double,double,double)));
    connect(this,SIGNAL(speedChanged(double,double)),this->m_frmMain->m_flightdisplay,SLOT(speedChanged(double,double)));

    connect(this,SIGNAL(dataparam_update(VehicleState&)),this->m_frmMain->m_datapview,SLOT(update(VehicleState&)));


    //connect(this->m_frmMain->m_datapview->btn_arm,SIGNAL(clicked()),this,SLOT(btn_send_disarmed_click()));
    //connect(this->m_frmMain->m_datapview->btn_land,SIGNAL(clicked()),this,SLOT(btn_send_land_cmd_click()));
//    connect(this->m_frmMain->m_datapview->btn_setspeed,SIGNAL(clicked()),this,SLOT(btn_set_speed()));
//    connect(this->m_frmMain->m_datapview->btn_arm,SIGNAL(clicked()),this,SLOT(btn_send_disarmed_click()));
    //connect(this->m_frmMain->m_datapview->ui->btn_RTL,SIGNAL(clicked()),this,SLOT(btn_send_setRTL_click()));


    //飞行控制-Map Events
    connect(this,SIGNAL(UpdateUAVState(internals::PointLatLng,double,int)),this->m_frmMain->m_mapcontrol,SLOT(UpdateUAVState(internals::PointLatLng,double,int)));
    //connect(this,SIGNAL(UpdateUAVState(internals::PointLatLng,double,int)),this->m_frmMain->m_mapEditor,SLOT(UpdateUAVState(internals::PointLatLng,double,int)));
    connect(this,SIGNAL(signal_showWPtoMap(const QList<ProtocolDataType>&)),this->m_frmMain,SIGNAL(signal_updateShowList(const QList<ProtocolDataType>&)));
    //connect(this,SIGNAL(showWPtoMap(const QList<struct UavWaypointData>&)),this->m_frmMain->m_mapcontrol->m_mapWidget,SLOT(showWP(const QList<struct UavWaypointData>&)));
    //connect(this,SIGNAL(showWPtoMap(const QList<struct UavWaypointData>&)),this->m_frmMain->m_mapEditor,SLOT(showWP(const QList<struct UavWaypointData>&)));

    //任务编辑器 Events
    connect(this->m_frmMain->m_mapEditor,SIGNAL(signal_sendWP(const QList<ProtocolDataType>&)),this,SLOT(btn_send_wayPoints_click(const QList<ProtocolDataType>&)));
    connect(this->m_frmMain->m_mapEditor,SIGNAL(signal_getWP()),this,SLOT(btn_send_getwp_cmd_click()));
    //connect(this->m_frmMain->m_mapEditor,SIGNAL(signal_mapWidgetEditor_clearShowList(QList<MapPointData>)),this->m_frmMain,SIGNAL(clearShowList(const QList<MapPointData>&)));
    //connect(this, SIGNAL(HomePos_Updated(internals::PointLatLng,int)), this->m_frmMain->m_mapEditor, SLOT(initSetHome(internals::PointLatLng,int)));
    connect(this, SIGNAL(HomePos_Updated(internals::PointLatLng,double)), this->m_frmMain->m_mapEditor, SLOT(initSetHome(internals::PointLatLng,double)));
    connect(this->m_frmMain->m_mapEditor, SIGNAL(signal_UpdateVehicleHome(internals::PointLatLng,int)), this, SLOT(set_home(internals::PointLatLng,int)));



    //参数设置 Events
    connect(this->m_frmMain->m_paramRightControl->ui->btn_sync_param,SIGNAL(clicked()),this,SLOT(request_parameterList()));
    connect(this->m_frmMain->m_paramView,SIGNAL(set_parameter(char*,float,MAV_PARAM_TYPE)),this,SLOT(set_parameter(char*,float,MAV_PARAM_TYPE)));
    connect(this->m_frmMain->m_refresh_c->ui->toolButton,SIGNAL(clicked()),this,SLOT(request_parameterList()));
    //slots
    if(this->m_frmMain->m_curveController!=NULL)
    {
    connect(this,SIGNAL(dataparam_update(VehicleState&)),this->m_frmMain->m_curveController,SLOT(addstate(VehicleState&)));
    connect(this->m_frmMain->m_curveController->m_btn_downloadLog, SIGNAL(clicked()), this, SLOT(DownloadLog_slot()));
    connect(this->m_frmMain->m_curveController, SIGNAL(requestLogData(uint16_t,uint32_t,uint32_t)), this, SLOT(requestLogData_slot(uint16_t,uint32_t,uint32_t)));
    }
    //connect(this,SIGNAL(params_received(QList<ParamData>*)),this->m_frmMain->m_paramView,SLOT(sync_args_msgcomes(QList<ParamData>*)));


}

void FrmMainController::SetPortName(QString PortName,int BaudRate)
{
  this->m_PortName=PortName;
  this->m_BaudRate=BaudRate;
}

void FrmMainController::btn_send_setRTL_click()
{
    if(NULL != __vehicle)
    {
        __vehicle->mavLinkMessageInterface.commandReturnToLaunch(0);
    }
}

void FrmMainController::stop()
{
    //关闭串口界面数据清0
    VehicleState t_state;
    if(m_frmMain->m_statusMonitor)
       m_frmMain->m_statusMonitor->setStatus(t_state);
    if(m_frmMain->m_warningForm)
    m_frmMain->m_datapview->UpdateStatus(0,0,0,0,0);
    if(m_pZYStick)
        m_pZYStick->linkClose();
    if(m_pJoystick)
        m_pJoystick->stopPolling();
    if(m_pNet206!=NULL)
    {
        m_pNet206->stop();
        delete m_pNet206;
        m_pNet206=NULL;
    }

    if(m_timer!=NULL)
    {
        m_timer->stop();
        delete m_timer;
        m_timer=NULL;
    }
//    if(m_homeget_timer!=NULL)
//    {
//        m_homeget_timer->stop();
//        delete  m_homeget_timer;
//        m_homeget_timer=NULL;
//    }
    if(m_paramFirstLoadTimer!=NULL)
    {
        m_paramFirstLoadTimer->stop();
        delete  m_paramFirstLoadTimer;
        m_paramFirstLoadTimer=NULL;
    }
    if(m_delay_timer!=NULL)
    {
        m_delay_timer->stop();
        delete  m_delay_timer;
        m_delay_timer=NULL;
    }

    if(m_paramcontroller!=NULL)
    {
        delete m_paramcontroller;
        m_paramcontroller=NULL;
    }

    if(__vehicle!=NULL)
    {
        delete __vehicle;
       __vehicle=NULL;
    }
    if(__communicationInterface1)
     {
        ZYGroundGlobalConfig::m_SerialIsOpen=false;
        __linkManager->removeLink(__communicationInterface1);
        __communicationInterface1->linkClose();
        __communicationInterface1=NULL;
     }
    if(__communicationInterface2)
     {
        __linkManager->removeLink(__communicationInterface2);
        __communicationInterface2->linkClose();
        __communicationInterface2=NULL;
     }
}


void FrmMainController::run()
{
    if(m_timer!=NULL)
    {

        m_timer->stop();
        delete m_timer;
        m_timer=NULL;
    }
    m_timer=new QTimer(this);
    m_timer->setInterval(UPDATE_INTERVAL);
    m_timer->setSingleShot(false);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(Timer_Tick()));
    m_timer->start();

    SKIP_COUNT=3000/UPDATE_INTERVAL;
    SKIP_INDEX=0;

    SKIP_HEARTBEAT_COUNT=1000/UPDATE_INTERVAL;
    SKIP_HEARTBEAT_INDEX=0;





//    if(m_homeget_timer!=NULL)
//    {
//        m_homeget_timer->stop();
//        delete  m_homeget_timer;
//        m_homeget_timer=NULL;
//    }
//    m_homeget_timer=new QTimer();
//    m_homeget_timer->setInterval(UPDATE_INTERVAL*15);
//    m_homeget_timer->setSingleShot(false);
//    connect(m_homeget_timer,SIGNAL(timeout()),this,SLOT(btn_send_gethome()));
//    m_homeget_timer->start();
//    m_lockData->setCallBack(10,(TimeOutCallBack&)(FrmMainController::SerialOpentimeOuted));
//    m_lockData->lock();
       __InitialLink();
//    m_lockData->lock();
//    qDebug()<<"串口已连接。正在同步数据";
	if (!ZYGroundGlobalConfig::m_SerialIsOpen)
	{
		return;
	}
    //开启实时日志
//    {
        //m_runninglog.parserunningLogFile();
//        //m_runninglog.start();
//        //connect(&__vehicle->mavLinkMessageInterface, SIGNAL(runninglog_recved(mavlink_message_t)), &m_runninglog, SLOT(messLogData(mavlink_message_t)));
        //connect(&m_runninglog, SIGNAL(runninglogMessage_siganl(mavlink_message_t)), __vehicle, SLOT(onMavLinkMessage(mavlink_message_t)));
//    }



    if(ZYGroundGlobalConfig::m_bRequestParamEnabled)  //gyqmod
    {
        if(m_paramFirstLoadTimer!=NULL)
        {
            m_paramFirstLoadTimer->stop();
            delete  m_paramFirstLoadTimer;
            m_paramFirstLoadTimer=NULL;
        }
        m_paramFirstLoadTimer=new QTimer();
        m_paramFirstLoadTimer->setSingleShot(true);
        m_paramFirstLoadTimer->setInterval(2000);
        connect(m_paramFirstLoadTimer,SIGNAL(timeout()),this,SLOT(request_parameterList()));
        m_paramFirstLoadTimer->start();
    }

    if(m_delay_timer!=NULL)
    {
        m_delay_timer->stop();
        delete  m_delay_timer;
        m_delay_timer=NULL;
    }
    m_delay_timer=new QTimer();
    m_delay_timer->setSingleShot(true);
    m_delay_timer->setInterval(500);
    connect(m_delay_timer,SIGNAL(timeout()),this,SLOT(delay_requestStream()));
    m_delay_timer->start();

    //gyq add 206suo
    if(ZYGroundGlobalConfig::m_bUsedNet206)
    {
        m_pNet206 = new NetControlTo206(ZYGroundGlobalConfig::m_sNet206IP,
                        ZYGroundGlobalConfig::m_nNet206Port,&__vehicle->mavLinkMessageInterface);
        m_pNet206->start();
        qDebug()<<"206 Net Start.........";
    }
}



void FrmMainController::delay_requestStream()
{
  __requestDataStream(Slow);

  QThread::msleep(50);
  EnableBatteryMot(true);
}

FrmMainController* FrmMainController::Instance()
{
  if(m_instance==NULL)
      m_instance= new FrmMainController();
   return m_instance;
}

void FrmMainController::Timer_Tick()
{
    if(!__vehicle)
        return;
    VehicleState t_state;
    __vehicle->getVehicleState(t_state);
    if(m_frmMain->m_statusMonitor)
        m_frmMain->m_statusMonitor->setStatus(t_state);

    emit updateAttitude(t_state.roll,t_state.pitch,t_state.yaw,1);
    emit this->speedChanged(t_state.groundspeed,t_state.airspeed);
    emit this->altitudeChanged(t_state.height,1.0,0.5);
    //emit dataparam_update(t_state);

    internals::PointLatLng t_pos;
    t_pos.SetLat(t_state.latitude);
    t_pos.SetLng(t_state.lontitude);

    if(SKIP_INDEX==SKIP_COUNT)
    {
        emit UpdateUAVState(t_pos,t_state.yaw,t_state.height);
        SKIP_INDEX=0;
    }
    SKIP_INDEX++;
    //心跳数据添加by xingjiwei 2017-01-17
    if(SKIP_HEARTBEAT_INDEX==SKIP_HEARTBEAT_COUNT)
    {
        __vehicle->checkConn();
        this->__vehicle->mavLinkMessageInterface.sendHeartBeat();
        //send heartbeat;
        SKIP_HEARTBEAT_INDEX=0;
    }
        SKIP_HEARTBEAT_INDEX++;

    bool armState=__vehicle->mavLinkMessageInterface.getArmedState();   //获取锁定状态
    if(armState)//锁定状态下起飞按钮不可用
    {
      this->m_frmMain->m_datapview->btn_takeoffOnekey->setEnabled(false);
      this->m_frmMain->m_datapview->btn_takeoffOnekey->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
                                                                     font: 14pt \"黑体\";\
                                                                     border-top-left-radius:8px;\
                                                                     border-bottom-left-radius:8px;\
                                                                     background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
                                                                     stop:0 rgba(185, 185, 185, 255),\
                                                                    stop:1 rgba(135, 135, 135, 255));"));
    }
    else
    {
       this->m_frmMain->m_datapview->btn_takeoffOnekey->setEnabled(true);//打开串口成功，使起飞按钮可用


       this->m_frmMain->m_datapview->btn_takeoffOnekey->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
                                                                                     font: 14pt \"黑体\";\
                                                                                     border-top-left-radius:8px;\
                                                                                     border-bottom-left-radius:8px;\
                                                                                     background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
                                                                                     stop:0 rgba(225, 225, 225, 255),\
                                                                                     stop:1 rgba(185, 185, 185, 255));"));
    }

    //系留报警
    uint16_t gps_sts,rtk_sts,psi_sts;
    uint32_t sys_sts;
    __vehicle->mavLinkMessageInterface.getSystemStatus(sys_sts,gps_sts,rtk_sts,psi_sts);
    this->m_frmMain->m_datapview->UpdateStatus(sys_sts,gps_sts,rtk_sts,psi_sts,t_state.isJS);
    if(t_state.isJS)
    {
        if(m_pZYStick && !m_pZYStick->isOpened())
        {
            bool ret=m_pZYStick->linkOpen(ZYGroundGlobalConfig::m_sStickPortName,ZYGroundGlobalConfig::m_sStickBaudRate);
            if(!ret)
                qDebug()<<"Open ZY Stick failed";
            else
                qDebug()<<"Open ZY Stick success";
        }
        else if(m_pJoystick && !m_pJoystick->isOpened())
        {
            m_pJoystick->startPolling();
            qDebug()<<"Open Joy Stick success";
        }
    }
    else
    {
        if(m_pZYStick && m_pZYStick->isOpened())
        {
            m_pZYStick->linkClose();
            qDebug()<<"ZY Stick closed";
        }
        else if(m_pJoystick && m_pJoystick->isOpened())
        {
            m_pJoystick->stopPolling();
            qDebug()<<"Joy Stick closed";
        }
    }

}



//
//降落
//
void FrmMainController::btn_send_land_cmd_click()
{
    if(NULL != __vehicle)
    {
        VehicleState t_state;
        __vehicle->getVehicleState(t_state);
        __vehicle->mavLinkMessageInterface.commandNavLand(1,t_state.yaw,t_state.lontitude,t_state.latitude,0);
    }
    else
    {
        //Log.Warning("没有发现可用的UAV对象");
    }
}

//
//改变高度
//
void FrmMainController::btn_send_setalt_cmd_click()
{
    if(NULL != __vehicle )
    {
       //float alt_value= this->m_frmMain->m_datapview->lineEdit->text().toFloat();
       //__vehicle->mavLinkMessageInterface.commandConditionChangeALT(0,1,alt_value);
    }
    else
    {
        //Log.Warning("没有发现可用的UAV对象");
    }
}

//改变速度
void FrmMainController::btn_set_speed()
{
    if(NULL != __vehicle )
    {
//       float speed_value= (float)this->m_frmMain->m_datapview->lineEdit->text().toFloat();
//       __vehicle->mavLinkMessageInterface.commandChangeSpeed(0,1,speed_value,0);//1:groundspeed,0:airspeed
    }
    else
    {
        //Log.Warning("没有发现可用的UAV对象");
    }
}

//获取Home点
void FrmMainController::btn_send_gethome()
{
    if(NULL != __vehicle)
    {
        __vehicle->mavLinkMessageInterface.commandGetHomePosition(0);
        //qDebug("---send commandGetHomePosition(0)!---");
    }
}

//收到Home点
void  FrmMainController::onHomePosition(mavlink_home_position_t position)
{
    homePoint.SetLat(position.latitude);
    homePoint.SetLng(position.longitude);
    internals::PointLatLng pos;
    pos.SetLat(position.latitude);
    pos.SetLng(position.longitude); 
    emit HomePos_Updated(pos,position.altitude);
    //qDebug()<<"X_DEBUG>lat:"<<position.latitude<<"lon:"<<position.longitude<<"alt:"<<position.altitude;
}

//
//获取航路点
//
void FrmMainController::btn_send_getwp_cmd_click()
{
    if(NULL != __vehicle)
    {
        disconnect(__vehicle->mavLinkMessageInterface.getMissionHandle(), SIGNAL(onMissionList(std::vector<mavlink_mission_item_int_t*>)), this, SLOT(onMissionList(std::vector<mavlink_mission_item_int_t*>)));
        connect(__vehicle->mavLinkMessageInterface.getMissionHandle(), SIGNAL(onMissionList(std::vector<mavlink_mission_item_int_t*>)), this, SLOT(onMissionList(std::vector<mavlink_mission_item_int_t*>)));
        __vehicle->mavLinkMessageInterface.readMavWaypointList();
        //qDebug("---send readMavWaypointList()!---");
        //QMessageBox::warning(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("下载航路完成"), QMessageBox::Yes);
    }
}

//
//写入航路点
//
void FrmMainController::btn_send_wayPoints_click(const QList<ProtocolDataType>& wpItems)
{
    if(NULL != __vehicle)
    {
        std::vector<mavlink_mission_item_int_t*> items;
        items.clear();
        if(wpItems.length()>0)
        {
            mavlink_mission_item_int_t* item = new mavlink_mission_item_int_t ;
            item->param1 = 0;//
            item->param2 = 5;
            item->param3 = 0;
            item->param4 = 0;
            item->x      = wpItems[0].x*10000000;
            item->y      = wpItems[0].y*10000000;
            item->z      = wpItems[0].z; //xjw ??? need fix 01;

            item->seq = 0;
            item->command = MAV_CMD_NAV_WAYPOINT;
            item->frame = MAV_FRAME_GLOBAL;
            item->current = 1;
            item->autocontinue = 1;
            items.push_back(item);
        }

        for(int i=0;i<wpItems.size();i++)
        {
             mavlink_mission_item_int_t* item = new mavlink_mission_item_int_t ;
             item->param1 = 0;//
             item->param2 = 5;
             item->param3 = 0;
             item->param4 = 0;
             item->x      = wpItems[i].x*10000000;
             item->y      = wpItems[i].y*10000000;
             item->z      = wpItems[i].z;
 //              this->m_frmMain->m_outputLoger->getBrowser()->append(QString::asprintf(">>>>>>send alt:%d",item->z));
             item->seq = i+1;
             item->command = wpItems[i].command;
             item->frame = MAV_FRAME_GLOBAL_RELATIVE_ALT;
             item->current = 1;
             item->autocontinue = 1;
             items.push_back(item);
        }
        //qDebug("------begin writeMavWaypointList(items) !-----");
        __vehicle->mavLinkMessageInterface.writeMavWaypointList(items);
       // qDebug("------end writeMavWaypointList(items) !-----");
    }
    else
    {
        //Log.Warning("没有发现可用的UAV对象");
    }
    QMessageBox::warning(NULL,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("上传完成"),QMessageBox::Yes);
}


//上锁/解锁
void FrmMainController::btn_send_disarmed_click()
{
    if(NULL != __vehicle)
    {
        if(IsArmed)
        {
            __vehicle->mavLinkMessageInterface.commandComponentArmDisarm(0,0);
            //this->m_frmMain->m_datapview->btn_arm->setText(QString::fromLocal8Bit("解锁"));
        }
        else
        {
            __vehicle->mavLinkMessageInterface.commandComponentArmDisarm(0,1);
            //this->m_frmMain->m_datapview->btn_arm->setText(QString::fromLocal8Bit("锁定"));
        }
        IsArmed=!IsArmed;
    }
    //已经发送解锁/锁定命令，等待飞控返回
}
//
//获取参数列表
//
void FrmMainController::request_parameterList()
{

    if(NULL != __vehicle)
    {
       m_paramcontroller->request_parameterList();
       //qDebug()<<"m_paramcontroller->request_parameterList()";
    }
}

//
//设置参数
//
void FrmMainController::set_parameter(char* param_id,float param_value,MAV_PARAM_TYPE para_type)
{
    //qDebug() << "param_id: " << param_id << "/" << param_value << "/" << para_type;
    if(NULL != __vehicle)
    {
       m_paramcontroller->set_parameter(param_id,param_value,para_type);
    }
    else
    {
        //Log.Warning("没有发现可用的UAV对象");
    }
}

//
//任务列表信号
//

void FrmMainController::onMissionList(std::vector<mavlink_mission_item_int_t*> items){

    if(0 == items.size())
    {
        return;
    }
    std::vector<mavlink_mission_item_int_t*>::const_iterator _iterator=items.begin();


    //home点通知 add by xjw
    internals::PointLatLng pos;
    pos.SetLat((double)(((mavlink_mission_item_int_t*)(*_iterator))->x)/10000000.0);
    pos.SetLng((double)(((mavlink_mission_item_int_t*)(*_iterator))->y)/10000000.0);
    emit HomePos_Updated(pos,((mavlink_mission_item_int_t*)(*_iterator))->z);
    //home点通知


      QList<ProtocolDataType> t_wpoints;
    for(;_iterator!=items.end();_iterator++)
    {
        ProtocolDataType temp;
        temp.x=(double)(((mavlink_mission_item_int_t*)(*_iterator))->x)/10000000.0;
        temp.y=(double)(((mavlink_mission_item_int_t*)(*_iterator))->y)/10000000.0;
        temp.z=((mavlink_mission_item_int_t*)(*_iterator))->z;
        temp.command = ((mavlink_mission_item_int_t*)(*_iterator))->command;

         //qDebug()<<"...................."<<((mavlink_mission_item_int_t*)(*_iterator))->z<<".................";
        t_wpoints.append(temp);
    }
     //qDebug("---waypoint received success!---");

    t_wpoints.pop_front();
    if(t_wpoints.size()>0){
        emit signal_showWPtoMap(t_wpoints);
        QMessageBox::warning(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("下载航路完成"), QMessageBox::Yes);
    }else{
        QMessageBox::warning(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("下载航路失败"), QMessageBox::Yes);
    }
}
//
//status text
//
void FrmMainController::onStatusText(mavlink_statustext_t text){

    if(m_replacelist==NULL)
    {
       m_replacelist= SettingManager::getMultilang();
    }
    QString contentstr=QString::asprintf("%s",text);
    emit textMessageReceived(1, 0, text.severity, QString::asprintf("%s",text.text));

   contentstr=contentstr.replace("APM", "SYSTEM");
   QList<MultiData>::const_iterator itr= m_replacelist->begin();
   for(;itr!=m_replacelist->end();itr++)
   {
     contentstr=contentstr.replace((*itr).En, (*itr).Zh_CN);
   }

    this->m_frmMain->m_outputLoger->getBrowser()->append(contentstr);


}

//设置模式
void FrmMainController::setMode(int mode)
{
    if(NULL != __vehicle)
    {
        __vehicle->mavLinkMessageInterface.setMode(MAV_MODE_FLAG_CUSTOM_MODE_ENABLED,mode);
    }
}

//设置是否启用摇杆 启用：1， 禁用： 0
void FrmMainController::setRCType(int flag){
    if(NULL != __vehicle)
    {
        __vehicle->mavLinkMessageInterface.setRCType(flag);
    }
}

//
//请求数据流
//
void FrmMainController::__requestDataStream(StreamSpeedType speed)
{

    if(NULL != __vehicle)
    {

        if(speed==Fast)
        {
            __vehicle->mavLinkMessageInterface.requestDataStreamAll(1,8);
            __vehicle->mavLinkMessageInterface.requestDataStreamExtra1(1, 10);
            __vehicle->mavLinkMessageInterface.requestDataStreamExtra2(1, 10);
            __vehicle->mavLinkMessageInterface.requestDataStreamExtra3(1, 10);
        }
        else if(speed==Slow)
        {
            __vehicle->mavLinkMessageInterface.requestDataStreamAll(1,1);
            __vehicle->mavLinkMessageInterface.requestDataStreamExtra1(1, 1);
            __vehicle->mavLinkMessageInterface.requestDataStreamExtra2(1, 1);
            __vehicle->mavLinkMessageInterface.requestDataStreamExtra3(1, 1);
        }
        else if(speed==Stop)
        {
            __vehicle->mavLinkMessageInterface.requestDataStreamAll(1,0);
            __vehicle->mavLinkMessageInterface.requestDataStreamExtra1(1, 0);
            __vehicle->mavLinkMessageInterface.requestDataStreamExtra2(1, 0);
            __vehicle->mavLinkMessageInterface.requestDataStreamExtra3(1, 0);
        }

    }
}

//
//连接信号
//
void FrmMainController::__connectSignal(){

}


void FrmMainController::set_home(internals::PointLatLng pos,int height)
{
    if(NULL != __vehicle)
    {
     __vehicle->mavLinkMessageInterface.commandDoSetHome(0,1,pos.Lng(),pos.Lat(),height);
    }
}

void FrmMainController::startCompassMotCalibration()
{
    if(NULL != __vehicle)
    {
     __vehicle->mavLinkMessageInterface.startCompassMotCalibration();
    }
}
void FrmMainController::cancelCompassMotCalibration()
{
    if(NULL != __vehicle)
    {

     __vehicle->mavLinkMessageInterface.paramRequestRead((char*)"COMPASS_MOT_X");
     __vehicle->mavLinkMessageInterface.paramRequestRead((char*)"COMPASS_MOT_Y");
     __vehicle->mavLinkMessageInterface.paramRequestRead((char*)"COMPASS_MOT_Z");

    }
}
void FrmMainController::SendCommandACK(uint16_t cmd, uint8_t res)
{
    mavlink_command_ack_t commandAck;
    commandAck.command=cmd;
    commandAck.result=res;
    mavlink_message_t msg;
    mavlink_msg_command_ack_encode(1,1,&msg,&commandAck);

    if(NULL != __vehicle)
    {
     __vehicle->mavLinkMessageInterface.sendMessage(msg);
    }
}
void FrmMainController::handleRCCalibration(mavlink_message_t message)
{
        mavlink_rc_channels_raw_t channels;
        mavlink_msg_rc_channels_raw_decode(&message, &channels);

        const unsigned int portWidth = 8; // XXX magic number
        //qDebug() << "--------------------------channels.chan1_raw:" << channels.chan1_raw;
        //emit remoteControlRSSIChanged(channels.rssi/255.0f);
        if(channels.chan1_raw != UINT16_MAX)
        {
            emit remoteControlChannelRawChanged(0, (uint16_t)channels.chan1_raw);
        }
        if(channels.chan2_raw != UINT16_MAX)
        {
            emit remoteControlChannelRawChanged(1, (uint16_t)channels.chan2_raw);
        }
        if (channels.chan3_raw != UINT16_MAX)
            emit remoteControlChannelRawChanged(2, (uint16_t)channels.chan3_raw);
        if (channels.chan4_raw != UINT16_MAX)
            emit remoteControlChannelRawChanged(3, (uint16_t)channels.chan4_raw);
        if (channels.chan5_raw != UINT16_MAX)
            emit remoteControlChannelRawChanged(4, (uint16_t)channels.chan5_raw);
        if (channels.chan6_raw != UINT16_MAX)
            emit remoteControlChannelRawChanged(5, (uint16_t)channels.chan6_raw);
        if (channels.chan7_raw != UINT16_MAX)
            emit remoteControlChannelRawChanged(6, (uint16_t)channels.chan7_raw);
        if (channels.chan8_raw != UINT16_MAX)
            emit remoteControlChannelRawChanged(7, (uint16_t)channels.chan8_raw);

}



void FrmMainController::EnableBatteryMot(bool enable)
{
    if(m_paramcontroller!=NULL)
    {
        if(SettingManager::Instance()->_ParamExist((char*)"BATT_MONITOR"))
        {
          if(enable)
            m_paramcontroller->set_parameter_noanswer((char*)"BATT_MONITOR",4,MAV_PARAM_TYPE_INT32);
            else
            {
                 m_paramcontroller->set_parameter_noanswer((char*)"BATT_VOLT_PIN",-1,MAV_PARAM_TYPE_INT32);
                 QThread::msleep(100);
                 m_paramcontroller->set_parameter_noanswer((char*)"BATT_CURR_PIN",-1,MAV_PARAM_TYPE_INT32);
                 QThread::msleep(100);
                 m_paramcontroller->set_parameter_noanswer((char*)"BATT_MONITOR",0,MAV_PARAM_TYPE_INT32);
                 QThread::msleep(100);
            }
        }
    }
}

void FrmMainController::DownloadLog_slot(){
    if(NULL != __vehicle){
        __vehicle->mavLinkMessageInterface.logRequestList();
    }else{
       QMessageBox::warning(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("没有已连接的飞机"), QMessageBox::Yes);
    }
}

void FrmMainController::requestLogData_slot(uint16_t id,uint32_t ofs,uint32_t count){
    if(NULL != __vehicle){
        __vehicle->mavLinkMessageInterface.logRequestData(id, ofs, count);
    }
}

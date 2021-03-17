#include "frmmain.h"
#include "ui_frmmain.h"
#include "controls/iconhelper.h"
#include "controls/myhelper.h"
#include <QHBoxLayout>
#include "controls/picturebox.h"
#include "qfile.h"
#include "controls/MapWidgetContainer.h"
#include "controls/MapWidgetEditor.h"
#include "controls/dataparameterview.h"
#include <qtableview.h>
#include "qlist.h"
#include "controls/frmmessagebox.h"
#include "connections/datastruct.h"
#include "controls/parametertableview.h"
#include "internals/pointlatlng.h"
#include <QVector>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_legend.h>
#include <qwt_point_data.h>
#include <opmapcustom/opmapgadgetwidget.h>
#include <QString>
//#include <common/mavlink.h>
#include "connections/CommonStru.h"
#include <QSerialPortInfo>
#include <QDebug>
#include "controllers/frmmaincontroller.h"
#include <QDockWidget>
#include <QFrame>
#include "controls/cmdwindow.h"
#include <QMenu>
#include <QMenuBar>
#include <QCursor>
#include "controllers/curvecontroller.h"
#include "controls/Sensor/databoxresolver.h"
#include "controls/Sensor/sensordatamodel.h"
#include "controls/Sensor/sensortableview.h"
#include "Utils/serialportfinder.h"
#include "loghelper/outputlogger.h"
#include "ui_dataparameterview.h"
#include <QSplitter>
#include "controls/calibration/AccelCalibrationConfig.h"
//#include "controls/calibration/CompassConfig.h"
#include "controls/calibration/RadioCalibrationConfig.h"
#include "controls/calibration/calibrateform.h"
#include "controls/statuscontrol/signalcontrol.h"
#include "controls/statuscontrol/satellitecontrol.h"
#include "controls/statuscontrol/batterycontrol.h"
#include <QThread>
#include "Utils/zygroundglobalconfig.h"
#include <QTabWidget>
#include "controls/statuscontrol/refreshcontrol.h"
#include "controls/statuscontrol/warningform.h"
#include "controls/testform.h"
#include "controls/firmwareform.h"
#include "controls/InfoPanel/infomonitor.h"

bool isCommConnected = false;//串口是否连接
struct EnKeyItem
{
    bool   bUseKey;
    qint64 iLastTime;
    qint64 iEndTime;
};
frmMain::frmMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmMain)
{
    hOnlyOnceMutex = CreateMutex(NULL,FALSE,L"zyGround");

    if(ERROR_ALREADY_EXISTS == GetLastError())
    {
        myHelper::ShowMessageBoxInfo("程序已经启动");
        exit(0);
    }
    //许可文件判断
    QFile fs("./data/Nor.sr");
    bool bKeyOk=false;
    qint64 iCurTime=QDate::currentDate().toJulianDay();
    if(fs.open(QFile::ReadWrite))
    {
        EnKeyItem keyItem;
        int sz=sizeof(keyItem);
        if(sz==fs.read((char*)&keyItem,sz))
        {
            char* pKey=(char*)&keyItem;
            for(int i=0;i<sz;i++)  //解密
            {
                *(pKey+i)+=5;
                *(pKey+i)^=12138;
            }
            if(!keyItem.bUseKey)   //许可没有启用
                bKeyOk=true;
            else
            {
                //许可要大于当前时间，并在许可范围之内，把当前时间写入许可文件
               if(iCurTime>=keyItem.iLastTime && iCurTime<=keyItem.iEndTime)
               {
                   bKeyOk=true;
                   keyItem.iLastTime=iCurTime;
                   for(int i=0;i<sz;i++)
                   {
                       *(pKey+i)^=12138;
                       *(pKey+i)-=5;
                   }
                   fs.write((char*)&keyItem,sz);  //更新许可文件
               }
            }
        }
        fs.close();
    }
    if(!bKeyOk)
    {
        myHelper::ShowMessageBoxInfo("软件已过期");
        exit(0);
    }

    ui->setupUi(this);

    m_WinRect=myHelper::GetWinRect(this);
    setGeometry(m_WinRect);

    m_flightdisplay=NULL;
    m_datapview=NULL;
    m_paramView=NULL;
    m_paramRightControl=NULL;
    m_calibrateForm=NULL;
    m_FrmMainController=NULL;
    m_mapcontrol=NULL;
    m_mapEditor=NULL;
    m_curveController=NULL;
    m_serialFinder=NULL;
    m_outputLoger=NULL;

    this->InitStyle();
    //加载配置文件 gyq add
    ZYGroundGlobalConfig::LoadConfig();

   // myHelper::FormInCenter(this);

    //this->InitFlightMode();

    //this->InitRCType();

    this->LoadLayout();
    //this->PlanLoadLayout();

    InitialStatusControl();

    this->ConfigLoadLayout();

    m_FrmMainController=new FrmMainController();
    m_FrmMainController->SetControl(this);

    initPortInfo();
    showFlyControl();

    index_saveTag=0;

    m_calibrateForm=NULL;
    m_videoPlayer=NULL;

    if(g_bWriteLog)
    {
        QDir dir;
        dir.setPath("./data/runninglog");
        if(!dir.exists())
        {
            dir.mkdir("./data/runninglog");
        }

        QDateTime dt;
        dt=QDateTime::currentDateTime();
        QString t_timestr = dt.toString("yy-MM-dd hh.mm.ss");


        g_fRecv.setFileName("./data/runninglog/"+t_timestr+".rawlog");

        g_fRecv.open(QIODevice::WriteOnly);


        g_recvDataStream.setDevice(&g_fRecv);
        g_recvDataStream.writeRawData("zyfklog",sizeof("zyfklog"));
        g_fRecv.flush();

        g_fSend.setFileName("./data/runninglog/"+t_timestr+"send_raw_log.txt");
        g_fSend.open(QIODevice::WriteOnly);
        g_sendDatastream.setDevice(&g_fSend);
        g_sendDatastream.writeRawData("zyfklog",sizeof("zyfklog"));
        g_fSend.flush();
    }

    g_bSendFlushed=false;
    g_bRecvFlushed=false;

    g_FlushFileTimer=new QTimer(this);
    connect(g_FlushFileTimer,SIGNAL(timeout()),this,SLOT(OnFlushFile()));

    LoadCalibrate();

    //设置标题和图标
    QString strTitle=g_strTitle+"V2.6.0.0";
    ui->lab_Title->setText(strTitle);  //标题
    setWindowTitle(strTitle);          //任务栏显示的标题
    QString str=QCoreApplication::applicationDirPath();
    str+=ZYGroundGlobalConfig::m_sIconName;
    QIcon ico(str);
    ui->lab_Ico->setPixmap(ico.pixmap(QSize(24,24)));//上边标题图标
    setWindowIcon(ico);                          //下边任务栏图标

#ifdef ZYDataAnalasis_ENABLED
    m_curveController=new CurveController();
    this->DataLoadLayout();
#else
    removeTab("tab_data");
#endif
#ifdef AIRSENSOR_ENABLED
    this->LoadSensorLayout();
#else
    removeTab("tab_airsensor");
#endif
#ifdef AERIALVideo_ENABLED
     LoadVideoLayout();
#else
    removeTab("videoContainer");
#endif
#ifdef ZYMAVLinkTest_ENABLED
     LoadMavTest();
#else
    removeTab("tab_mavtest");
#endif
    LoadFirmWare();
    if(false == g_bDevelopMode)
    {
        int num=ui->tabWidget_container->count();
        qDebug()<<"tab size="<<num;
        for(int i = num-1; i > 0; --i)
        {
            ui->tabWidget_container->removeTab(i);
        }

    }
    setTabEnabled(false);
    connect(m_datapview,SIGNAL(signal_setTabEnabled(bool)),this,SLOT(setTabEnabled(bool)));

}

frmMain::~frmMain()
{
   //资源、线程等释放
    QList<QObject*>::Iterator cur=	m_resourcelst.begin();
    for(;cur!=m_resourcelst.end();cur++)
    {
        delete (QObject*)*cur;
    }
    delete ui;
    //delete m_flightdisplay;
    //delete m_datapview;
    delete m_paramView;
    m_serialFinder->terminate();
    delete m_serialFinder;
    delete m_paramRightControl;
   // delete m_FrmMainController;
    delete m_mapcontrol;
    //delete m_mapEditor;
    delete m_curveController;
    delete m_outputLoger;

    if(g_bWriteLog)
    {
        g_fSend.close();
        g_fRecv.close();
    }
}

void frmMain::InitStyle()
{
    location = this->geometry();

    max = false;
    mousePressed = false;

    //安装事件监听器,让标题栏识别鼠标双击
    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068), 10);
//    IconHelper::Instance()->SetIcon(ui->btnMenu, QChar(0xf0c9), 10);
//    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf015), 12);

    setBackground(ui->btnMenu_Close,QColor(255,255,255));
    setBackground(ui->btnMenu_Max,QColor(255,255,255));
    setBackground(ui->btnMenu_Min,QColor(255,255,255));
//    setBackground(ui->lab_Ico,QColor(255,255,255));


}

void frmMain::initialMenu()
{
    QString menuBar_str="QMenuBar{background-color:transparent}";
    QString menu_str="QMenu {background-color: white; border: 1px solid white;}QMenu::item {background-color: transparent;padding:4px 32px;margin:0px 4px;border-bottom:1px solid #DBDBDB;}QMenu::item:selected {background-color: #2dabf9;}";
    ui->verticalLayout_2->setContentsMargins(0,0,0,0);
    ui->hl_toolbarcontainer->setContentsMargins(0,0,0,0);


    QMenuBar* bar=new QMenuBar();
    bar->setStyleSheet(menuBar_str);
    QFont fontme;
    fontme.setPointSize(10);
    bar->setFont(fontme);
    QMenu* m_menu_file=new QMenu(QString::fromLocal8Bit("文件(F)"));
    m_menu_file->setFont(fontme);
    m_menu_file->setStyleSheet(menu_str);
    m_menu_file->addAction(QString::fromLocal8Bit("打开航路文件"));
    m_menu_file->addAction(QString::fromLocal8Bit("保存航路文件"));
    m_menu_file->addAction(QString::fromLocal8Bit("退出"),qApp,SLOT(quit()));

    QMenu* m_menu_edit=new QMenu(QString::fromLocal8Bit("编辑(E)"));
    m_menu_edit->setFont(fontme);
    m_menu_edit->setStyleSheet(menu_str);
    m_menu_edit->addAction(QString::fromLocal8Bit("路线规划"));
    m_menu_edit->addAction(QString::fromLocal8Bit("参数设置"));

    QMenu* m_menu_cmd=new QMenu(QString::fromLocal8Bit("命令(C)"));
    m_menu_cmd->setFont(fontme);
    m_menu_cmd->setStyleSheet(menu_str);

    m_menu_cmd->addAction(QString::fromLocal8Bit("获取航路"),m_FrmMainController,SLOT(btn_send_getwp_cmd_click()));

    m_menu_cmd->addAction(QString::fromLocal8Bit("获取参数"),m_FrmMainController,SLOT(request_parameterList()));

    m_menu_cmd->addAction(QString::fromLocal8Bit("启动姿态监视"),m_FrmMainController,SLOT(enableAttitudeMonitor()));

    m_menu_cmd->addAction(QString::fromLocal8Bit("启动位置监视"),m_FrmMainController,SLOT(enableLocationMonitor()));

    m_menu_cmd->addAction(QString::fromLocal8Bit("起飞"),m_FrmMainController,SLOT(btn_send_takeoff_cmd_click()));

    m_menu_cmd->addAction(QString::fromLocal8Bit("降落"),m_FrmMainController,SLOT(btn_send_land_cmd_click()));


    QMenu* m_menu_view=new QMenu(QString::fromLocal8Bit("视图(V)"));
     m_menu_view->setFont(fontme);
    m_menu_view->setStyleSheet(menu_str);
    m_menu_view->addAction(QString::fromLocal8Bit("飞行控制"),this,SLOT(showFlyControl()));
    m_menu_view->addAction(QString::fromLocal8Bit("航路规划"),this,SLOT(showFlyPlan()));
    m_menu_view->addAction(QString::fromLocal8Bit("飞行数据"),this,SLOT(showDataCurve()));
    m_menu_view->addAction(QString::fromLocal8Bit("参数设置"),this,SLOT(showParam()));


    QMenu* m_menu_setting=new QMenu(QString::fromLocal8Bit("校准(A)"));
    m_menu_setting->setFont(fontme);
    m_menu_setting->setStyleSheet(menu_str);
    m_menu_setting->addAction(QString::fromLocal8Bit("加速度计校准"),this,SLOT(showAccel()));
    m_menu_setting->addAction(QString::fromLocal8Bit("Compass校准"),this,SLOT(showCompass()));
    m_menu_setting->addAction(QString::fromLocal8Bit("RC校准"),this,SLOT(showRCCalibration()));

    QMenu* m_menu_help=new QMenu(QString::fromLocal8Bit("帮助(H)"));
    m_menu_help->setFont(fontme);
    m_menu_help->setStyleSheet(menu_str);
    m_menu_help->addAction(QString::fromLocal8Bit("查看文档"));
    m_menu_help->addAction(QString::fromLocal8Bit("联系我们"));

    QMenu* m_menu_about=new QMenu(QString::fromLocal8Bit("关于(I)"));
    m_menu_about->setFont(fontme);
    m_menu_about->setStyleSheet(menu_str);
    m_menu_about->addAction(QString::fromLocal8Bit("关于我们"));
    m_menu_about->addAction(QString::fromLocal8Bit("版本信息"));

    bar->addMenu(m_menu_file);
    bar->addMenu(m_menu_edit);
    bar->addMenu(m_menu_cmd);
    bar->addMenu(m_menu_view);
    bar->addMenu(m_menu_setting);
    bar->addMenu(m_menu_help);
    bar->addMenu(m_menu_about);

    this->ui->hl_toolbarcontainer->insertWidget(0,bar);
}

void frmMain::InitialStatusControl()
{

    m_warningForm=new WarningForm(this);

    this->ui->hl_toolbarcontainer->insertWidget(2,m_warningForm,5);

    m_warningForm->hide();

    m_flymode_c=new SignalControl();
    m_flymode_c->setMode(QString::fromLocal8Bit("增稳"));
    m_isPositionMode = false;

    this->ui->hl_toolbarcontainer->insertWidget(0,m_flymode_c);

    m_flymode_c->hide();

    m_flymode_c->setMinimumWidth(90);

    m_lockedstate_c=new SignalControl();
    m_lockedstate_c->setLockState(true);

    this->ui->hl_toolbarcontainer->insertWidget(0,m_lockedstate_c);

    m_lockedstate_c->hide();

   m_signal_c=new SignalControl();
   m_signal_c->setSignalLevel(1);

   this->ui->hl_toolbarcontainer->insertWidget(0,m_signal_c);

   m_signal_c->hide();

   m_sate_fix_type_c=new SatelliteControl();
   m_sate_fix_type_c->setSatelliteState(3);
   //this->ui->hl_toolbarcontainer->insertWidget(0,m_sate_fix_type_c);

   m_sate_c=new SatelliteControl();
   m_sate_c->setSatelliteNum(16);
   //this->ui->hl_toolbarcontainer->insertWidget(0,m_sate_c);

   m_battery_c=new BatteryControl();
   m_battery_c->setValue(75);

   this->ui->hl_toolbarcontainer->insertWidget(0,m_battery_c);

   m_battery_c->hide();

   m_volt_c=new SignalControl();
   m_volt_c->setVolt(12.0);

   this->ui->hl_toolbarcontainer->insertWidget(0,m_volt_c);

   m_volt_c->hide();

}

void frmMain::initPortInfo()
{
    m_serialFinder=new SerialPortFinder();
    connect(m_serialFinder,SIGNAL(SerialPortChangedNotify(QList<QSerialPortInfo>)),m_datapview,SLOT(PortsFoundNotify(QList<QSerialPortInfo>)));
    m_serialFinder->start();
    //initPortBaudList(m_datapview->ui->cbx_ports_baudrate);
}

void frmMain::initPortBaudList(QComboBox* m_box)
{
    m_box->addItem("57600");
    m_box->addItem("115200");
    m_box->setCurrentIndex(1);
}

void frmMain::InitFlightMode()
{
#ifdef ZYFlightMode_ENABLED

   // ui->cbx_flightMode->addItem(QString::fromLocal8Bit("手动"),MAIN_STATE_MANUAL);
    //ui->cbx_flightMode->addItem(QString::fromLocal8Bit("定高"),MAIN_STATE_ALTCTL);
//    ui->cbx_flightMode->addItem(QString::fromLocal8Bit("定点"),MAIN_STATE_POSCTL);
//    ui->cbx_flightMode->addItem(QString::fromLocal8Bit("自动航路"),MAIN_STATE_AUTO_MISSION);
//    ui->cbx_flightMode->addItem(QString::fromLocal8Bit("自动盘旋"),MAIN_STATE_AUTO_LOITER);
//    ui->cbx_flightMode->addItem(QString::fromLocal8Bit("自动返航"),MAIN_STATE_AUTO_RTL);
////    ui->cbx_flightMode->addItem(QString::fromLocal8Bit("特技"),MAIN_STATE_ACRO);
////    ui->cbx_flightMode->addItem(QString::fromLocal8Bit("离线"),MAIN_STATE_OFFBOARD);
////    ui->cbx_flightMode->addItem(QString::fromLocal8Bit("混合定高"),MAIN_STATE_RATTITUDE);
//    ui->cbx_flightMode->addItem(QString::fromLocal8Bit("稳定"),MAIN_STATE_STAB);
//    ui->cbx_flightMode->addItem(QString::fromLocal8Bit("起飞"),MAIN_STATE_AUTO_TAKEOFF);
//    ui->cbx_flightMode->addItem(QString::fromLocal8Bit("降落"),MAIN_STATE_AUTO_LAND);

#else
    ui->cbx_flightMode->addItem(QString::fromLocal8Bit("增稳"),STABILIZE);
//    ui->cbx_flightMode->addItem(QString::fromLocal8Bit("特技"),ACRO);
    ui->cbx_flightMode->addItem(QString::fromLocal8Bit("定高"),ALT_HOLD);
    ui->cbx_flightMode->addItem(QString::fromLocal8Bit("自动"),AUTO);
//    ui->cbx_flightMode->addItem(QString::fromLocal8Bit("引导"),GUIDED);
    ui->cbx_flightMode->addItem(QString::fromLocal8Bit("悬停"),LOITER);
    ui->cbx_flightMode->addItem(QString::fromLocal8Bit("返航"),RTL);
//    ui->cbx_flightMode->addItem(QString::fromLocal8Bit("绕圈"),CIRCLE);
    ui->cbx_flightMode->addItem(QString::fromLocal8Bit("定点"),POSITION);
    ui->cbx_flightMode->addItem(QString::fromLocal8Bit("着陆"),LAND);
#endif
}

void frmMain::InitRCType(){
//    ui->cbx_rcType->addItem(QString::fromLocal8Bit("遥控器"), 1);
//    ui->cbx_rcType->addItem(QString::fromLocal8Bit("摇杆"), 2);
}


//void frmMain::LoadLayout()
//{
//    QHBoxLayout* flightdata_layout=new QHBoxLayout();
//    flightdata_layout->setDirection(QBoxLayout::LeftToRight);

//   /******************右边控件**************************/
//    QVBoxLayout* rgt_panel=new QVBoxLayout();
//    rgt_panel->setDirection(QBoxLayout::TopToBottom);

//    double ScaleFactor=1;

//    if(m_destopWidget.width()==1920)
//    {
//        ScaleFactor=1;
//        ZYGroundGlobalConfig::RESOLUTION_1920=true;
//    }
//    else if(m_destopWidget.width()<=1366)
//    {
//        ScaleFactor=0.8;
//        ZYGroundGlobalConfig::RESOLUTION_1920=false;
//    }
//    //statecontrol
//    m_flightdisplay = new PrimaryFlightDisplay(480*ScaleFactor,360*ScaleFactor,NULL);
//    //m_flightdisplay->setStyleSheet("QWidget{position:relative;width:100%;}");
//    m_flightdisplay->setMinimumWidth(480*ScaleFactor);
//    m_flightdisplay->setMinimumHeight(360*ScaleFactor);
//    m_flightdisplay->setMaximumWidth(480*ScaleFactor);
//    m_flightdisplay->setMaximumHeight(360*ScaleFactor);
//    m_flightdisplay->setStyleSheet("QWidget{border:2px groove gray;border-radius:10px;padding:2px 4px;}");
//    m_resourcelst.push_back(m_flightdisplay);//垃圾回收
//    rgt_panel->addWidget(m_flightdisplay);//姿态控件

//    m_datapview = new DataParameterView(this);
//    // m_datapview->setStyleSheet("QWidget{position:relative;width:100%;}");
//    m_resourcelst.push_back(m_datapview);//垃圾回收
//    rgt_panel->addWidget(m_datapview);//姿态控件


//   /******************左边控件**************************/
//    m_mapcontrol=new MapWidgetContainer(this);
//    m_mapcontrol->ShowUAV(true);
//    internals::PointLatLng point;
//    point.SetLat(39.9);
//    point.SetLng(116.456);
//    m_mapcontrol->UpdateUAVState(point,0.5,100);

//    flightdata_layout->addWidget(m_mapcontrol->GetWindow());
//    flightdata_layout->addLayout(rgt_panel);

//    ui->tab_flightdata->setLayout(flightdata_layout);

//    m_outputLoger=new OutPutLogger();
//    QTextBrowser* txt_Browser=m_outputLoger->getBrowser();
//    txt_Browser->setParent(m_datapview);

//    int LOG_TOP_MARGIN=0;
//    int LOG_WIDTH=0;
//    int LOG_HEIGHT=0;
//if(!ZYGroundGlobalConfig::RESOLUTION_1920)
//{
//     LOG_TOP_MARGIN=200;
//     LOG_WIDTH=383;
//     LOG_HEIGHT=113;
//}
//else
//{
//     LOG_TOP_MARGIN=210;
//     LOG_WIDTH=471;
//     LOG_HEIGHT=320;
//}

//    txt_Browser->setGeometry(QRect(0, LOG_TOP_MARGIN, LOG_WIDTH, LOG_HEIGHT));

//}
void frmMain::LoadLayout()
{
    QHBoxLayout* flightdata_layout=new QHBoxLayout();
    flightdata_layout->setDirection(QBoxLayout::LeftToRight);

    QHBoxLayout* left_top_layout=new QHBoxLayout();
    left_top_layout->setDirection(QBoxLayout::LeftToRight);

   /******************右边控件**************************/
    QVBoxLayout* rgt_panel=new QVBoxLayout();
    rgt_panel->setDirection(QBoxLayout::TopToBottom);

    double ScaleFactor=1;


    if(m_WinRect.width()==1920)
    {
        ScaleFactor=1;
        ZYGroundGlobalConfig::RESOLUTION_1920=true;
    }
    else if(m_WinRect.width()<=1367)
    {
        ScaleFactor=0.7;
        ZYGroundGlobalConfig::RESOLUTION_1920=false;
    }

    m_datapview = new DataParameterView(this);
    m_datapview->setFixedWidth(m_WinRect.width() - 2*(650*ScaleFactor) - 120);
    if(ZYGroundGlobalConfig::RESOLUTION_1920==false)
    {

        this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
        ui->lab_Ico->setVisible(true);
        ui->lab_Title->setVisible(true);

        ui->btnMenu_Close->setVisible(true);
        ui->btnMenu_Max->setVisible(true);
        ui->btnMenu_Min->setVisible(true);

        m_datapview->ui->btn_exit->setFixedSize(0,0);
    }
    else
    {
        this->setWindowFlags(Qt::FramelessWindowHint);
        ui->lab_Ico->setVisible(false);
        ui->lab_Title->setVisible(false);

        ui->btnMenu_Close->setVisible(false);
        ui->btnMenu_Max->setVisible(false);
        ui->btnMenu_Min->setVisible(false);
        ui->widget_title->setFixedSize(0,0);
    }
    m_resourcelst.push_back(m_datapview);//垃圾回收
    rgt_panel->addWidget(m_datapview);

   /******************左边控件**************************/
    m_mapcontrol=new MapWidgetContainer(this);
//    m_mapcontrol->ShowUAV(true);
//    internals::PointLatLng point;
//    point.SetLat(39.9);
//    point.SetLng(116.456);
//    m_mapcontrol->UpdateUAVState(point,0.5,100);
    m_mapcontrol->GetWindow()->setMinimumWidth(700*ScaleFactor);
    m_mapcontrol->GetWindow()->setMinimumHeight(450*ScaleFactor);
    m_mapcontrol->GetWindow()->setMaximumWidth(700*ScaleFactor);
    m_mapcontrol->GetWindow()->setMaximumHeight(450*ScaleFactor);

    //InfoMonitor* pMoniter=new InfoMonitor(this);
    //pMoniter->setFixedSize(600*ScaleFactor,400*ScaleFactor);

    m_flightdisplay = new PrimaryFlightDisplay(650*ScaleFactor,450*ScaleFactor,NULL);
    m_flightdisplay->setMinimumWidth(650*ScaleFactor);
    m_flightdisplay->setMinimumHeight(450*ScaleFactor);
    m_flightdisplay->setMaximumWidth(650*ScaleFactor);
    m_flightdisplay->setMaximumHeight(450*ScaleFactor);
    m_flightdisplay->setStyleSheet("QWidget{border:2px groove gray;border-radius:10px;padding:2px 4px;}");
    m_resourcelst.push_back(m_flightdisplay);//垃圾回收

    m_statusMonitor=new StatusMonitor();
    m_statusMonitor->setMinimumHeight(140);
    m_statusMonitor->setFixedSize(2*(650*ScaleFactor) + 60,m_WinRect.height()-450*ScaleFactor - 100);
    m_statusMonitor->setScale(ScaleFactor);
    QVBoxLayout* lft_panel=new QVBoxLayout();
    left_top_layout->addWidget(m_mapcontrol->GetWindow());
    left_top_layout->addWidget(m_flightdisplay);
    lft_panel->addLayout(left_top_layout);
    lft_panel->addWidget(m_statusMonitor);
    /**********************中间控件*****************************/
//    m_checkInform=new CheckInfoForm();
//    m_checkInform->setMinimumWidth(315);
//    QVBoxLayout* mid_panel=new QVBoxLayout();
//    mid_panel->setDirection(QBoxLayout::TopToBottom);
//    mid_panel->addWidget(m_checkInform);
    flightdata_layout->addLayout(lft_panel);
    flightdata_layout->addLayout(rgt_panel);

    ui->tab_flightdata->setLayout(flightdata_layout);
    //flightdata_layout->addLayout(mid_panel);

//    m_outputLoger=new OutPutLogger();
//    QTextBrowser* txt_Browser=m_outputLoger->getBrowser();
//    txt_Browser->setParent(m_datapview);

//    int LOG_TOP_MARGIN=0;
//    int LOG_WIDTH=0;
//    int LOG_HEIGHT=0;
//    if(!ZYGroundGlobalConfig::RESOLUTION_1920)
//    {
//         LOG_TOP_MARGIN=260;
//         LOG_WIDTH=383;
//         LOG_HEIGHT=53;
//    }
//    else
//    {
//         LOG_TOP_MARGIN=270;
//         LOG_WIDTH=471;
//         LOG_HEIGHT=260;
//    }

//        txt_Browser->setGeometry(QRect(0, LOG_TOP_MARGIN, LOG_WIDTH, LOG_HEIGHT));

}

/***加载任务计划页面***/
void frmMain::PlanLoadLayout()
{
    QSplitter* flightdata_splitter=new QSplitter(Qt::Vertical);
    m_mapEditor=new MapWidgetEditor(this);
    m_mapEditor->ShowUAV(true);
    connect(m_mapEditor, SIGNAL(signal_UpdateMapType(core::MapType::Types)), m_mapcontrol, SLOT(slot_updateMapType(core::MapType::Types)));
    m_newSC = new ShowCurrentWayPoints();
    connect(m_newSC, SIGNAL(signal_SCDeleteOneWP(int)), this, SLOT(deleteOneWP_slots(int)));
    connect(m_newSC, SIGNAL(signal_SCUpdateOneWP(const int& , const MapPointData&)), this, SLOT(updateOneWP_slots(const int&, const MapPointData&)));
    connect(m_newSC, SIGNAL(signal_SCUpMoveOneWp(int)), this, SLOT(upOneWayPointData_slots(int)));
    connect(m_newSC, SIGNAL(signal_SCDownMoveOneWp(int)), this, SLOT(downOneWayPointData_slots(int)));
    connect(m_mapEditor, SIGNAL(signal_mapEditor_UpdateMovableWP(const int&, const MapPointData&)), m_newSC, SLOT(showTable_updateMovableWP_slots(const int&,const MapPointData&)));
    connect(this, SIGNAL(signal_updateShowList(const QList<ProtocolDataType>&)), m_newSC, SLOT(showTable_updateShowList_slots(const QList<ProtocolDataType>&)));
    connect(m_newSC,SIGNAL(siganle_UpdateMapPointList(const QList<MapPointData>&)),m_mapcontrol->m_mapWidget,SLOT(showWP(const QList<MapPointData>&)));
    connect(m_newSC,SIGNAL(siganle_UpdateMapPointList(const QList<MapPointData>&)),m_mapEditor,SLOT(showWP(const QList<MapPointData>&)));
    m_resourcelst.push_back(m_mapEditor);//垃圾回收
//    flightdata_splitter->addWidget(m_mapEditor);
//    flightdata_splitter->addWidget(m_newSC);
    m_mapEditor->AddTableView(m_newSC, flightdata_splitter);
//    m_resourcelst.push_back(flightdata_splitter);//垃圾回收
    QIcon* qiFP = new QIcon(":/image/tabicon/mission.ico");
    ui->tabWidget_container->insertTab(1,m_mapEditor, *qiFP, QString::fromLocal8Bit("任务规划"));
}

void frmMain::deleteOneWP_slots(int index){
    m_mapEditor->mapWidgetEditor_deleteOneWP(index);
}

void frmMain::updateOneWP_slots(const int& row, const MapPointData& oneWP){
    m_mapEditor->mapWidgetEditor_updateOneWP(row, oneWP);
}

void frmMain::upOneWayPointData_slots(int row){
    m_mapEditor->mapWidgetEditor_upOneWP(row);
}

void frmMain::downOneWayPointData_slots(int row){
    m_mapEditor->mapWidgetEditor_downOneWP(row);
}

void frmMain::UpdateFlightMode(int mode)
{
//    if(4 == mode)//悬停状态下降落等按钮可用
//    {
        this->m_datapview->btn_land->setEnabled(true);
        this->m_datapview->btn_changeAlt->setEnabled(true);
        this->m_datapview->btn_changeYaw->setEnabled(true);
        this->m_datapview->btn_follow->setEnabled(true);
        this->m_datapview->btn_electirc->setEnabled(true);
        this->m_datapview->btn_electirc_close->setEnabled(true);


        this->m_datapview->btn_land->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
                                                                  font: 14pt \"黑体\";\
                                                                  border-radius:8px;\
                                                                  background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
                                                                  stop:0 rgba(225, 225, 225, 255),\
                                                                  stop:1 rgba(185, 185, 185, 255));"));
        this->m_datapview->btn_changeAlt->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
                                                                       font: 14pt \"黑体\";\
                                                                       border-top-left-radius:8px;\
                                                                       border-bottom-left-radius:8px;\
                                                                       background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
                                                                       stop:0 rgba(225, 225, 225, 255),\
                                                                       stop:1 rgba(185, 185, 185, 255));"));
        this->m_datapview->btn_changeYaw->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
                                                                       font: 14pt \"黑体\";\
                                                                       border-top-left-radius:8px;\
                                                                       border-bottom-left-radius:8px;\
                                                                       background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
                                                                       stop:0 rgba(225, 225, 225, 255),\
                                                                       stop:1 rgba(185, 185, 185, 255));"));
        this->m_datapview->btn_follow->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
                                                                    font: 14pt \"黑体\";\
                                                                    border-radius:8px;\
                                                                    background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
                                                                    stop:0 rgba(225, 225, 225, 255),\
                                                                    stop:1 rgba(185, 185, 185, 255));"));
        this->m_datapview->btn_electirc->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
                                                                     font: 14pt \"黑体\";\
                                                                     border-radius:8px;\
                                                                     background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
                                                                     stop:0 rgba(225, 225, 225, 255),\
                                                                     stop:1 rgba(185, 185, 185, 255));"));
        this->m_datapview->btn_electirc_close->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
                                                                     font: 14pt \"黑体\";\
                                                                     border-radius:8px;\
                                                                     background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
                                                                     stop:0 rgba(225, 225, 225, 255),\
                                                                     stop:1 rgba(185, 185, 185, 255));"));
//    }
//    else
//    {

//        this->m_datapview->btn_land->setEnabled(false);
//        this->m_datapview->btn_changeAlt->setEnabled(false);
//        this->m_datapview->btn_changeYaw->setEnabled(false);
//        this->m_datapview->btn_follow->setEnabled(false);
//        this->m_datapview->btn_land->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
//                                                                  font: 14pt \"黑体\";\
//                                                                  border-radius:8px;\
//                                                                  background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
//                                                                  stop:0 rgba(185, 185, 185, 255),\
//                                                                  stop:1 rgba(135, 135, 135, 255))"));
//        this->m_datapview->btn_changeAlt->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
//                                                                       font: 14pt \"黑体\";\
//                                                                       border-top-left-radius:8px;\
//                                                                       border-bottom-left-radius:8px;\
//                                                                       background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
//                                                                       stop:0 rgba(185, 185, 185, 255),\
//                                                                       stop:1 rgba(135, 135, 135, 255))"));
//        this->m_datapview->btn_changeYaw->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
//                                                                       font: 14pt \"黑体\";\
//                                                                       border-top-left-radius:8px;\
//                                                                       border-bottom-left-radius:8px;\
//                                                                       background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
//                                                                       stop:0 rgba(185, 185, 185, 255),\
//                                                                       stop:1 rgba(135, 135, 135, 255))"));
//        this->m_datapview->btn_follow->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
//                                                                    font: 14pt \"黑体\";\
//                                                                    border-radius:8px;\
//                                                                    background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
//                                                                    stop:0 rgba(185, 185, 185, 255),\
//                                                                    stop:1 rgba(135, 135, 135, 255))"));
//      }
#ifdef ZYFlightMode_ENABLED
    switch (mode) {
    case 0:
        this->m_statusMonitor->setFlightMode(QString::fromLocal8Bit("手动"));
        break;
    case 1:
        this->m_statusMonitor->setFlightMode(QString::fromLocal8Bit("定高"));
        break;
    case 2:
        this->m_statusMonitor->setFlightMode(QString::fromLocal8Bit("定点"));
        m_isPositionMode = true;
        break;
    case 3:
        this->m_statusMonitor->setFlightMode(QString::fromLocal8Bit("自动航路"));
        break;
    case 4:
        this->m_statusMonitor->setFlightMode(QString::fromLocal8Bit("悬停"));
        break;
    case 5:
        this->m_statusMonitor->setFlightMode(QString::fromLocal8Bit("自动返航"));
        break;
    case 6:
        this->m_statusMonitor->setFlightMode(QString::fromLocal8Bit("特技"));
        break;
    case 7:
        this->m_statusMonitor->setFlightMode(QString::fromLocal8Bit("离线"));
        break;
    case 8:
        this->m_statusMonitor->setFlightMode(QString::fromLocal8Bit("自稳"));
        break;
    case 9:
        this->m_statusMonitor->setFlightMode(QString::fromLocal8Bit("混合定高"));
        break;
    case 10:
        this->m_statusMonitor->setFlightMode(QString::fromLocal8Bit("起飞"));
        break;
    case 11:
        this->m_statusMonitor->setFlightMode(QString::fromLocal8Bit("降落"));
        break;
    case 12:
        this->m_statusMonitor->setFlightMode(QString::fromLocal8Bit("跟随"));
        break;
    default:
        this->m_statusMonitor->setFlightMode(QString::fromLocal8Bit("unkown"));
        break;
    }
#else
    switch (mode) {
    case 0:
        this->m_statusMonitor->setMode(QString::fromLocal8Bit("增稳"));
        break;
    case 1:
        this->m_statusMonitor->setMode(QString::fromLocal8Bit("特技"));
        break;
    case 2:
        this->m_statusMonitor->setMode(QString::fromLocal8Bit("定高"));
        break;
    case 3:
        this->m_statusMonitor->setMode(QString::fromLocal8Bit("自动"));
        break;
    case 4:
        this->m_statusMonitor->setMode(QString::fromLocal8Bit("引导"));
        break;
    case 5:
        this->m_statusMonitor->setMode(QString::fromLocal8Bit("悬停"));
        break;
    case 6:
        this->m_statusMonitor->setMode(QString::fromLocal8Bit("返航"));
        break;
    case 7:
        this->m_statusMonitor->setMode(QString::fromLocal8Bit("绕圈"));
        break;
    case 8:
        this->m_statusMonitor->setMode(QString::fromLocal8Bit("定点"));
        break;
    case 9:
        this->m_statusMonitor->setMode(QString::fromLocal8Bit("着陆"));
        break;
    case 10:
        this->m_statusMonitor->setMode(QString::fromLocal8Bit("OF_Loiter"));
        break;
    case 11:
        this->m_statusMonitor->setMode(QString::fromLocal8Bit("ToyA"));
        break;
    case 12:
        this->m_statusMonitor->setMode(QString::fromLocal8Bit("ToyM"));
        break;
    case 13:
        this->m_statusMonitor->setMode(QString::fromLocal8Bit("Sport"));
        break;
    default:
        this->m_statusMonitor->setMode(QString::fromLocal8Bit("unkown"));
        break;
    }
#endif
}

//加载数据分析-曲线页面
void frmMain::DataLoadLayout()
{
    QWidget*  widget_plot=m_curveController->getPlot();
    QVBoxLayout* data_layout=new QVBoxLayout();
    data_layout->setDirection(QBoxLayout::TopToBottom);
    data_layout->addWidget(widget_plot);
    ui->tab_data->setLayout(data_layout);
}

void frmMain::ConfigLoadLayout()
{
   m_refresh_c=  new RefreshControl();

   this->ui->hl_toolbarcontainer->insertWidget(8,m_refresh_c);

   m_refresh_c->hide();

   QHBoxLayout* data_layout=new QHBoxLayout();
   data_layout->setDirection(QBoxLayout::LeftToRight);

    QList<ParamData>* tempdata= SettingManager::Instance()->LoadPartSettings("Data/DisplayList.xml","Data/parameters.xml");
    m_paramView=new ParameterTableView();
    m_paramView->setList(tempdata);
    m_paramView->reset();
    data_layout->addWidget(m_paramView);

    m_paramRightControl=new ParamRightControl();
    m_paramRightControl->setStyleSheet("QWidget{border:1px solid gray;}");
    data_layout->addWidget(m_paramRightControl);
#ifndef ZYFlightMode_ENABLED
    QTabWidget* tabControl=new QTabWidget();
    tabControl->setTabPosition(QTabWidget::West);

    m_settingControl=new CustomSettings();
    tabControl->addTab(m_settingControl,QString::fromLocal8Bit("优化调参"));
    QWidget* m_bashSettingControl=new QWidget();
    m_bashSettingControl->setLayout(data_layout);
    tabControl->addTab(m_bashSettingControl,QString::fromLocal8Bit("基本设置"));

    QHBoxLayout* temp_box=new QHBoxLayout();
    temp_box->addWidget(tabControl);
    ui->tab_config->setLayout(temp_box);

#else
    ui->tab_config->setLayout(data_layout);//植保
#endif
}

void frmMain::LoadSensorLayout()
{
    _resolver=new DataBoxResolver(this);

    cbx_sensor_ports = new QComboBox();
    cbx_sensor_ports->setStyleSheet("QComboBox{max-width:120px;height:35px}");

    cbx_sensor_ports_baudrate= new QComboBox();
    cbx_sensor_ports_baudrate->setStyleSheet("QComboBox{max-width:120px;margin-top:20px;height:35px}");

    btn_sensor_connect = new QPushButton();
    btn_sensor_connect->setStyleSheet("QPushButton{max-width:120px;margin-top:20px;height:35px}");
    btn_sensor_connect->setText(QString::fromLocal8Bit("Sensor连接"));
    connect(btn_sensor_connect,SIGNAL(clicked()),this,SLOT(sensor_connect_clicked()));


    connect(_resolver, SIGNAL(datacoming(AIRSENSORSTR&,UAVDATASTR&)), this, SLOT(data_recv(AIRSENSORSTR&,UAVDATASTR&)));

    _datalist=new QList<UAVDdataStruct>();
    _sensordatalist=new QList<AIRSENSORSTR>();

    _sensorView=new SensorTableView();

    QHBoxLayout* data_layout=new QHBoxLayout();
    data_layout->setDirection(QBoxLayout::LeftToRight);

    QWidget* m_widget=new QWidget();
    m_widget->setMinimumSize(140,300);
    QVBoxLayout* temp=new QVBoxLayout();
    temp->setDirection(QBoxLayout::TopToBottom);
    temp->addWidget(cbx_sensor_ports_baudrate);
    temp->addWidget(cbx_sensor_ports);
    temp->addWidget(btn_sensor_connect);

    QSpacerItem *verticalSpacer= new QSpacerItem(140, 400, QSizePolicy::Minimum, QSizePolicy::Expanding);

    temp->addItem(verticalSpacer);

    m_widget->setLayout(temp);

    data_layout->addWidget(_sensorView);
    data_layout->addWidget(m_widget);
    ui->tab_airsensor->setLayout(data_layout);
}


 void frmMain::LoadCalibrate()
 {
//   if(m_calibrateForm!=NULL)
//   {
//       delete m_calibrateForm;
//       m_calibrateForm=NULL;
//       this->ui->tab_calibrate->setLayout(NULL);
//   }
    QHBoxLayout* box=new QHBoxLayout();
    m_calibrateForm=new CalibrateForm();
    box->addWidget(m_calibrateForm);
    this->ui->tab_calibrate->setLayout(box);
 }



void frmMain::setBackground(QWidget* wid,QColor c)
{
    QPalette palette;
    palette.setColor(wid->backgroundRole(),c);
    wid->setPalette(palette);
}

void frmMain::setBackgroundImage(QWidget* control,QString path,int width,int height)
{
    QPalette palette;
    QBrush brush= QBrush(QImage(path).scaled(width,height));

    palette.setBrush(control->backgroundRole(),brush);
    control->setPalette(palette);
}

void frmMain::setOpacity(QWidget* control)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(0x00,0xff,0x00,0x00));
    control->setPalette(pal);
}

/**********传感器**************/
void frmMain::sensor_connect_clicked()
{
    _resolver->setPortName(this->cbx_sensor_ports->currentText(),this->cbx_sensor_ports_baudrate->currentText().toInt());

    if(_resolver!=NULL)
    {
        if(_resolver->isRunning())
           _resolver->exit();
        _resolver->start();
        btn_sensor_connect->setText(QString::fromLocal8Bit("Sensor已连接"));


//        for(int i=0;i<100000;i++)
//        {
//            AirSensorStruct air;
//            air.CO=19200;
//            air.humidity=11;
//            air.NO2=839;
//            air.O3=2;
//            air.SO2=38;
//            air.temperature=223;
//            air.pm10=1;
//            air.pm1_0=2;
//            air.pm2_5=3;
//            UAVDdataStruct uav;
//            uav.day=23;
//            uav.hight=1892;
//            uav.hour=12;
//            uav.latitude=39.993;
//            uav.longitude=116.399;
//            uav.min=1;
//            uav.mon=1;
//            uav.sec=56;
//            uav.year=2016;
//            data_recv(air,uav);
//        }

    }

}

void frmMain::data_recv(AIRSENSORSTR& airSensorStr, UAVDATASTR& uavDataStr)
{
    //update view to list;
      _datalist->append(uavDataStr);
      _sensordatalist->append(airSensorStr);

      //显示10行数据
      if(_datalist->count()>20)
      {
          _datalist->removeAt(0);
          _sensordatalist->removeAt(0);
          if(index_saveTag==20)
          {
              index_saveTag=0;
              _resolver->SaveFile(*_sensordatalist,*_datalist);
          }
          index_saveTag++;
      }
      _sensorView->_sensorModel->setData(*_datalist,*_sensordatalist);
      _sensorView->reset();
      _sensorView->_sensorModel->refresh();
}


//模式选择
void frmMain::on_cbx_flightMode_currentIndexChanged(int index)
{
   //int mode=  ui->cbx_flightMode->currentData().toInt();

//   FrmMainController::Instance()->setMode(mode);

}
void frmMain::showFlyControl()
{
  this->ui->tabWidget_container->setCurrentIndex(0);
}
void frmMain::showFlyPlan()
{
    this->ui->tabWidget_container->setCurrentIndex(1);
}
void frmMain::showDataCurve()
{
    this->ui->tabWidget_container->setCurrentIndex(2);
}
void frmMain::showParam()
{
    this->ui->tabWidget_container->setCurrentIndex(3);
}
//是否启用摇杆,1为启用，0为未启用
void frmMain::updateRCType(bool flag){

}

/********************************************/

bool frmMain::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void frmMain::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void frmMain::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void frmMain::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void frmMain::on_btnMenu_Close_clicked()
{
    qApp->exit();
}

void frmMain::on_btnMenu_Max_clicked()
{
    if (max) {
        this->setGeometry(location);
        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
        ui->btnMenu_Max->setToolTip("最大化");
    } else {
        location = this->geometry();
        this->setGeometry(qApp->desktop()->availableGeometry());
        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf079), 10);
        ui->btnMenu_Max->setToolTip("还原");
    }
    max = !max;
}

void frmMain::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}


void frmMain::LoadVideoLayout()
{
    QHBoxLayout* video_layout=new QHBoxLayout();
    video_layout->setDirection(QBoxLayout::LeftToRight);

//    m_videoPlayer=new CommonVideoPlayer(this);
//    m_videoPlayer->OpenUrl("udp://@:2000");

//    //m_videoPlayer->OpenUrl("dshow://:dshow-vdev=xVideo :dshow-adev=xAudio :dshow-size=1920x1080:dshow-caching=200 :dshow-vdev=xVideo :dshow-adev=xAudio :dshow-chroma= :dshow-fps=0 :no-dshow-config :no-dshow-tuner :dshow-tuner-channel=0 :dshow-tuner-country=0 :dshow-tuner-input=0 :dshow-video-input=-1 :dshow-video-output=-1 :dshow-audio-input=-1 :dshow-audio-output=-1 :dshow-amtuner-mode=1 :dshow-audio-channels=0 :dshow-audio-samplerate=0 :dshow-audio-bitspersample=0 :dshow-caching=200");
//    m_videoPlayer->resize(1200,800);
//    //m_videoPlayer->showMaximized();
//    video_layout->addWidget(m_videoPlayer);

//    this->ui->videoContainer->setLayout(video_layout);

     qDebug()<<"here load the VLC component on the first time.....";

     m_camera=new Camera(this);
     m_camera->show();
    // m_camera->resize(1200,800);
     video_layout->addWidget(m_camera);
     this->ui->videoContainer->setLayout(video_layout);

     qDebug()<<"here load the DShow component on the first time.....";
}

void frmMain::LoadMavTest()
{
    QHBoxLayout* lay=new QHBoxLayout();
    TestForm* testfrm=new TestForm(this);
    testfrm->show();
    lay->addWidget(testfrm);
    ui->tab_mavtest->setLayout(lay);
}

void frmMain::LoadFirmWare()
{
    QHBoxLayout* lay=new QHBoxLayout();
    FirmWareForm* firm=new FirmWareForm(this);
    firm->show();
    lay->addWidget(firm);
    ui->tab_firmware->setLayout(lay);
}
void  frmMain::paintEvent(QPaintEvent * event)
{

}

void frmMain::on_cbx_rcType_currentIndexChanged(int index)
{
    if(!m_isPositionMode){
        QMessageBox::information(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请切换到定点模式!"));
        //ui->cbx_rcType->setCurrentIndex(0);
        return;
    }
    //int rctype=  ui->cbx_rcType->currentData().toInt();
//    if(rctype == 2){
//        startJoyStick();
//    }else {
//        stopJoyStick();
    //}
}

void frmMain::OnFlushFile()
{
    g_bSendFlushed=true;
    g_bRecvFlushed=true;
    //qDebug()<<"Timer out Start flushed";
}

void frmMain::setTabEnabled(bool bIsTabEnabled)
{
//    QWidget* findItem;
//    int findIndex;

//    findItem= this->ui->tabWidget_container->findChild<QWidget*>(QString("tab_config"));
//    findIndex= this->ui->tabWidget_container->indexOf(findItem);

//    this->ui->tabWidget_container->setTabEnabled(findIndex,bIsTabEnabled);

//    findItem= this->ui->tabWidget_container->findChild<QWidget*>(QString("tab_calibrate"));
//    findIndex= this->ui->tabWidget_container->indexOf(findItem);

//    this->ui->tabWidget_container->setTabEnabled(findIndex,bIsTabEnabled);
    int count =ui->tabWidget_container->count();
    if(count<=1)
        return;
    for(int i=1;i<count;i++)
    {
        ui->tabWidget_container->setTabEnabled(i,bIsTabEnabled);
    }

}
void frmMain::removeTab(const QString &tabName)
{
    QWidget* findItem= this->ui->tabWidget_container->findChild<QWidget*>(tabName);
    int findIndex= this->ui->tabWidget_container->indexOf(findItem);
    this->ui->tabWidget_container->removeTab(findIndex);
}

void frmMain::keyReleaseEvent(QKeyEvent *event)
{
    Vehicle* pVeh=FrmMainController::Instance()->__vehicle;
    if(!pVeh)
        return;
    if (!event->isAutoRepeat() &&(event->modifiers() & Qt::AltModifier))
    {
        if (event->key() == Qt::Key_F5)
        {
            pVeh->mavLinkMessageInterface.doCommand((MAV_CMD)181,10,100,0,0,0,0,0);
            qDebug()<<"Send NJ+";
        }
        else if(event->key() == Qt::Key_F6)
        {
            pVeh->mavLinkMessageInterface.doCommand((MAV_CMD)181,10,0,0,0,0,0,0);
            qDebug()<<"Send NJ-";
        }
    }
}

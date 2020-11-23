#include "dataparameterview.h"
#include "Utils/zygroundglobalconfig.h"
#include "ui_dataparameterview.h"
#include <math.h>
#include <QSerialPortInfo>
#include "frmmessagebox.h"
#include "myhelper.h"
#include "Joystick/joysticktoflycontroller.h"
#include "joystick/zystick.h"
#define LAB_MH 25

DataParameterView::DataParameterView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataParameterView)
{
    ui->setupUi(this);
    label_tips = ui->label_tips;
    label_CompassState = ui->label_CompassState;
    label_Accelerometer = ui->label_Accelerometer;
    label_RemoteController = ui->label_RemoteController;
    label_Digital = ui->label_Digital;
    label_GPS = ui->label_GPS;
    label_RTK = ui->label_RTK;
    label_HeadingRTK = ui->label_HeadingRTK;
    label_RecData=ui->label_RecData;
    label_CalPosition=ui->label_CalPosition;

    btn_takeoffOnekey = ui->btn_takeoffOnekey;
    le_height = ui->le_height;
    btn_changeYaw = ui->btn_changeYaw;
    le_yaw = ui->le_yaw;
    btn_changeAlt = ui->btn_changeAlt;
    le_alt = ui->le_alt;

    btn_land = ui->btn_land;
    btn_follow = ui->btn_follow;
    btn_stick =ui->btn_stick;
    reSetupUi(this);
    m_current_alt=0;
    m_timer=new QTimer(this);
    m_timer->setInterval(3000);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(on_takeoff_after_unlocked()));
    m_takeoff_alt=0;

    btn_takeoffOnekey->setEnabled(false);//未打开串口之前默认按钮不可用
    btn_land->setEnabled(false);
    btn_changeAlt->setEnabled(false);
    btn_changeYaw->setEnabled(false);
    btn_follow->setEnabled(false);
    btn_stick->setEnabled(false);
	ui->btn_info->setEnabled(false);

    m_pJoystick=NULL;
    m_pZYStick =NULL;

    btn_takeoffOnekey->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
                                                    font: 14pt \"黑体\";\
                                                    border-top-left-radius:8px;\
                                                    border-bottom-left-radius:8px;\
                                                    background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
                                                    stop:0 rgba(185, 185, 185, 255),\
                                                    stop:1 rgba(135, 135, 135, 255));"));
    btn_land->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
                                           font: 14pt \"黑体\";\
                                           border-radius:8px;\
                                           background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
                                           stop:0 rgba(185, 185, 185, 255),\
                                           stop:1 rgba(135, 135, 135, 255));"));
    btn_changeAlt->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
                                                font: 14pt \"黑体\";\
                                                border-top-left-radius:8px;\
                                                border-bottom-left-radius:8px;\
                                                background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
                                                stop:0 rgba(185, 185, 185, 255),\
                                                stop:1 rgba(135, 135, 135, 255));"));
    btn_changeYaw->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
                                                font: 14pt \"黑体\";\
                                                border-top-left-radius:8px;\
                                                border-bottom-left-radius:8px;\
                                                background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
                                                stop:0 rgba(185, 185, 185, 255),\
                                                stop:1 rgba(135, 135, 135, 255));"));
    btn_follow->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
                                             font: 14pt \"黑体\";\
                                             border-radius:8px;\
                                             background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
                                             stop:0 rgba(185, 185, 185, 255),\
                                             stop:1 rgba(135, 135, 135, 255));"));
    btn_stick->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
                                             font: 14pt \"黑体\";\
                                             border-radius:8px;\
                                             background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
                                             stop:0 rgba(185, 185, 185, 255),\
                                             stop:1 rgba(135, 135, 135, 255));"));
	ui->btn_info->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
												font: 14pt \"黑体\";\
												border-radius:8px;\
												background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
												stop:0 rgba(185, 185, 185, 255),\
												stop:1 rgba(135, 135, 135, 255));"));
    IsConnected=false;

    le_alt->setValidator(new QDoubleValidator(5.0, 300.0,2, this));
    le_height->setValidator(new QDoubleValidator(5.0, 300.0,2, this));
    le_yaw->setValidator(new QDoubleValidator(0.0, 360.0,2, this));

    this->ui->label_2->setImage(":/image/control/control_06.png");
    this->ui->label_2->setFontSize(14);
    this->ui->label_2->setLabelText(QStringLiteral("米"));

    this->ui->label_3->setImage(":/image/control/control_06.png");
    this->ui->label_3->setFontSize(14);
    this->ui->label_3->setLabelText(QStringLiteral("度"));

    this->ui->label_4->setImage(":/image/control/control_06.png");
    this->ui->label_4->setFontSize(14);
    this->ui->label_4->setLabelText(QStringLiteral("米"));
}

DataParameterView::~DataParameterView()
{
    if(btn_stick->text()==QStringLiteral("关闭摇杆"))
        on_btn_stick_clicked();
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
}

void DataParameterView::update(VehicleState& data)
{
//     m_InfoPanel->update(data);
//     m_current_alt=data.height;
}

void DataParameterView::on_takeoff_after_unlocked()
{
    m_timer->stop();
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.commandTakeOff(1,NAN,m_takeoff_alt);
}


void DataParameterView::reSetupUi(QWidget *DataParameterView)
{
    int TopMargin=0;
    int RES_WIDTH=383;

    int BUTTON_HEIGHT=40;
    int BUTTON_TOP_MARGIN=155;
    int BUTTON_WIDTH=383;

    int spinBox_width=81;
    int spinBox_height=40;
    int changeBtn_width=105;
    int changeBtn_height=41;
    int changeAlt_btn_left=120;
    int changeYaw_btn_left=330;

    if(!ZYGroundGlobalConfig::RESOLUTION_1920)
    {
         TopMargin=5;
         RES_WIDTH=400;

         BUTTON_HEIGHT=40;
         BUTTON_TOP_MARGIN=155;
         BUTTON_WIDTH=383;

         spinBox_width=81*((double)1366.0/1920.0);
         spinBox_height=40*((double)768.0/1080.0);
         changeBtn_width=105*((double)1366.0/1920.0);
         changeBtn_height=41*((double)768.0/1080.0);
         changeAlt_btn_left=120*((double)1366.0/1920.0)-15;

         label_tips->setStyleSheet(QStringLiteral("font: 10pt \"黑体\";\
                 color: rgb(233, 233, 233);\
                 background-color: rgb(125, 125, 125);"));

         label_CompassState->setText(GetLabHtmlStr("CompassState.png",QStringLiteral("罗盘状态")));
         label_CompassState->setStyleSheet(QStringLiteral("font: 12pt \"黑体\";\
                                                          color: rgb(233, 233, 233);\
                                                          background-color: rgb(125, 125, 125);"));
         label_Accelerometer->setStyleSheet(QStringLiteral("font: 12pt \"黑体\";\
                                                           color: rgb(233, 233, 233);\
                                                           background-color: rgb(125, 125, 125);"));
         label_Accelerometer->setText(GetLabHtmlStr("Accelerometer.png",QStringLiteral("加速度计")));
         label_RemoteController->setStyleSheet(QStringLiteral("font: 12pt \"黑体\";\
                                                              color: rgb(233, 233, 233);\
                                                              background-color: rgb(125, 125, 125);"));
         label_RemoteController->setText(GetLabHtmlStr("RemoteController.png",QStringLiteral("遥控器")));
         label_Digital->setStyleSheet(QStringLiteral("font: 12pt \"黑体\";\
                                                     color: rgb(233, 233, 233);\
                                                     background-color: rgb(125, 125, 125);"));
         label_Digital->setText(GetLabHtmlStr("DigitalState.png",QStringLiteral("数传")));
         label_GPS->setStyleSheet(QStringLiteral("font: 12pt \"黑体\";\
                                                 color: rgb(233, 233, 233);\
                                                 background-color: rgb(125, 125, 125);"));
         label_GPS->setText(GetLabHtmlStr("GPSState.png",QStringLiteral("GPS")));
         label_RTK->setStyleSheet(QStringLiteral("font: 12pt \"黑体\";\
                                                 color: rgb(233, 233, 233);\
                                                 background-color: rgb(125, 125, 125);"));
         label_RTK->setText(GetLabHtmlStr("RTKState.png",QStringLiteral("RTK")));
         label_HeadingRTK->setStyleSheet(QStringLiteral("font: 12pt \"黑体\";\
                                                        color: rgb(233, 233, 233);\
                                                        background-color: rgb(125, 125, 125);"));
         label_HeadingRTK->setText(GetLabHtmlStr("HeadingRTK.png",QStringLiteral("航向RTK")));

         label_RecData->setStyleSheet(QStringLiteral("font: 12pt \"黑体\";\
                                                     color: rgb(233, 233, 233);\
                                                     background-color: rgb(125, 125, 125);"));
         label_RecData->setText(GetLabHtmlStr("FileRec.png",QStringLiteral("数据记录")));

         label_CalPosition->setStyleSheet(QStringLiteral("font: 12pt \"黑体\";\
                                                     color: rgb(233, 233, 233);\
                                                     background-color: rgb(125, 125, 125);"));
         label_CalPosition->setText(GetLabHtmlStr("PosCal.png",QStringLiteral("位置解算")));

    }
    else
    {
         TopMargin=10;
         RES_WIDTH=471;
         BUTTON_HEIGHT=50;
         BUTTON_WIDTH=471;
         BUTTON_TOP_MARGIN=155;

         spinBox_width=81;
         spinBox_height=40;
         changeBtn_width=105;
         changeBtn_height=41;
         changeAlt_btn_left=120;
         changeYaw_btn_left=330;
    }
    if (DataParameterView->objectName().isEmpty())
        DataParameterView->setObjectName(QStringLiteral("DataParameterView"));
    DataParameterView->resize(532, 893);
    disconnect(btn_takeoffOnekey, SIGNAL(clicked()), this, SLOT(on_btn_takeoffOnekey_clicked()));//event connect land
    connect(btn_takeoffOnekey, SIGNAL(clicked()), this, SLOT(on_btn_takeoffOnekey_clicked()));//event connect onekey take off
    disconnect(btn_land, SIGNAL(clicked()), this, SLOT(on_btn_land_clicked()));//event connect land
    connect(btn_land, SIGNAL(clicked()), this, SLOT(on_btn_land_clicked()));//event connect land
    disconnect(btn_changeAlt, SIGNAL(clicked()), this, SLOT(on_btn_changeAlt_clicked()));
    connect(btn_changeAlt, SIGNAL(clicked()), this, SLOT(on_btn_changeAlt_clicked())); //event change alt
    disconnect(btn_changeYaw, SIGNAL(clicked()), this, SLOT(on_btn_changeYaw_clicked()));
    connect(btn_changeYaw, SIGNAL(clicked()), this, SLOT(on_btn_changeYaw_clicked())); //event change alt
    disconnect(btn_follow, SIGNAL(clicked()), this, SLOT(on_btn_follow_clicked()));
    connect(btn_follow, SIGNAL(clicked()), this, SLOT(on_btn_follow_clicked())); //event change alt
    disconnect(btn_stick, SIGNAL(clicked()), this, SLOT(on_btn_stick_clicked()));
    connect(btn_stick, SIGNAL(clicked()), this, SLOT(on_btn_stick_clicked())); //event change alt

} // setupUi


QString DataParameterView::GetLabHtmlStr(const QString &pig, const QString &name)
{
//    QString str= QStringLiteral("<html><head/><body><p><img height=\"25\" src=\":/image/icon/")+pig;
//    if(ZYGroundGlobalConfig::RESOLUTION_1920)
//        str += QStringLiteral("\"/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
//    else
//        str += QStringLiteral("\"/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
//    return  str+name+QStringLiteral("</p></body></html>");
    return name;
}
  void DataParameterView::valueChanged(int value)
  {
//     lineEdit->setText(QString::number(value));
  }
  void DataParameterView::textChanged(QString value)
  {
    int alt=value.toInt();
    if(alt>50)
        alt=50;
    if(alt<0)
        alt=0;
//    if(horizontalSlider->value()!=alt)
//        horizontalSlider->setValue(alt);
  }
  void DataParameterView::OneKeyTakeOFF(float alt)
  {
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.commandComponentArmDisarm(0,1);
    m_timer->start();

    //FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.commandTakeOff(1,NAN,alt);
  }
  void DataParameterView::OneKeyLand()
  {
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.setMode(MAV_MODE_FLAG_CUSTOM_MODE_ENABLED,MAIN_STATE_AUTO_LAND);
  }
  void DataParameterView::ChangeAlt(float alt)
  {
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.commandTakeOff(1,NAN,alt);
    //FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.commandLoiter(1,NAN,alt);
  }
  void DataParameterView::ChangeYAW(float yaw)
  {
    float radius_yaw=yaw*M_PI/180.0;
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.commandTakeOff(1,radius_yaw,NAN);
    //FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.commandLoiter(1,radius_yaw,NAN);
  }

  void DataParameterView::on_btn_takeoffOnekey_clicked()
  {
    if(myHelper::ShowMessageBoxQuesion("确定要起飞么?")!=1)
    {
        return;
    }
    QString altStr = le_height->text();
    if(0 == altStr.length())
    {
        myHelper::ShowMessageBoxError("起飞高度不能为空");
        return;
    }
    float height=  le_height->text().toFloat();

    if(height < 5.0 || height > 300.0)
    {
        myHelper::ShowMessageBoxError("飞机高度请设置在5-300米之间");
        return;
    }
    m_takeoff_alt=height;
    OneKeyTakeOFF(height);
//    m_InfoPanel->resetXY();
  }

  void DataParameterView::on_btn_land_clicked()
  {
      if(myHelper::ShowMessageBoxQuesion("确定要降落么?")!=1)
      {
          return;
      }
      OneKeyLand();
  }

  void DataParameterView::on_btn_changeAlt_clicked()
  {
      if(myHelper::ShowMessageBoxQuesion("确定要改变高度么?")!=1)
      {
          return;
      }
      QString altStr = le_alt->text();
      if(0 == altStr.length())
      {
          myHelper::ShowMessageBoxError("改变高度数值不能为空");
          return;
      }

      float height=  le_alt->text().toFloat();

      if(height < 5.0 || height > 300.0)
      {
          myHelper::ShowMessageBoxError("飞机高度请设置在5-300米之间");
          return;
      }
      //OneKeyTakeOFF(height);
      ChangeAlt(height);
  }

  void DataParameterView::on_btn_changeYaw_clicked()
  {
       if(myHelper::ShowMessageBoxQuesion("确定要改变航向么?")!=1)
       {
           return;
       }
       QString altStr = le_yaw->text();
       if(0 == altStr.length())
       {
           myHelper::ShowMessageBoxError("改变航向角度不能为空");
           return;
       }
        float yaw=  le_yaw->text().toFloat();

        if(yaw < 0.0 || yaw > 360.0)
        {
            myHelper::ShowMessageBoxError("改变航向角度请设置在0-360度之间");
            return;
        }
        ChangeYAW(yaw);
  }

  void DataParameterView::on_btn_follow_clicked()
  {
      if(myHelper::ShowMessageBoxQuesion("确定要进入跟随模式么?")!=1)
      {
          return;
      }
      FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.commandDoSetMode(0,129,4,8);
  }

  void DataParameterView::PortsFoundNotify(QList<QSerialPortInfo> portlist)
  {
      ui->cbx_ports->clear();
      foreach (const QSerialPortInfo &info, portlist)
      {
          qDebug() << "Name : " << info.portName();
          qDebug() << "Manufacturer: " << info.manufacturer();
          this->ui->cbx_ports->addItem(info.portName());
      }
      this->ui->cbx_ports->addItem("UDP");
  }

  void DataParameterView::on_btn_connect_clicked()
  {
      if(!IsConnected)
      {
          _currentport= this->ui->cbx_ports->currentText();

          _currentBaud=this->ui->cbx_ports_baudrate->currentText().toInt();

          if(_currentport.isEmpty()||_currentport.isNull())
          {
             myHelper::ShowMessageBoxError("没有发现可用的端口");
             return;
          }
          FrmMainController::Instance()->SetPortName(_currentport,_currentBaud);
          FrmMainController::Instance()->run();
		  
          if(ZYGroundGlobalConfig::m_SerialIsOpen)
          {

            ui->btn_connect->setText(QStringLiteral("断开"));

            btn_takeoffOnekey->setEnabled(true);//打开串口成功，使起飞按钮可用


			btn_takeoffOnekey->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
															font: 14pt \"黑体\";\
															border-top-left-radius:8px;\
															border-bottom-left-radius:8px;\
															background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
															stop:0 rgba(225, 225, 225, 255),\
															stop:1 rgba(185, 185, 185, 255));"));
			btn_takeoffOnekey->setEnabled(true);//打开串口成功，使起飞按钮可用


			btn_takeoffOnekey->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
															font: 14pt \"黑体\";\
															border-top-left-radius:8px;\
															border-bottom-left-radius:8px;\
															background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
															stop:0 rgba(225, 225, 225, 255),\
															stop:1 rgba(185, 185, 185, 255));"));
			ui->btn_info->setEnabled(true);
			ui->btn_info->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
															 font: 14pt \"黑体\";\
															 border-top-left-radius:8px;\
															 border-bottom-left-radius:8px;\
															 background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
															 stop:0 rgba(225, 225, 225, 255),\
															 stop:1 rgba(185, 185, 185, 255));"));
            btn_stick->setEnabled(true);
            btn_stick->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
                                                             font: 14pt \"黑体\";\
                                                             border-top-left-radius:8px;\
                                                             border-bottom-left-radius:8px;\
                                                             background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
                                                             stop:0 rgba(225, 225, 225, 255),\
                                                             stop:1 rgba(185, 185, 185, 255));"));

            connect(&FrmMainController::Instance()->__vehicle->mavLinkMessageInterface, SIGNAL(updateCritActInfo(char*,float)), &m_critActInfo, SLOT(updateCritActInfo(char*,float)));

            if(g_bWriteLog)
            {
                g_FlushFileTimer->start(1000);
            }
            ui->cbx_ports->setEnabled(false);
            ui->cbx_ports_baudrate->setEnabled(false);

            emit signal_setTabEnabled(true);

          }
          else
          {
             FrmMainController::Instance()->stop();
             ui->cbx_ports->setEnabled(true);
             ui->cbx_ports_baudrate->setEnabled(true);

             myHelper::ShowMessageBoxError("端口打开失败，请重试...");
             emit signal_setTabEnabled(false);
             return;
          }
      }
      else
      {
          FrmMainController::Instance()->stop();
          ui->btn_connect->setText(QStringLiteral("连接"));

          btn_takeoffOnekey->setEnabled(false);//关闭串口，使按钮不可用
          btn_land->setEnabled(false);
          btn_changeAlt->setEnabled(false);
          btn_changeYaw->setEnabled(false);
          btn_follow->setEnabled(false);
          btn_stick->setEnabled(false);
		  ui->btn_info->setEnabled(true);
          btn_takeoffOnekey->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
                                                          font: 14pt \"黑体\";\
                                                          border-top-left-radius:8px;\
                                                          border-bottom-left-radius:8px;\
                                                          background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
                                                          stop:0 rgba(185, 185, 185, 255),\
                                                          stop:1 rgba(135, 135, 135, 255));"));
          btn_land->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
                                                 font: 14pt \"黑体\";\
                                                 border-radius:8px;\
                                                 background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
                                                 stop:0 rgba(185, 185, 185, 255),\
                                                 stop:1 rgba(135, 135, 135, 255));"));
          btn_changeAlt->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
                                                      font: 14pt \"黑体\";\
                                                      border-top-left-radius:8px;\
                                                      border-bottom-left-radius:8px;\
                                                      background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
                                                      stop:0 rgba(185, 185, 185, 255),\
                                                      stop:1 rgba(135, 135, 135, 255));"));
          btn_changeYaw->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
                                                      font: 14pt \"黑体\";\
                                                      border-top-left-radius:8px;\
                                                      border-bottom-left-radius:8px;\
                                                      background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
                                                      stop:0 rgba(185, 185, 185, 255),\
                                                      stop:1 rgba(135, 135, 135, 255));"));
          btn_follow->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
                                                   font: 14pt \"黑体\";\
                                                   border-radius:8px;\
                                                   background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
                                                   stop:0 rgba(185, 185, 185, 255),\
                                                   stop:1 rgba(135, 135, 135, 255));"));
		  ui->btn_info->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
													  font: 14pt \"黑体\";\
													  border-radius:8px;\
													  background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
													  stop:0 rgba(185, 185, 185, 255),\
													  stop:1 rgba(135, 135, 135, 255));"));
          btn_stick->setStyleSheet(QStringLiteral("color:rgb(0, 0, 0);\
                                                      font: 14pt \"黑体\";\
                                                      border-radius:8px;\
                                                      background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
                                                      stop:0 rgba(185, 185, 185, 255),\
                                                      stop:1 rgba(135, 135, 135, 255));"));
		  m_critActInfo.close();
         

          _currentport.clear();
          g_FlushFileTimer->stop();

          ui->cbx_ports->setEnabled(true);
          ui->cbx_ports_baudrate->setEnabled(true);

          emit signal_setTabEnabled(false);
      }

      IsConnected=!IsConnected;

      isCommConnected = IsConnected;
  }

  void DataParameterView::UpdateStatus(uint32_t sys_sts,uint16_t gps_sts,uint16_t rtk_sts,uint16_t psi_sts,bool bjs)
  {
      QString strNormalStatus;
      QString strAlarmStatus;
      if(ZYGroundGlobalConfig::RESOLUTION_1920)
      {
          strNormalStatus = QStringLiteral("background-color: rgb(97, 128, 65);font: 14pt \"黑体\";color: rgb(233, 233, 233);");
          strAlarmStatus = QStringLiteral("background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
                                          stop:0 rgba(255, 205, 205, 255),\
                                          stop:1 rgba(230, 60, 70, 255));font: 14pt \"黑体\";color: rgb(233, 233, 233);");
      }
      else
      {
          strNormalStatus = QStringLiteral("background-color: rgb(97, 128, 65);font: 12pt \"黑体\";color: rgb(233, 233, 233);");
          strAlarmStatus = QStringLiteral("background-color: qlineargradient(spread:pad, x1:0.522124, y1:0, x2:0.506, y2:1,\
                                          stop:0 rgba(255, 220, 220, 255),\
                                          stop:1 rgba(230, 60, 70, 255));font: 12pt \"黑体\";color: rgb(233, 233, 233);");
      }
      if(sys_sts&0x00000004)//罗盘
      {
          label_CompassState->setStyleSheet(strNormalStatus);
      }
      else
      {
          label_CompassState->setStyleSheet(strAlarmStatus);
      }

      if(sys_sts&0x00000002)//加速度计
      {
          label_Accelerometer->setStyleSheet(strNormalStatus);
      }
      else
      {
          label_Accelerometer->setStyleSheet(strAlarmStatus);
      }

      //RTK5GPS6航向RTK13遥控器17
      if(sys_sts&0x00010000)
      {
          label_RemoteController->setStyleSheet(strNormalStatus);
      }
      else
      {
          label_RemoteController->setStyleSheet(strAlarmStatus);
      }

      if(sys_sts&0x08000000)//数传
      {
          label_Digital->setStyleSheet(strNormalStatus);
      }
      else
      {
          label_Digital->setStyleSheet(strAlarmStatus);
      }

      if(sys_sts&0x00000020)//GPS
      {
          label_GPS->setStyleSheet(strNormalStatus);
      }
      else
      {
          label_GPS->setStyleSheet(strAlarmStatus);
      }
      if(sys_sts&0x00000010)//RTK
      {
          label_RTK->setStyleSheet(strNormalStatus);
      }
      else
      {
          label_RTK->setStyleSheet(strAlarmStatus);
      }

      if(sys_sts&0x00001000)//航向RTK
      {
          label_HeadingRTK->setStyleSheet(strNormalStatus);
      }
      else
      {
          label_HeadingRTK->setStyleSheet(strAlarmStatus);
      }

      if(sys_sts&0x00010000)//数据记录(sd卡)
          label_RecData->setStyleSheet(strNormalStatus);
      else
          label_RecData->setStyleSheet(strAlarmStatus);

      if(sys_sts&0x10000000)//位置解算
          label_CalPosition->setStyleSheet(strNormalStatus);
      else
          label_CalPosition->setStyleSheet(strAlarmStatus);

      QString strGPS_status;
      QString strRTK_status;
      QString strHeadingRTK_status;

      GetFixType(gps_sts,strGPS_status);
      strGPS_status=GetLabHtmlStr("GPSState.png","GPS:"+strGPS_status);
      label_GPS->setText(strGPS_status);

      GetFixType(rtk_sts,strRTK_status);
      strRTK_status=GetLabHtmlStr("RTKState.png","RTK:"+strRTK_status);
      label_RTK->setText(strRTK_status);

      GetFixType(psi_sts,strHeadingRTK_status);
      strHeadingRTK_status=GetLabHtmlStr("HeadingRTK.png",QStringLiteral("航向RTK:")+strHeadingRTK_status);
      label_HeadingRTK->setText(strHeadingRTK_status);

      if(bjs)
          label_RemoteController->setText(GetLabHtmlStr("RemoteController.png",QStringLiteral("遥杆")));
      else
        label_RemoteController->setText(GetLabHtmlStr("RemoteController.png",QStringLiteral("遥控器")));
 }


void DataParameterView::GetFixType(qint16 status, QString &strFixType)
{
    switch((status>>8)&0x00FF)
    {
        case 0:
           //strFixType += QStringLiteral("无连接");
           strFixType += "0";
           break;

        case 1:
            //strFixType += QStringLiteral("无位置信息");
            strFixType += "1";
            break;

        case 2:
            //strFixType += QStringLiteral("2D定位");
            strFixType += "2";
            break;

        case 3:
            //strFixType += QStringLiteral("3D定位");
            strFixType += "3";
            break;

        case 4:
             //strFixType += QStringLiteral("DGPS定位");
             strFixType += "4";
             break;

        case 5:
             //strFixType += QStringLiteral("RTK浮动解");
             strFixType += "5";
             break;

        case 6:
             //strFixType += QStringLiteral("RTK固定解");
             strFixType += "6";
             break;
        default:
             break;
    }

    qint8 starCount = status&0x00FF;

    strFixType+=",";

    strFixType += QString::number(starCount);

    strFixType += "*";

}

void DataParameterView::on_btn_exit_clicked()
{
    if(myHelper::ShowMessageBoxQuesion("确定要退出软件么?")!=1)
    {
        return;
    }
    exit(0);
}


void DataParameterView::on_cbx_ports_currentTextChanged(const QString &arg1)
{
    if(arg1 == "UDP")
    {
        ui->cbx_ports_baudrate->clear();
        ui->cbx_ports_baudrate->addItem("14550");
    }
    else if(arg1.contains("COM"))
    {
        ui->cbx_ports_baudrate->clear();
        ui->cbx_ports_baudrate->addItem("57600");
        ui->cbx_ports_baudrate->addItem("115200");
        ui->cbx_ports_baudrate->setCurrentIndex(1);
    }
}

void DataParameterView::on_btn_info_clicked()
{
    if(!m_critActInfo.isVisible())
    {
        m_critActInfo.showNormal();
    }
}

void DataParameterView::on_btn_stick_clicked()
{
    if(!ZYGroundGlobalConfig::m_bUsedMyStick)
        return;
    bool bUsedMyStick=true;
    if(ZYGroundGlobalConfig::m_sStickPortName=="")
        bUsedMyStick=false;
    if(btn_stick->text()==QStringLiteral("开启摇杆"))
        btn_stick->setText(QStringLiteral("关闭摇杆"));
    else
        btn_stick->setText(QStringLiteral("开启摇杆"));

    if(btn_stick->text()==QStringLiteral("关闭摇杆"))
    {
        if(bUsedMyStick)
        {
            if(!m_pZYStick)
                m_pZYStick=new ZYStick(this);
            if(!m_pZYStick->linkOpen(ZYGroundGlobalConfig::m_sStickPortName
                                     ,ZYGroundGlobalConfig::m_sStickBaudRate))
                qDebug()<<"Open ZY Stick failed";
            else
                qDebug()<<"Open ZY Stick success";
        }
        else
        {
            if(!m_pJoystick)
                m_pJoystick = new JoyStickToFlyController();
            m_pJoystick->startPolling();
        }

        FrmMainController::Instance()->setRCType(1);
    }
    else
    {
        if(bUsedMyStick)
            m_pZYStick->linkClose();
        else
            m_pJoystick->stopPolling();

        FrmMainController::Instance()->setRCType(0);
    }
}

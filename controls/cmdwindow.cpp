#include "cmdwindow.h"
#include "ui_cmdwindow.h"
//#include <common/mavlink.h>
#include "connections/CommonStru.h"
#include "controllers/frmmaincontroller.h"
#include "connections/mavlinkmessageinterface.h"

CmdWindow::CmdWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CmdWindow)
{
    ui->setupUi(this);

   InitialControl();


//   lm = LinkManager::getInstance();
//   vm = VehicleManager::getInstance();
//   ci = lm->addLink(SerialLinkType);
//   ci->linkOpen("COM4");
//   vehicle = vm->addVehicle(this);
//   vehicle->connectLink(ci);

   //connect(&vehicle->mavLinkMessageInterface,SIGNAL(onParamValue(mavlink_param_value_t)),this,SLOT(onParamValue(mavlink_param_value_t)),Qt::AutoConnection);
   //vehicle->mavLinkMessageInterface.requestDataStreamExtendedStatus(1,2);  //#62 导航控制器输出
   //vehicle->mavLinkMessageInterface.requestDataStreamRcChannels(1,2);      //#35 遥控通道RAW 格式数据
   //vehicle->mavLinkMessageInterface.requestDataStreamRawController(1,2);//
   //vehicle->mavLinkMessageInterface.requestDataStreamPosition(1,2);        //#62 导航控制器输出
}

CmdWindow::~CmdWindow()
{
    delete ui;
}

void CmdWindow::InitialControl()
{
    //连接预警模式
//    ui->cbx_SwitchMode->addItem(QString::fromLocal8Bit("飞行前准备"),MAV_MODE_PREFLIGHT);
//    ui->cbx_SwitchMode->addItem(QString::fromLocal8Bit("手动非预警"),MAV_MODE_MANUAL_DISARMED);
//    ui->cbx_SwitchMode->addItem(QString::fromLocal8Bit("测试非预警"),MAV_MODE_TEST_DISARMED);
//    ui->cbx_SwitchMode->addItem(QString::fromLocal8Bit("稳定非预警"),MAV_MODE_STABILIZE_DISARMED);
//    ui->cbx_SwitchMode->addItem(QString::fromLocal8Bit("引导非预警"),MAV_MODE_GUIDED_DISARMED);
//    ui->cbx_SwitchMode->addItem(QString::fromLocal8Bit("自动非预警"),MAV_MODE_AUTO_DISARMED);
//    ui->cbx_SwitchMode->addItem(QString::fromLocal8Bit("手动预警"),MAV_MODE_MANUAL_ARMED);
//    ui->cbx_SwitchMode->addItem(QString::fromLocal8Bit("测试预警"),MAV_MODE_TEST_ARMED);
//    ui->cbx_SwitchMode->addItem(QString::fromLocal8Bit("稳定预警"),MAV_MODE_STABILIZE_ARMED);
//    ui->cbx_SwitchMode->addItem(QString::fromLocal8Bit("引导预警"),MAV_MODE_GUIDED_ARMED);
//    ui->cbx_SwitchMode->addItem(QString::fromLocal8Bit("自动预警"),MAV_MODE_AUTO_ARMED);


  ui->cbx_SwitchCommand->addItem("MAV_CMD_NAV_WAYPOINT             ",MAV_CMD_NAV_WAYPOINT             );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_NAV_LOITER_UNLIM         ",MAV_CMD_NAV_LOITER_UNLIM         );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_NAV_LOITER_TURNS         ",MAV_CMD_NAV_LOITER_TURNS         );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_NAV_LOITER_TIME          ",MAV_CMD_NAV_LOITER_TIME          );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_NAV_RETURN_TO_LAUNCH     ",MAV_CMD_NAV_RETURN_TO_LAUNCH     );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_NAV_LAND                 ",MAV_CMD_NAV_LAND                 );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_NAV_TAKEOFF              ",MAV_CMD_NAV_TAKEOFF              );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_NAV_ROI                  ",MAV_CMD_NAV_ROI                  );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_NAV_PATHPLANNING         ",MAV_CMD_NAV_PATHPLANNING         );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_NAV_SPLINE_WAYPOINT      ",MAV_CMD_NAV_SPLINE_WAYPOINT      );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_NAV_GUIDED_ENABLE        ",MAV_CMD_NAV_GUIDED_ENABLE        );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_NAV_LAST                 ",MAV_CMD_NAV_LAST                 );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_CONDITION_DELAY          ",MAV_CMD_CONDITION_DELAY          );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_CONDITION_CHANGE_ALT     ",MAV_CMD_CONDITION_CHANGE_ALT     );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_CONDITION_DISTANCE       ",MAV_CMD_CONDITION_DISTANCE       );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_CONDITION_YAW            ",MAV_CMD_CONDITION_YAW            );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_CONDITION_LAST           ",MAV_CMD_CONDITION_LAST           );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_DO_SET_MODE              ",MAV_CMD_DO_SET_MODE              );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_DO_JUMP                  ",MAV_CMD_DO_JUMP                  );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_DO_CHANGE_SPEED          ",MAV_CMD_DO_CHANGE_SPEED          );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_DO_SET_HOME              ",MAV_CMD_DO_SET_HOME              );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_DO_SET_PARAMETER         ",MAV_CMD_DO_SET_PARAMETER         );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_DO_SET_RELAY             ",MAV_CMD_DO_SET_RELAY             );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_DO_REPEAT_RELAY          ",MAV_CMD_DO_REPEAT_RELAY          );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_DO_SET_SERVO             ",MAV_CMD_DO_SET_SERVO             );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_DO_REPEAT_SERVO          ",MAV_CMD_DO_REPEAT_SERVO          );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_DO_FLIGHTTERMINATION     ",MAV_CMD_DO_FLIGHTTERMINATION     );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_DO_LAND_START            ",MAV_CMD_DO_LAND_START            );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_DO_RALLY_LAND            ",MAV_CMD_DO_RALLY_LAND            );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_DO_GO_AROUND             ",MAV_CMD_DO_GO_AROUND             );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_DO_CONTROL_VIDEO         ",MAV_CMD_DO_CONTROL_VIDEO         );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_DO_SET_ROI               ",MAV_CMD_DO_SET_ROI               );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_DO_DIGICAM_CONFIGURE     ",MAV_CMD_DO_DIGICAM_CONFIGURE     );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_DO_DIGICAM_CONTROL       ",MAV_CMD_DO_DIGICAM_CONTROL       );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_DO_MOUNT_CONFIGURE       ",MAV_CMD_DO_MOUNT_CONFIGURE       );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_DO_MOUNT_CONTROL         ",MAV_CMD_DO_MOUNT_CONTROL         );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_DO_SET_CAM_TRIGG_DIST    ",MAV_CMD_DO_SET_CAM_TRIGG_DIST    );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_DO_FENCE_ENABLE          ",MAV_CMD_DO_FENCE_ENABLE          );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_DO_PARACHUTE             ",MAV_CMD_DO_PARACHUTE             );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_DO_INVERTED_FLIGHT       ",MAV_CMD_DO_INVERTED_FLIGHT       );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_DO_MOUNT_CONTROL_QUAT    ",MAV_CMD_DO_MOUNT_CONTROL_QUAT    );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_DO_GUIDED_MASTER         ",MAV_CMD_DO_GUIDED_MASTER         );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_DO_GUIDED_LIMITS         ",MAV_CMD_DO_GUIDED_LIMITS         );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_DO_LAST                  ",MAV_CMD_DO_LAST                  );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_PREFLIGHT_CALIBRATION    ",MAV_CMD_PREFLIGHT_CALIBRATION    );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_PREFLIGHT_SET_SENSOR_OFFSETS ",MAV_CMD_PREFLIGHT_SET_SENSOR_OFFSETS );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_PREFLIGHT_STORAGE        ",MAV_CMD_PREFLIGHT_STORAGE        );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_PREFLIGHT_REBOOT_SHUTDOWN",MAV_CMD_PREFLIGHT_REBOOT_SHUTDOWN);
  ui->cbx_SwitchCommand->addItem("MAV_CMD_OVERRIDE_GOTO            ",MAV_CMD_OVERRIDE_GOTO            );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_MISSION_START            ",MAV_CMD_MISSION_START            );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_COMPONENT_ARM_DISARM     ",MAV_CMD_COMPONENT_ARM_DISARM     );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_START_RX_PAIR            ",MAV_CMD_START_RX_PAIR            );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_IMAGE_START_CAPTURE      ",MAV_CMD_IMAGE_START_CAPTURE      );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_IMAGE_STOP_CAPTURE       ",MAV_CMD_IMAGE_STOP_CAPTURE       );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_VIDEO_START_CAPTURE      ",MAV_CMD_VIDEO_START_CAPTURE      );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_VIDEO_STOP_CAPTURE       ",MAV_CMD_VIDEO_STOP_CAPTURE       );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_PANORAMA_CREATE          ",MAV_CMD_PANORAMA_CREATE          );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_PAYLOAD_PREPARE_DEPLOY   ",MAV_CMD_PAYLOAD_PREPARE_DEPLOY   );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_PAYLOAD_CONTROL_DEPLOY   ",MAV_CMD_PAYLOAD_CONTROL_DEPLOY   );
  ui->cbx_SwitchCommand->addItem("MAV_CMD_ENUM_END                 ",MAV_CMD_ENUM_END                 );



//  ui->cbx_SwitchbaseMode->addItem("CUSTOM_MODE_ENABLED",CUSTOM_MODE_ENABLED);
//  ui->cbx_SwitchbaseMode->addItem("TEST_ENABLED",TEST_ENABLED);
//  ui->cbx_SwitchbaseMode->addItem("AUTO_ENABLED",AUTO_ENABLED);
//  ui->cbx_SwitchbaseMode->addItem("GUIDED_ENABLED",GUIDED_ENABLED);
//  ui->cbx_SwitchbaseMode->addItem("STABILIZE_ENABLED",STABILIZE_ENABLED);
//  ui->cbx_SwitchbaseMode->addItem("HIL_ENABLED",HIL_ENABLED);
//  ui->cbx_SwitchbaseMode->addItem("MANUAL_INPUT_ENABLED",MANUAL_INPUT_ENABLED);
//  ui->cbx_SwitchbaseMode->addItem("SAFETY_ARMED",SAFETY_ARMED);
//  ui->cbx_SwitchbaseMode->addItem("ENUM_END",ENUM_END);


//  ui->cbx_custumMode->addItem("CUSTOM_MODE_ENABLED",CUSTOM_MODE_ENABLED);
//  ui->cbx_custumMode->addItem("TEST_ENABLED",TEST_ENABLED);
//  ui->cbx_custumMode->addItem("AUTO_ENABLED",AUTO_ENABLED);
//  ui->cbx_custumMode->addItem("GUIDED_ENABLED",GUIDED_ENABLED);
//  ui->cbx_custumMode->addItem("STABILIZE_ENABLED",STABILIZE_ENABLED);
//  ui->cbx_custumMode->addItem("HIL_ENABLED",HIL_ENABLED);
//  ui->cbx_custumMode->addItem("MANUAL_INPUT_ENABLED",MANUAL_INPUT_ENABLED);
//  ui->cbx_custumMode->addItem("SAFETY_ARMED",SAFETY_ARMED);
//  ui->cbx_custumMode->addItem("ENUM_END",ENUM_END);
}


//void CmdWindow::on_btn_getWP_clicked(bool noUsed)
//{

//}

void CmdWindow::onMissionList(std::vector<mavlink_mission_item_int_t*> items){
    int i = items.size();
    for(std::vector<mavlink_mission_item_int_t*>::iterator iter = items.begin(); iter != items.end(); ++iter){
        float x = (*iter)->x;
        float y = (*iter)->y;
        float z = (*iter)->z;
        qDebug("==========(%f,%f,%f)\n",x,y,z);
    }
}




void CmdWindow::on_pushClearStream_clicked()
{
    vehicle->mavLinkMessageInterface.requestDataStreamAll(0,1);
}

void CmdWindow::on_pushButtonTmp_clicked()
{
    //
    //vehicle->mavLinkMessageInterface.commandDoSetHome(MAV_FRAME_GLOBAL,1,1,1,8.60f,14.3f,0);
    //vehicle->mavLinkMessageInterface.commandGetHomePosition(MAV_FRAME_GLOBAL,1,1);

    //参数请求列表
    //vehicle->mavLinkMessageInterface.paramRequestList();
    //设置参数
    //vehicle->mavLinkMessageInterface.paramSet(1,"RSSI_PIN",2);
    //vehicle->mavLinkMessageInterface.paramSet(3.123,"GPS_AUTO_SWITCH",2);
    //取指定参数
    //vehicle->mavLinkMessageInterface.paramRequestRead("GPS_AUTO_SWITCH");

    //vehicle->mavLinkMessageInterface.commandDoSetParameter(MAV_FRAME_MISSION,123,2000);


    //data stream
    //vehicle->mavLinkMessageInterface.requestDataStreamAll(0,2);
    //vehicle->mavLinkMessageInterface.requestDataStreamRawSensors(1,2);      //#27 #29 #116
    //vehicle->mavLinkMessageInterface.requestDataStreamExtendedStatus(1,2);  //#1 #24 #42 #62 #125
    //vehicle->mavLinkMessageInterface.requestDataStreamRcChannels(1,2);      //#35 #36
    vehicle->mavLinkMessageInterface.requestDataStreamRawController(1,2);   //
    //vehicle->mavLinkMessageInterface.requestDataStreamPosition(1,2);        //#33
    //vehicle->mavLinkMessageInterface.requestDataStreamExtra1(1,2);          //#30
    //vehicle->mavLinkMessageInterface.requestDataStreamExtra2(1,2);          //#74
    //vehicle->mavLinkMessageInterface.requestDataStreamExtra3(1,2);          //#2 #241

    //vehicle->mavLinkMessageInterface.commandDoSetHome(0,0,3.61f,14.3f,500.0f);
}

void CmdWindow::onParamValue(mavlink_param_value_t paramValue){
    qDebug("========paramValue=======(%f,%s,%d)",paramValue.param_value,paramValue.param_id,paramValue.param_type);


}

void CmdWindow::on_pushButton_clicked()
{

       InitialControl();
//       LoadEvents();

       ci = lm->addLink(SerialLinkType);
       ci->linkOpen("COM4",57600);
       vehicle = vm->addVehicle(this);
       //vehicle->connectLink(ci);
       connect(&vehicle->mavLinkMessageInterface,SIGNAL(onParamValue(mavlink_param_value_t)),this,SLOT(onParamValue(mavlink_param_value_t)),Qt::AutoConnection);

       vehicle->mavLinkMessageInterface.requestDataStreamExtendedStatus(1,2);  //#62 导航控制器输出
       vehicle->mavLinkMessageInterface.requestDataStreamPosition(1,2);        //#62 导航控制器输出
}
/**************************************************************/

void CmdWindow::on_btn_getWP_clicked()
{
    bool ret = connect(vehicle->mavLinkMessageInterface.getMissionHandle(), SIGNAL(onMissionList(std::vector<mavlink_mission_item_int_t*>)), this, SLOT(onMissionList(std::vector<mavlink_mission_item_int_t*>)));
    vehicle->mavLinkMessageInterface.readMavWaypointList();
}

void CmdWindow::on_btn_getParam_clicked()
{

}

void CmdWindow::on_btn_RTL_clicked()
{

}

void CmdWindow::on_btn_sendWP_clicked()
{
    std::vector<mavlink_mission_item_int_t*> items;
    items.clear();

    mavlink_mission_item_int_t* item = new mavlink_mission_item_int_t ;
    item->param1 = 0;//
    item->param2 = 5;
    item->param3 = 0;
    item->param4 = 0;
    item->x      = 0.f;
    item->y      = 0.f;
    item->z      = 0.f;
    item->seq = 0;
    item->command = MAV_CMD_NAV_WAYPOINT;
    item->frame = MAV_FRAME_MISSION;
    item->current = 1;
    item->autocontinue = 1;
    items.push_back(item);

     item = new mavlink_mission_item_int_t ;
    item->param1 = 0;//
    item->param2 = 5;
    item->param3 = 0;
    item->param4 = 0;
    item->x      = 1.59f;
    item->y      = 14.3f;
    item->z      = 500.0f;
    item->seq = 1;
    item->command = MAV_CMD_NAV_WAYPOINT;
    item->frame = MAV_FRAME_MISSION;
    item->current = 1;
    item->autocontinue = 1;
    items.push_back(item);

    item = new mavlink_mission_item_int_t ;
    item->param1 = 0;//
    item->param2 = 5;
    item->param3 = 0;
    item->param4 = 0;
    item->x      = 2.60f;
    item->y      = 14.3f;
    item->z      = 500.0f;
    item->seq = 2;
    item->command = MAV_CMD_NAV_WAYPOINT;
    item->frame = MAV_FRAME_MISSION;
    item->current = 1;
    item->autocontinue = 1;
    items.push_back(item);

    item = new mavlink_mission_item_int_t ;
    item->param1 = 0;//
    item->param2 = 5;
    item->param3 = 0;
    item->param4 = 0;
    item->x      = 3.61f;
    item->y      = 14.3f;
    item->z      = 500.0f;
    item->seq = 3;
    item->command = MAV_CMD_NAV_WAYPOINT;
    item->frame = MAV_FRAME_MISSION;
    item->current = 1;
    item->autocontinue = 1;
    items.push_back(item);

    vehicle->mavLinkMessageInterface.writeMavWaypointList(items);
}

void CmdWindow::on_btn_sendParam_clicked()
{

}

void CmdWindow::on_btn_ModeSet_clicked()
{

}
void CmdWindow::on_btn_cmd_send_clicked()
{
      float p1=  ui->lineEdit->text().toFloat();
      float p2=  ui->lineEdit_2->text().toFloat();
      float p3=  ui->lineEdit_3->text().toFloat();
      float p4=  ui->lineEdit_4->text().toFloat();
      float p5=  ui->lineEdit_5->text().toFloat();
      float p6=  ui->lineEdit_6->text().toFloat();
      float p7=  ui->lineEdit_7->text().toFloat();
     int t_cmd=ui->cbx_SwitchCommand->currentData().toInt();
     if(FrmMainController::Instance()->__vehicle)
     FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.doCommand((MAV_CMD)t_cmd,p1,p2,p3,p4,p5,p6,p7);
}

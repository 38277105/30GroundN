#include "statusmonitor.h"
#include "ui_statusmonitor.h"
#include <QDebug>
#include <QMessageBox>
#include "./Utils/zygroundglobalconfig.h"
#include "./../controllers/frmmaincontroller.h"

StatusMonitor::StatusMonitor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusMonitor)
{
    ui->setupUi(this);

    m_bakVolt_coefficient = 3;
    setMonitor();
    reshapeLabel();

    ui->lbl_ESC_K_desc->hide();
    ui->lbl_ESC_L_desc->hide();
    ui->lbl_ESC_K_value->hide();
    ui->lbl_ESC_L_value->hide();
    ui->line_30->hide();
    ui->horizontalLayout_11->removeItem(ui->verticalSpacer_31);
    ui->horizontalLayout_11->removeItem(ui->verticalSpacer_32);
    ui->horizontalLayout_11->removeItem(ui->verticalSpacer_33);
    ui->horizontalLayout_11->removeItem(ui->verticalSpacer_34);
    ui->line_16->hide();

    ui->lbl_ESC_I_desc->hide();
    ui->lbl_ESC_J_desc->hide();
    ui->lbl_ESC_I_value->hide();
    ui->lbl_ESC_J_value->hide();
    ui->line_29->hide();
    ui->horizontalLayout_9->removeItem(ui->verticalSpacer_27);
    ui->horizontalLayout_9->removeItem(ui->verticalSpacer_28);
    ui->horizontalLayout_9->removeItem(ui->verticalSpacer_29);
    ui->horizontalLayout_9->removeItem(ui->verticalSpacer_30);
    ui->line_15->hide();

    connect(&m_timeAdd,SIGNAL(timeout()),this,SLOT(TimeAddSlot()));
    m_timeAdd.start(1000);
    m_tmCount=0;
}

StatusMonitor::~StatusMonitor()
{
    m_timeAdd.stop();
    delete ui;
}

void StatusMonitor::setStatus(VehicleState _state)
{

    m_PA_mainVolt.checkValue(_state.MainVoltage);
    m_PA_bakVolt.checkValue(_state.BakVoltage);
    m_PA_mainCurrent.checkValue(_state.throttle);
    m_PA_Temperature.checkValue(_state.Temperature);

//电机转速暂时没有数据
//    if(_state.Z - 5 > 0.01)
//    {
    m_PA_ESC_A.checkValue(_state.ESC_Current[0]);
    m_PA_ESC_B.checkValue(_state.ESC_Current[1]);
    m_PA_ESC_C.checkValue(_state.ESC_Current[2]);
    m_PA_ESC_D.checkValue(_state.ESC_Current[3]);
    m_PA_ESC_E.checkValue(_state.ESC_Current[4]);
    m_PA_ESC_F.checkValue(_state.ESC_Current[5]);
    m_PA_ESC_G.checkValue(_state.ESC_Current[6]);
    m_PA_ESC_H.checkValue(_state.ESC_Current[7]);
    m_PA_ESC_I.checkValue(_state.ESC_Current[8]);
    m_PA_ESC_J.checkValue(_state.ESC_Current[9]);
    m_PA_ESC_K.checkValue(_state.ESC_Current[10]);
    m_PA_ESC_L.checkValue(_state.ESC_Current[11]);
//    }


    //this->ui->lbl_mainVolt_value->setText(QString::number(_state.MainVoltage, 'f', 2));
    this->ui->lbl_mainCurrent_value->setText(QString::number(_state.throttle));
    //this->ui->lbl_powerTempu_value->setText(QString::number(_state.Temperature, 'f', 2));
    this->ui->lbl_bakVolt_value->setText(QString::number(_state.BakVoltage, 'f', 2));

    this->ui->ll_east->setLabelText(QString::number(_state.X,'f', 2) + "m");
    this->ui->ll_north->setLabelText(QString::number(_state.Y,'f', 2)+ "m");
    this->ui->ll_height->setLabelText(QString::number(_state.Z, 'f', 2) + + "m");
    this->ui->ll_roll->setLabelText(QString::number(_state.roll, 'f', 2) + QStringLiteral("°"));
    this->ui->ll_pitch->setLabelText(QString::number(_state.pitch, 'f', 2) + QStringLiteral("°"));
    if(_state.yaw>360||_state.yaw<-360)
    {
       _state.yaw=-1000;
    }
    this->ui->ll_yaw->setLabelText(QString::number(_state.yaw, 'f', 2) + QStringLiteral("°"));
    this->ui->ll_Hspeed->setLabelText(QString::number(_state.groundspeed, 'f', 2) + "m/s");
    this->ui->ll_Vspeed->setLabelText(QString::number(_state.climb, 'f', 2) + "m/s");

    //this->ui->widget_throttle->set_angle(_state.throttle);

    //this->ui->widget_heading->set_angle(_state.yaw);

    this->ui->lbl_ESC_A_value->setLabelText(QString::fromLocal8Bit("%1").arg(_state.ESC_Current[0]));
    this->ui->lbl_ESC_B_value->setLabelText(QString::fromLocal8Bit("%1").arg(_state.ESC_Current[1]));
    this->ui->lbl_ESC_C_value->setLabelText(QString::fromLocal8Bit("%1").arg(_state.ESC_Current[2]));
    this->ui->lbl_ESC_D_value->setLabelText(QString::fromLocal8Bit("%1").arg(_state.ESC_Current[3]));
    this->ui->lbl_ESC_E_value->setLabelText(QString::fromLocal8Bit("%1").arg(_state.ESC_Current[4]));
    this->ui->lbl_ESC_F_value->setLabelText(QString::fromLocal8Bit("%1").arg(_state.ESC_Current[5]));
    this->ui->lbl_ESC_G_value->setLabelText(QString::fromLocal8Bit("%1").arg(_state.ESC_Current[6]));
    this->ui->lbl_ESC_H_value->setLabelText(QString::fromLocal8Bit("%1").arg(_state.ESC_Current[7]));
    this->ui->lbl_ESC_I_value->setLabelText(QString::fromLocal8Bit("%1").arg(_state.ESC_Current[8]));
    this->ui->lbl_ESC_J_value->setLabelText(QString::fromLocal8Bit("%1").arg(_state.ESC_Current[9]));
    this->ui->lbl_ESC_K_value->setLabelText(QString::fromLocal8Bit("%1").arg(_state.ESC_Current[10]));
    this->ui->lbl_ESC_L_value->setLabelText(QString::fromLocal8Bit("%1").arg(_state.ESC_Current[11]));

}

void StatusMonitor::setMonitor()
{
    alarmConfig tmpConfig;
    tmpConfig.minValue = 25;
    tmpConfig.maxValue = 62;
    //m_PA_mainVolt.setAlarmConfig(tmpConfig,ui->lbl_mainVolt_desc);

    tmpConfig.minValue = 43;//3.8*m_bakVolt_coefficient;
    tmpConfig.maxValue = 62;
    tmpConfig.alarmOffset = 2;
    tmpConfig.warnOffset = 1;
    m_PA_bakVolt.setAlarmConfig(tmpConfig,ui->lbl_bakVolt_desc);
    tmpConfig.alarmOffset = 0;
    tmpConfig.warnOffset = 0;
    //主电电流现在没有了，暂时用油门代替0%-75%,超过75报警
    tmpConfig.minValue = 0;
    tmpConfig.maxValue = 75;
    m_PA_mainCurrent.setAlarmConfig(tmpConfig,ui->lbl_mainCurrent_desc);

    tmpConfig.minValue = 10;
    tmpConfig.maxValue = 100;
    //m_PA_Temperature.setAlarmConfig(tmpConfig,ui->lbl_powerTempu_desc);

    tmpConfig.minValue = 1000;
    tmpConfig.maxValue = 1700;
    m_PA_ESC_A.setAlarmConfig(tmpConfig,ui->lbl_ESC_A_desc);
    m_PA_ESC_B.setAlarmConfig(tmpConfig,ui->lbl_ESC_B_desc);
    m_PA_ESC_C.setAlarmConfig(tmpConfig,ui->lbl_ESC_C_desc);
    m_PA_ESC_D.setAlarmConfig(tmpConfig,ui->lbl_ESC_D_desc);
    m_PA_ESC_E.setAlarmConfig(tmpConfig,ui->lbl_ESC_E_desc);
    m_PA_ESC_F.setAlarmConfig(tmpConfig,ui->lbl_ESC_F_desc);
    m_PA_ESC_G.setAlarmConfig(tmpConfig,ui->lbl_ESC_G_desc);
    m_PA_ESC_H.setAlarmConfig(tmpConfig,ui->lbl_ESC_H_desc);
    m_PA_ESC_I.setAlarmConfig(tmpConfig,ui->lbl_ESC_I_desc);
    m_PA_ESC_J.setAlarmConfig(tmpConfig,ui->lbl_ESC_J_desc);
    m_PA_ESC_K.setAlarmConfig(tmpConfig,ui->lbl_ESC_K_desc);
    m_PA_ESC_L.setAlarmConfig(tmpConfig,ui->lbl_ESC_L_desc);

    tmpConfig.minValue=600;
    tmpConfig.maxValue=60000;
    m_PA_RotorRemainTime.setAlarmConfig(tmpConfig,ui->label_rotorremaintime_value);
    m_PA_MotorRemainTime.setAlarmConfig(tmpConfig,ui->label_motorremaintime_value);
    m_PA_EleDebugRemainTime.setAlarmConfig(tmpConfig,ui->label_eledebugremaintime_value);
    m_PA_BatteryRemainTime.setAlarmConfig(tmpConfig,ui->label_batteryremaintime_value);

}

void StatusMonitor::TimeAddSlot()
{
    Vehicle*  pVeh=FrmMainController::Instance()->__vehicle;
    if(!pVeh)
        return;
    if(pVeh->mavLinkMessageInterface.getArmedState()) //飞机已经解锁
    {
        m_tmCount++;
        if(m_tmCount%60==0)  //一分钟
        {
            //ZYGroundGlobalConfig::m_xuanyi_outtime--;
            ZYGroundGlobalConfig::SubOutTime();

            //旋翼，电机，电调，电池剩余时间字体显示,飞行时长显示
            m_PA_RotorRemainTime.checkValue(ZYGroundGlobalConfig::m_RotorRemainTime);
            this->ui->label_rotorremaintime_value->setLabelText(QString::number(ZYGroundGlobalConfig::m_RotorRemainTime));
            m_PA_MotorRemainTime.checkValue(ZYGroundGlobalConfig::m_MotorRemainTime);
            this->ui->label_motorremaintime_value->setLabelText(QString::number(ZYGroundGlobalConfig::m_MotorRemainTime));
            m_PA_EleDebugRemainTime.checkValue(ZYGroundGlobalConfig::m_EleDebugRemainTime);
            this->ui->label_eledebugremaintime_value->setLabelText(QString::number(ZYGroundGlobalConfig::m_EleDebugRemainTime));
            m_PA_BatteryRemainTime.checkValue(ZYGroundGlobalConfig::m_BatteryRemainTime);
            this->ui->label_batteryremaintime_value->setLabelText(QString::number(ZYGroundGlobalConfig::m_BatteryRemainTime));
            //飞行时长显示
            this->ui->lbl_flytime_value->setText(QString::number(ZYGroundGlobalConfig::m_flytime));
        }
    }
}

void StatusMonitor::tipMessageBox(const QString& title, const QString& content){
    QMessageBox box(QMessageBox::Warning,title,content);
        box.setStandardButtons (QMessageBox::Ok/*|QMessageBox::Cancel*/);
        box.setButtonText (QMessageBox::Ok,QString(QString::fromLocal8Bit("确 定")));
//        box.setButtonText (QMessageBox::Cancel,QString("取 消"));
        box.exec ();
}


void StatusMonitor::on_btn_set_bakVolt_coefficient_clicked()
{
//    QString cftContext = ui->lineE_bakVolt_coefficient_value->text();
//    int tmp = cftContext.toInt();
//    if(tmp == 0 || tmp > 20){
//        ui->lineE_bakVolt_coefficient_value->setText(QString::number(m_bakVolt_coefficient));
//        return;
//    }
//    m_bakVolt_coefficient = tmp;
//    alarmConfig bakVoltConfig;
//    bakVoltConfig.minValue = 3.8*m_bakVolt_coefficient;
//    bakVoltConfig.maxValue = 62;
//    m_PA_bakVolt.setAlarmConfig(bakVoltConfig,ui->lbl_bakVolt_desc);
//    tipMessageBox(QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("当前备电系数为 ") + cftContext);
    //QMessageBox::information(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("当前备电系数为 ") + cftContext, QMessageBox::Yes, QMessageBox::Yes);
}

void StatusMonitor::setFlightMode(QString strText)
{
    ui->lbl_flymode_value->setLabelText(strText);
}

void StatusMonitor::setScale(float scale)
{
   //ui->widget_heading->set_scale(scale);
   //ui->widget_throttle->set_scale(scale);
}

void StatusMonitor::reshapeLabel()
{
   QString strStyle;
   qint32 fontsize;
   if(ZYGroundGlobalConfig::RESOLUTION_1920)
   {
       strStyle = QStringLiteral("font: 15pt \"黑体\";\
                  color: rgb(233, 233, 233);\
                   background-color: rgb(125, 125, 125);");
       fontsize = 15;
   }
   else
   {
       strStyle = QStringLiteral("font: 12pt \"黑体\";\
                  color: rgb(233, 233, 233);\
                   background-color: rgb(125, 125, 125);");
       fontsize = 12;
   }
   //this->ui->lbl_mainVolt_desc->setImage(":/image/control/control_01.png");
   //this->ui->lbl_mainVolt_desc->setFontSize(fontsize);
   //this->ui->lbl_mainVolt_desc->setLabelText(QStringLiteral("主电压监控"));
   //this->ui->lbl_mainVolt_value->setStyleSheet(strStyle);
   //this->ui->lbl_mainVolt_desc_2->setImage(":/image/control/control_06.png");
   //this->ui->lbl_mainVolt_desc_2->setFontSize(fontsize);
   //this->ui->lbl_mainVolt_desc_2->setLabelText(QStringLiteral("伏特"));

   this->ui->lbl_bakVolt_desc->setImage(":/image/control/control_01.png");
   this->ui->lbl_bakVolt_desc->setFontSize(fontsize);
   this->ui->lbl_bakVolt_desc->setLabelText(QStringLiteral("电池电压监控"));
   this->ui->lbl_bakVolt_value->setStyleSheet(strStyle);
   this->ui->lbl_bakVolt_desc_2->setImage(":/image/control/control_06.png");
   this->ui->lbl_bakVolt_desc_2->setFontSize(fontsize);
   this->ui->lbl_bakVolt_desc_2->setLabelText(QStringLiteral("伏特"));

   this->ui->lbl_mainCurrent_desc->setImage(":/image/control/control_01.png");
   this->ui->lbl_mainCurrent_desc->setFontSize(fontsize);
   this->ui->lbl_mainCurrent_desc->setLabelText(QStringLiteral("油门"));
   this->ui->lbl_mainCurrent_value->setStyleSheet(strStyle);
   this->ui->lbl_mainCurrent_desc_2->setImage(":/image/control/control_06.png");
   this->ui->lbl_mainCurrent_desc_2->setFontSize(fontsize);
   this->ui->lbl_mainCurrent_desc_2->setLabelText(QStringLiteral("\%"));

//   this->ui->lbl_powerTempu_desc->setImage(":/image/control/control_01.png");
//   this->ui->lbl_powerTempu_desc->setFontSize(fontsize);
//   this->ui->lbl_powerTempu_desc->setLabelText(QStringLiteral("机载电源温度"));
//   this->ui->lbl_powerTempu_value->setStyleSheet(strStyle);
//   this->ui->lbl_powerTempu_desc_2->setImage(":/image/control/control_06.png");
//   this->ui->lbl_powerTempu_desc_2->setFontSize(fontsize);
//   this->ui->lbl_powerTempu_desc_2->setLabelText(QStringLiteral("度"));

   this->ui->lbl_ESC_A_desc->setImage(":/image/control/control_01.png");
   this->ui->lbl_ESC_A_desc->setFontSize(fontsize);
   this->ui->lbl_ESC_A_desc->setLabelText(QStringLiteral("电机1转速"));
   this->ui->lbl_ESC_A_value->setImage(":/image/control/control_05.png");
   this->ui->lbl_ESC_A_value->setFontSize(fontsize);


   this->ui->lbl_ESC_B_desc->setImage(":/image/control/control_01.png");
   this->ui->lbl_ESC_B_desc->setFontSize(fontsize);
   this->ui->lbl_ESC_B_desc->setLabelText(QStringLiteral("电机2转速"));
   this->ui->lbl_ESC_B_value->setImage(":/image/control/control_05.png");
   this->ui->lbl_ESC_B_value->setFontSize(fontsize);


   this->ui->lbl_ESC_C_desc->setImage(":/image/control/control_01.png");
   this->ui->lbl_ESC_C_desc->setFontSize(fontsize);
   this->ui->lbl_ESC_C_desc->setLabelText(QStringLiteral("电机3转速"));
   this->ui->lbl_ESC_C_value->setImage(":/image/control/control_05.png");
   this->ui->lbl_ESC_C_value->setFontSize(fontsize);

   this->ui->lbl_ESC_D_desc->setImage(":/image/control/control_01.png");
   this->ui->lbl_ESC_D_desc->setFontSize(fontsize);
   this->ui->lbl_ESC_D_desc->setLabelText(QStringLiteral("电机4转速"));
   this->ui->lbl_ESC_D_value->setImage(":/image/control/control_05.png");
   this->ui->lbl_ESC_D_value->setFontSize(fontsize);


   this->ui->lbl_ESC_E_desc->setImage(":/image/control/control_01.png");
   this->ui->lbl_ESC_E_desc->setFontSize(fontsize);
   this->ui->lbl_ESC_E_desc->setLabelText(QStringLiteral("电机5转速"));
   this->ui->lbl_ESC_E_value->setImage(":/image/control/control_05.png");
   this->ui->lbl_ESC_E_value->setFontSize(fontsize);

   this->ui->lbl_ESC_F_desc->setImage(":/image/control/control_01.png");
   this->ui->lbl_ESC_F_desc->setFontSize(fontsize);
   this->ui->lbl_ESC_F_desc->setLabelText(QStringLiteral("电机6转速"));
   this->ui->lbl_ESC_F_value->setImage(":/image/control/control_05.png");
   this->ui->lbl_ESC_F_value->setFontSize(fontsize);


   this->ui->lbl_ESC_G_desc->setImage(":/image/control/control_01.png");
   this->ui->lbl_ESC_G_desc->setFontSize(fontsize);
   this->ui->lbl_ESC_G_desc->setLabelText(QStringLiteral("电机7转速"));
   this->ui->lbl_ESC_G_value->setImage(":/image/control/control_05.png");
   this->ui->lbl_ESC_G_value->setFontSize(fontsize);

   this->ui->lbl_ESC_H_desc->setImage(":/image/control/control_01.png");
   this->ui->lbl_ESC_H_desc->setFontSize(fontsize);
   this->ui->lbl_ESC_H_desc->setLabelText(QStringLiteral("电机8转速"));
   this->ui->lbl_ESC_H_value->setImage(":/image/control/control_05.png");
   this->ui->lbl_ESC_H_value->setFontSize(fontsize);

   this->ui->lbl_ESC_I_desc->setImage(":/image/control/control_01.png");
   this->ui->lbl_ESC_I_desc->setFontSize(fontsize);
   this->ui->lbl_ESC_I_desc->setLabelText(QStringLiteral("电机9转速"));
   this->ui->lbl_ESC_I_value->setImage(":/image/control/control_05.png");
   this->ui->lbl_ESC_I_value->setFontSize(fontsize);

   this->ui->lbl_ESC_J_desc->setImage(":/image/control/control_01.png");
   this->ui->lbl_ESC_J_desc->setFontSize(fontsize);
   this->ui->lbl_ESC_J_desc->setLabelText(QStringLiteral("电机10转速"));
   this->ui->lbl_ESC_J_value->setImage(":/image/control/control_05.png");
   this->ui->lbl_ESC_J_value->setFontSize(fontsize);

   this->ui->lbl_ESC_K_desc->setImage(":/image/control/control_01.png");
   this->ui->lbl_ESC_K_desc->setFontSize(fontsize);
   this->ui->lbl_ESC_K_desc->setLabelText(QStringLiteral("电机11转速"));
   this->ui->lbl_ESC_K_value->setImage(":/image/control/control_05.png");
   this->ui->lbl_ESC_K_value->setFontSize(fontsize);

   this->ui->lbl_ESC_L_desc->setImage(":/image/control/control_01.png");
   this->ui->lbl_ESC_L_desc->setFontSize(fontsize);
   this->ui->lbl_ESC_L_desc->setLabelText(QStringLiteral("电机12转速"));
   this->ui->lbl_ESC_L_value->setImage(":/image/control/control_05.png");
   this->ui->lbl_ESC_L_value->setFontSize(fontsize);

   this->ui->lbl_flymode_desc->setStyleSheet(strStyle);
   this->ui->lbl_flymode_value->setImage(":/image/control/control_04.png");
   this->ui->lbl_flymode_value->setFontSize(fontsize);

   this->ui->lbl_flytime_desc->setStyleSheet(strStyle);
   this->ui->lbl_flytime_value->setImage(":/image/control/control_04.png");
   this->ui->lbl_flytime_value->setFontSize(fontsize);


   this->ui->ll_east_desc->setStyleSheet(strStyle);
   this->ui->ll_east->setImage(":/image/control/control_04.png");
   this->ui->ll_east->setFontSize(fontsize);

   this->ui->ll_north_desc->setStyleSheet(strStyle);
   this->ui->ll_north->setImage(":/image/control/control_04.png");
   this->ui->ll_north->setFontSize(fontsize);

   this->ui->ll_height_desc->setStyleSheet(strStyle);
   this->ui->ll_height->setImage(":/image/control/control_04.png");
   this->ui->ll_height->setFontSize(fontsize);

   this->ui->lbl_roll_desc->setStyleSheet(strStyle);
   this->ui->ll_roll->setImage(":/image/control/control_04.png");
   this->ui->ll_roll->setFontSize(fontsize);

   this->ui->lbl_pitch_desc->setStyleSheet(strStyle);
   this->ui->ll_pitch->setImage(":/image/control/control_04.png");
   this->ui->ll_pitch->setFontSize(fontsize);

   this->ui->lbl_yaw_desc->setStyleSheet(strStyle);
   this->ui->ll_yaw->setImage(":/image/control/control_04.png");
   this->ui->ll_yaw->setFontSize(fontsize);

   this->ui->ll_HSpeed_desc->setStyleSheet(strStyle);
   this->ui->ll_Hspeed->setImage(":/image/control/control_04.png");
   this->ui->ll_Hspeed->setFontSize(fontsize);

   this->ui->lbl_Vspeed_desc->setStyleSheet(strStyle);
   this->ui->ll_Vspeed->setImage(":/image/control/control_04.png");
   this->ui->ll_Vspeed->setFontSize(fontsize);

   //新加label电机，旋翼，电调，备降电池
   this->ui->label_rotorremaintime_desc->setStyleSheet(strStyle);
   this->ui->label_rotorremaintime_value->setImage(":/image/control/control_04.png");
   this->ui->label_rotorremaintime_value->setFontSize(fontsize);

   this->ui->label_motorremaintime_desc->setStyleSheet(strStyle);
   this->ui->label_motorremaintime_value->setImage(":/image/control/control_04.png");
   this->ui->label_motorremaintime_value->setFontSize(fontsize);

   this->ui->label_batteryremaintime_desc->setStyleSheet(strStyle);
   this->ui->label_batteryremaintime_value->setImage(":/image/control/control_04.png");
   this->ui->label_batteryremaintime_value->setFontSize(fontsize);

   this->ui->label_eledebugremaintime_desc->setStyleSheet(strStyle);
   this->ui->label_eledebugremaintime_value->setImage(":/image/control/control_04.png");
   this->ui->label_eledebugremaintime_value->setFontSize(fontsize);

   m_PA_RotorRemainTime.checkValue(ZYGroundGlobalConfig::m_RotorRemainTime);
   this->ui->label_rotorremaintime_value->setLabelText(QString::number(ZYGroundGlobalConfig::m_RotorRemainTime));
   m_PA_MotorRemainTime.checkValue(ZYGroundGlobalConfig::m_MotorRemainTime);
   this->ui->label_motorremaintime_value->setLabelText(QString::number(ZYGroundGlobalConfig::m_MotorRemainTime));
   m_PA_EleDebugRemainTime.checkValue(ZYGroundGlobalConfig::m_EleDebugRemainTime);
   this->ui->label_eledebugremaintime_value->setLabelText(QString::number(ZYGroundGlobalConfig::m_EleDebugRemainTime));
   m_PA_BatteryRemainTime.checkValue(ZYGroundGlobalConfig::m_BatteryRemainTime);
   this->ui->label_batteryremaintime_value->setLabelText(QString::number(ZYGroundGlobalConfig::m_BatteryRemainTime));

    this->ui->lbl_flytime_value->setText(QString::number(ZYGroundGlobalConfig::m_flytime));
}




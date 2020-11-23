/*===================================================================
APM_PLANNER Open Source Ground Control Station

(c) 2013 APM_PLANNER PROJECT <http://www.diydrones.com>

This file is part of the APM_PLANNER project

    APM_PLANNER is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    APM_PLANNER is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with APM_PLANNER. If not, see <http://www.gnu.org/licenses/>.

======================================================================*/

#include "AccelCalibrationConfig.h"
#include "controllers/frmmaincontroller.h"
#include "connections/mavlinkmessageinterface.h"


const char* COUNTDOWN_STRING = "<h3>校准MAV%03d<br>超时时间剩余: <b>%d</b><h3>";

AccelCalibrationConfig::AccelCalibrationConfig(QWidget *parent) : QWidget(parent),
    m_muted(false),
    m_isCalibrating(false),
    m_countdownCount(CALIBRATION_TIMEOUT_SEC)
{
    ui.setupUi(this);
    connect(ui.calibrateAccelButton,SIGNAL(clicked()),this,SLOT(calibrateButtonClicked()));

    m_accelAckCount=0;
    //coutdownLabel
    connect(&m_countdownTimer,SIGNAL(timeout()),this,SLOT(countdownTimerTick()));

    makeConnection();
}

AccelCalibrationConfig::~AccelCalibrationConfig()
{

    m_countdownTimer.stop();

}
void AccelCalibrationConfig::countdownTimerTick()
{
    if(FrmMainController::Instance()->__vehicle!=NULL)
    {
        int uav_id=FrmMainController::Instance()->__vehicle->m_State.m_Id;
       // ui.coutdownLabel->setText((QString().sprintf(COUNTDOWN_STRING, uav_id, m_countdownCount--)).toLocal8Bit());
       QString tempStr=QString::fromLocal8Bit("<h3>校准MAV")+QString("%1").arg(uav_id)+QString::fromLocal8Bit("超时时间剩余: <b>")+QString("%1").arg(m_countdownCount--);
       ui.coutdownLabel->setText(tempStr);
       if (m_countdownCount <= 0)
        {
            ui.coutdownLabel->setText(QString::fromLocal8Bit("命令超时,请重新再试."));
            m_countdownTimer.stop();
            ui.calibrateAccelButton->setText(QString::fromLocal8Bit("继续\n加速度计"));
            m_accelAckCount = 0;
        }
    }
}

void AccelCalibrationConfig::makeConnection()
{
    connect(FrmMainController::Instance(),SIGNAL(textMessageReceived(int,int,int,QString)),this,SLOT(uasTextMessageReceived(int,int,int,QString)));
}

void AccelCalibrationConfig::uasConnected()
{

}

void AccelCalibrationConfig::uasDisconnected()
{

}

void AccelCalibrationConfig::calibrateButtonClicked()
{
//    if(!ZYGroundGlobalConfig::m_SerialIsOpen)
//    {
//        myHelper::ShowMessageBoxInfo("请先连接无人机，再进行校准");
//        return;
//    }

    ui.outputLabel->clear();

    m_isCalibrating = true; // this is to guard against showing unwanted GCS Text Messages.

    if (m_accelAckCount == 0)
    {

        MAV_CMD command = MAV_CMD_PREFLIGHT_CALIBRATION;
        int confirm = 0;
        float param1 = 0.0;
        float param2 = 0.0;
        float param3 = 0.0;
        float param4 = 0.0;
        float param5 = 1.0;
        float param6 = 0.0;
        float param7 = 0.0;
        int component = 1;
        FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.doCommand(command, param1, param2, param3, param4, param5, param6, param7);
        m_countdownCount = CALIBRATION_TIMEOUT_SEC;

        int uav_id=FrmMainController::Instance()->__vehicle->m_State.m_Id;
        //ui.coutdownLabel->setText(QString().sprintf(COUNTDOWN_STRING, uav_id, m_countdownCount--));
        QString tempStr=QString::fromLocal8Bit("<h3>校准MAV")+QString("%1").arg(uav_id)+QString::fromLocal8Bit("超时时间剩余: <b>")+QString("%1").arg(m_countdownCount--);
        ui.coutdownLabel->setText(tempStr);
        m_countdownTimer.start(1000);
    }
    else if (m_accelAckCount <= 10)
    {
        executeCommandAck(m_accelAckCount++,true);
        m_countdownCount = CALIBRATION_TIMEOUT_SEC;
    }
    else
    {
        executeCommandAck(m_accelAckCount++,true);
        ui.coutdownLabel->setText("");
        m_countdownTimer.stop();
        ui.calibrateAccelButton->setText(QString::fromLocal8Bit("校准\n加速度计"));
        if (m_accelAckCount > 8)
        {
            //We've clicked too many times! Reset.
            for (int i=0;i<8;i++)
            {
                executeCommandAck(i,true);
            }
            m_accelAckCount = 0;
        }
    }
}
void AccelCalibrationConfig::executeCommandAck(int num, bool success)
{
    mavlink_message_t msg;
    mavlink_command_ack_t ack;
    ack.command = num;
    ack.result = (success ? 1 : 0);
    mavlink_msg_command_ack_encode(1,1,&msg,&ack);
    if(FrmMainController::Instance()->__vehicle!=NULL)
    {
       FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.sendMessage(msg);
    }
}

void AccelCalibrationConfig::hideEvent(QHideEvent *evt)
{
    Q_UNUSED(evt);

    for (int i=m_accelAckCount;i<8;i++)
    {
        executeCommandAck(i,true); //Clear out extra commands.
    }
}
void AccelCalibrationConfig::uasTextMessageReceived(int uasid, int componentid, int severity, QString text)
{
    Q_UNUSED(uasid);
    Q_UNUSED(componentid);

    if ((severity == 5 /*SEVERITY_USER_RESPONSE*/)||(severity == 3 /*SEVERITY_HIGH*/))
    {
        //This is a calibration instruction
        if (!m_isCalibrating || text.startsWith("PreArm:") || text.startsWith("EKF") || text.startsWith("Arm"))
        {
            // Don't show these warning messages
            return;
        }
        if(text.toLower().contains("left"))
        {
             ui.lbl_Image->setStyleSheet("QLabel{border-image:url(:/image/calibration/accel_left.png)}");
             ui.outputLabel->setText(QString::fromLocal8Bit("请将飞机左侧朝下放置."));
        }
        if(text.toLower().contains("right"))
        {
            ui.lbl_Image->setStyleSheet("QLabel{border-image:url(:/image/calibration/accel_right.png)}");
            ui.outputLabel->setText(QString::fromLocal8Bit("请将飞机右侧朝下放置."));
        }
        if(text.toLower().contains("back"))
        {
            ui.lbl_Image->setStyleSheet("QLabel{border-image:url(:/image/calibration/accel_up.png)}");
            ui.outputLabel->setText(QString::fromLocal8Bit("请将飞机背朝上放置."));
        }
        if(text.toLower().contains("down"))
        {
            ui.lbl_Image->setStyleSheet("QLabel{border-image:url(:/image/calibration/accel_front.png)}");
            ui.outputLabel->setText(QString::fromLocal8Bit("请将飞机头朝下放置."));
        }
        if(text.toLower().contains("level"))
        {
              ui.lbl_Image->setStyleSheet("QLabel{border-image:url(:/image/calibration/accel_down.png)}");
              ui.outputLabel->setText(QString::fromLocal8Bit("请将飞机水平放置."));
        }
        if(text.toLower().contains("up"))
        {
             ui.lbl_Image->setStyleSheet("QLabel{border-image:url(:/image/calibration/accel_back.png)}");
             ui.outputLabel->setText(QString::fromLocal8Bit("请将飞机头朝上放置."));
        }

        if (text.contains("Place") && text.contains ("and press any key"))
        {
            //Instruction
            if (m_accelAckCount == 0)
            {
                //Calibration Sucessful\r"
                ui.calibrateAccelButton->setText(QString::fromLocal8Bit("继续\n点击空格"));
                ui.calibrateAccelButton->setShortcut(QKeySequence(Qt::Key_Space));
            }
            m_accelAckCount++;
        }
        else if (text.contains("Calibration successful") || text.contains("FAILED"))
        {
            //Calibration complete success or failure
            if(text.contains("Calibration successful"))
            {
                ui.outputLabel->setText(ui.outputLabel->text() + "\n" + QString::fromLocal8Bit("校准成功."));

                FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.doCommand(MAV_CMD_PREFLIGHT_STORAGE, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

            }
            if(text.contains("FAILED"))
            {
                ui.outputLabel->setText(ui.outputLabel->text() + "\n" + QString::fromLocal8Bit("校准失败."));

            }
            ui.coutdownLabel->setText("");
            m_countdownTimer.stop();

            m_accelAckCount = 0;
            ui.calibrateAccelButton->setText(QString::fromLocal8Bit("校准\n加速度计"));
            ui.calibrateAccelButton->setShortcut(QKeySequence());
            m_isCalibrating = false;
        }
        else
        {
            ui.outputLabel->setText(ui.outputLabel->text() + "\n" + text);
        }
    }

}


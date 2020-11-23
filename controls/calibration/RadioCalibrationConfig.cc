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

/**
 * @file
 *   @brief Radio Calibration Configuration source.
 *
 *   @author Michael Carpenter <malcom2073@gmail.com>
 *
 */

#include "RadioCalibrationConfig.h"
//#include "QsLog.h"
#include "designer/QGCMouseWheelEventFilter.h"
#include <QMessageBox>
#include <QSettings>
#include "controllers/frmmaincontroller.h"
#include "controllers/parametercontroller.h"
//#include "CompassConfig.h"
#include <QDebug>

#define QLOG_DEBUG() qDebug()
#define QLOG_ERROR() qDebug()

RadioCalibrationConfig::RadioCalibrationConfig(QWidget *parent) : QWidget(parent),
    m_pitchChannel(2),
    m_rollChannel(1),
    m_yawChannel(4),
    m_throttleChannel(3),
    m_pitchWidget(NULL),
    m_throttleWidget(NULL),
    m_pitchCheckBox(NULL),
    m_throttleCheckBox(NULL),
    m_rcMode(0)
{
    ui.setupUi(this);

   guiUpdateTimer=NULL;
    readSettings();

    connect(ui.calibrateButton,SIGNAL(clicked()),this,SLOT(calibrateButtonClicked()));
    m_calibrationEnabled = false;
    ui.rollWidget->setMin(800);
    ui.rollWidget->setMax(2200);
    ui.leftVWidget->setMin(800);
    ui.leftVWidget->setMax(2200);
    ui.rightVWidget->setMin(800);
    ui.rightVWidget->setMax(2200);
    ui.yawWidget->setMin(800);
    ui.yawWidget->setMax(2200);

    ui.rollWidget->setName(tr("Roll"));
    ui.yawWidget->setName(tr("Yaw"));

    ui.radio5Widget->setMin(800);
    ui.radio5Widget->setMax(2200);
    ui.radio6Widget->setMin(800);
    ui.radio6Widget->setMax(2200);
    ui.radio7Widget->setMin(800);
    ui.radio7Widget->setMax(2200);
    ui.radio8Widget->setMin(800);
    ui.radio8Widget->setMax(2200);

    ui.rollWidget->setOrientation(Qt::Horizontal);
    ui.yawWidget->setOrientation(Qt::Horizontal);

    ui.radio5Widget->setOrientation(Qt::Horizontal);
    ui.radio5Widget->setName("Radio 5");
    ui.radio6Widget->setOrientation(Qt::Horizontal);
    ui.radio6Widget->setName("Radio 6");
    ui.radio7Widget->setOrientation(Qt::Horizontal);
    ui.radio7Widget->setName("Radio 7");
    ui.radio8Widget->setOrientation(Qt::Horizontal);
    ui.radio8Widget->setName("Radio 8");

    guiUpdateTimer = new QTimer(this);
    connect(guiUpdateTimer,SIGNAL(timeout()),this,SLOT(guiUpdateTimerTick()));

    rcMin << 1100.0 << 1100.0 << 1100.0 << 1100.0 << 1100.0 << 1100.0 << 1100.0 << 1100.0;
    rcMax << 1900.0 << 1900.0 << 1900.0 << 1900.0 << 1900.0 << 1900.0 << 1900.0 << 1900.0;
    rcTrim << 1500.0 << 1500.0 << 1500.0 << 1500.0 << 1500.0 << 1500.0 << 1500.0 << 1500.0;
    rcValue << 0.0 << 0.0 << 0.0 << 0.0 << 0.0 << 0.0 << 0.0 << 0.0;

    ui.revLeftVCheckBox->hide();
    ui.revRollCheckBox->hide();
    ui.revRightVCheckBox->hide();
    ui.revYawCheckBox->hide();
    //ui.elevonConfigGroupBox->hide();

    ui.modeComboBox->insertItem(0, "Mode 1", 1);
    ui.modeComboBox->insertItem(1, "Mode 2", 2);
    ui.modeComboBox->insertItem(2, "Mode 3", 3);
    ui.modeComboBox->insertItem(3, "Mode 4", 4);

    // Disable scroll wheel from easily triggering settings change
    ui.modeComboBox->installEventFilter(QGCMouseWheelEventFilter::getFilter());
    ui.modeComboBox->setCurrentIndex(ui.modeComboBox->findData(m_rcMode));
    modeIndexChanged(ui.modeComboBox->currentIndex());

    initConnections();

    connect(ui.revRollCheckBox, SIGNAL(clicked(bool)), this, SLOT(rollClicked(bool)));
    connect(ui.revYawCheckBox, SIGNAL(clicked(bool)), this, SLOT(yawClicked(bool)));

    connect(ui.elevonCheckBox, SIGNAL(clicked(bool)), this, SLOT(elevonsChecked(bool)));
    connect(ui.elevonRevCheckBox, SIGNAL(clicked(bool)), this, SLOT(elevonsReversed(bool)));
    connect(ui.elevonCh1CheckBox, SIGNAL(clicked(bool)), this, SLOT(elevonsCh1Rev(bool)));
    connect(ui.elevonCh2CheckBox, SIGNAL(clicked(bool)), this, SLOT(elevonsCh2Rev(bool)));

    ui.elevonOutputComboBox->addItem("Disabled");
    ui.elevonOutputComboBox->addItem("Up Up");
    ui.elevonOutputComboBox->addItem("Up Down");
    ui.elevonOutputComboBox->addItem("Down Up");
    ui.elevonOutputComboBox->addItem("Down Down");
    connect(ui.elevonOutputComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(elevonOutput()));

    connect(ui.modeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(modeIndexChanged(int)));
    ui.label_3->setVisible(false);
    ui.modeComboBox->setVisible(false);
    //ui.radio5Widget->setVisible(false);
    ui.radio6Widget->setVisible(false);
    ui.radio7Widget->setVisible(false);
    ui.radio8Widget->setVisible(false);
    //ui.label_8->setVisible(false);
    ui.label_9->setVisible(false);
    ui.label_10->setVisible(false);
    ui.label_11->setVisible(false);
}

RadioCalibrationConfig::~RadioCalibrationConfig()
{
    if(NULL!=guiUpdateTimer)
    {
        guiUpdateTimer->stop();
        delete guiUpdateTimer;
        guiUpdateTimer=NULL;
    }
}
void RadioCalibrationConfig::initConnections()
{

    connect(FrmMainController::Instance(),SIGNAL(remoteControlChannelRawChanged(int,uint16_t)),this,SLOT(remoteControlChannelRawChanged(int,uint16_t)));
    connect(ParameterController::Instance(),SIGNAL(OnParameterChanged(int,int,QString,QVariant)),this,SLOT(parameterChanged(int,int,QString,QVariant)));
//    if (m_uas->isFixedWing() || m_uas->isGroundRover()){
//        ui.revLeftVCheckBox->show();
//        ui.revRollCheckBox->show();
//        ui.revRightVCheckBox->show();
//        ui.revYawCheckBox->show();
//        ui.elevonConfigGroupBox->show();
//    } else {
        ui.revLeftVCheckBox->hide();
        ui.revRollCheckBox->hide();
        ui.revRightVCheckBox->hide();
        ui.revYawCheckBox->hide();
        //ui.elevonConfigGroupBox->hide();
//    }
}
void RadioCalibrationConfig::remoteControlChannelRawChanged(int chan,uint16_t val)
{

    //qDebug()<<"remoteControlChannelRawChanged(int chan,uint16_t val):"<<val;
    //Channel is 0-7 typically?
    //Val will be 0-3000, PWM value.
    if (m_calibrationEnabled) {
        if (val < rcMin[chan])
        {
            rcMin[chan] = val;
        }

        if (val > rcMax[chan])
        {
            rcMax[chan] = val;
        }
    }

    // Raw value
    rcValue[chan] = val;
}

void RadioCalibrationConfig::modeIndexChanged(int index)
{
    Q_UNUSED(index);

    if(m_pitchCheckBox || m_throttleCheckBox){
        disconnect(m_pitchCheckBox, SIGNAL(clicked(bool)), this, SLOT(pitchClicked(bool)));
        disconnect(m_throttleCheckBox, SIGNAL(clicked(bool)), this, SLOT(throttleClicked(bool)));
    }

    if((ui.modeComboBox->currentData() == 1)
        ||(ui.modeComboBox->currentData() == 3)){
        // Mode 1 & 3 (Throttle on right) (Pitch/Aileron left)
        m_throttleWidget = ui.rightVWidget;
        m_throttleCheckBox = ui.revRightVCheckBox;
        m_pitchWidget = ui.leftVWidget;
        m_pitchCheckBox = ui.revLeftVCheckBox;
    } else {
        // Mode 2 & 4 (Throttle on left) (Pitch/Aileron right)
        m_throttleWidget = ui.leftVWidget;
        m_throttleCheckBox = ui.revLeftVCheckBox;
        m_pitchWidget = ui.rightVWidget;
        m_pitchCheckBox = ui.revRightVCheckBox;
    }
    if ((m_pitchChannel != 0 ) && (m_pitchChannel != 0 )){
        m_pitchWidget->setName(tr("%1-Pitch").arg(m_pitchChannel));
        m_throttleWidget->setName(tr("%1-Throttle").arg(m_throttleChannel));
    } else {
        m_pitchWidget->setName(tr("Pitch"));
        m_throttleWidget->setName(tr("Throttle"));
    }

    connect(m_pitchCheckBox, SIGNAL(clicked(bool)), this, SLOT(pitchClicked(bool)));
    connect(m_throttleCheckBox, SIGNAL(clicked(bool)), this, SLOT(throttleClicked(bool)));

    m_rcMode = ui.modeComboBox->currentData().toInt();
    writeSettings();
}

void RadioCalibrationConfig::parameterChanged(int uas, int component, QString parameterName, QVariant value)
{
    Q_UNUSED(uas);
    Q_UNUSED(component);

    if(parameterName.startsWith("RCMAP_PITCH")){
        //qDebug()<<parameterName<<":"<<value;
        m_pitchChannel = value.toInt();
        m_pitchWidget->setName(tr("%1-Pitch").arg(m_pitchChannel));
        return;

    } else if(parameterName.startsWith("RCMAP_ROLL")){
       // qDebug()<<parameterName<<":"<<value;
        m_rollChannel = value.toInt();
        ui.rollWidget->setName(tr("%1-Roll").arg(value.toString()));
        return;

    } else if(parameterName.startsWith("RCMAP_YAW")){
        //qDebug()<<parameterName<<":"<<value;
        m_yawChannel = value.toInt();
        ui.yawWidget->setName(tr("%1-Yaw").arg(value.toString()));
        return;

    } else if(parameterName.startsWith("RCMAP_THROTTLE")){
        //qDebug()<<parameterName<<":"<<value;
        m_throttleChannel = value.toInt();
        m_throttleWidget->setName(tr("%1-Throttle").arg(m_throttleChannel));
        return;
    }

    if (parameterName.startsWith("ELEVON_MIXING")){
        ui.elevonCheckBox->setChecked(value.toBool());
    } else if (parameterName.startsWith("ELEVON_REVERSE")){
        ui.elevonRevCheckBox->setChecked(value.toBool());
    } else if (parameterName.startsWith("ELEVON_CH1_REV")){
        ui.elevonCh1CheckBox->setChecked(value.toBool());
    } else if (parameterName.startsWith("ELEVON_CH2_REV")){
        ui.elevonCh2CheckBox->setChecked(value.toBool());
    } else if (parameterName.startsWith("ELEVON_OUTPUT")){
        ui.elevonOutputComboBox->setCurrentIndex(value.toInt());
    }
}

void RadioCalibrationConfig::updateChannelReversalStates()
{
    // Update Pitch Reverse Channel
    updateChannelRevState(m_pitchCheckBox, m_pitchChannel);

    // Update Roll Reverse Channel
    updateChannelRevState(ui.revRollCheckBox, m_rollChannel);

    // Update Yaw Reverse Channel
    updateChannelRevState(ui.revYawCheckBox, m_yawChannel);

    // Update Throttle Reverse Channel
    updateChannelRevState(m_throttleCheckBox, m_throttleChannel);
}

void RadioCalibrationConfig::updateChannelRevState(QCheckBox* checkbox, int channelId)
{
//    int reverse = m_uas->getParamManager()->getParameterValue(1, "RC" + QString::number(channelId)
//                                                             + "_REV").toInt();
    ParamData t_data;
  char* argName= QString::asprintf("RC%s_REV",QString::number(channelId)).toLocal8Bit().data();
    _queryParam(t_data,argName);//need fixed xjw???
    int reverse=t_data.value.toInt();
    if (reverse == -1) {
        checkbox->setChecked(true);
    } else {
        checkbox->setChecked(false);
    }
}

void RadioCalibrationConfig::guiUpdateTimerTick()
{
    //qDebug()<<"m_rollChannel=="<<m_rollChannel<<" || m_pitchChannel=="<<m_pitchChannel<<" || m_throttleChannel=="<<m_throttleChannel<<" || m_yawChannel=="<<m_yawChannel;
    if (m_rollChannel==0 || m_pitchChannel==0 || m_throttleChannel==0 || m_yawChannel==0){
        // If not all parameters have been downloaded, don't update RC channels
        // which results in out-of-range array access.
        return;
    }

    ui.rollWidget->setValue(rcValue[m_rollChannel-1]);
    m_pitchWidget->setValue(rcValue[m_pitchChannel-1]);
    m_throttleWidget->setValue(rcValue[m_throttleChannel-1]);
    ui.yawWidget->setValue(rcValue[m_yawChannel-1]);
    ui.radio5Widget->setValue(rcValue[4]);
    ui.radio6Widget->setValue(rcValue[5]);
    ui.radio7Widget->setValue(rcValue[6]);
    ui.radio8Widget->setValue(rcValue[7]);
    if (m_calibrationEnabled)
    {
        ui.rollWidget->setMin(rcMin[m_rollChannel-1]);
        ui.rollWidget->setMax(rcMax[m_rollChannel-1]);
        m_pitchWidget->setMin(rcMin[m_pitchChannel-1]);
        m_pitchWidget->setMax(rcMax[m_pitchChannel-1]);
        m_throttleWidget->setMin(rcMin[m_throttleChannel-1]);
        m_throttleWidget->setMax(rcMax[m_throttleChannel-1]);
        ui.yawWidget->setMin(rcMin[m_yawChannel-1]);
        ui.yawWidget->setMax(rcMax[m_yawChannel-1]);
        ui.radio5Widget->setMin(rcMin[4]);
        ui.radio5Widget->setMax(rcMax[4]);
        ui.radio6Widget->setMin(rcMin[5]);
        ui.radio6Widget->setMax(rcMax[5]);
        ui.radio7Widget->setMin(rcMin[6]);
        ui.radio7Widget->setMax(rcMax[6]);
        ui.radio8Widget->setMin(rcMin[7]);
        ui.radio8Widget->setMax(rcMax[7]);
    }
    if(rcValue[4] <= 500)
    {
        ui.label_zw->setStyleSheet("background-color: rgb(255, 0, 0);");
        ui.label_dg->setStyleSheet("background-color: rgb(85, 170, 0);");
        ui.label_dd->setStyleSheet("background-color: rgb(85, 170, 0);");
    }
    else if(rcValue[4] > 500 && rcValue[4] < 1200)
    {
        ui.label_zw->setStyleSheet("background-color: rgb(85, 170, 0);");
        ui.label_dg->setStyleSheet("background-color: rgb(255, 0, 0);");
        ui.label_dd->setStyleSheet("background-color: rgb(85, 170, 0);");
    }
    else if(rcValue[4] >= 1200)
    {
        ui.label_zw->setStyleSheet("background-color: rgb(85, 170, 0);");
        ui.label_dg->setStyleSheet("background-color: rgb(85, 170, 0);");
        ui.label_dd->setStyleSheet("background-color: rgb(255, 0, 0);");
    }


    ui.ll_rollValue->setText(QString::number(((float)(rcValue[m_rollChannel-1] - rcMin[m_rollChannel - 1])/(float)(rcMax[m_rollChannel-1] - rcMin[m_rollChannel - 1]))*2-1,'g',2));
    ui.ll_pitchValue->setText(QString::number(((float)(rcValue[m_pitchChannel-1] - rcMin[m_pitchChannel - 1])/(float)(rcMax[m_pitchChannel-1] - rcMin[m_pitchChannel - 1]))*2-1,'g',2));
    ui.ll_throttleValue->setText(QString::number(((float)(rcValue[m_throttleChannel-1] - rcMin[m_throttleChannel - 1])/(float)(rcMax[m_throttleChannel-1] - rcMin[m_throttleChannel - 1]))*2-1,'g',2));
    ui.ll_yawValue->setText(QString::number(((float)(rcValue[m_yawChannel-1] - rcMin[m_yawChannel - 1])/(float)(rcMax[m_yawChannel-1] - rcMin[m_yawChannel - 1]))*2-1,'g',2));
    updateChannelReversalStates();
}
void RadioCalibrationConfig::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    guiUpdateTimer->start(100);
}
void RadioCalibrationConfig::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);
    guiUpdateTimer->stop();
}
void RadioCalibrationConfig::calibrateButtonClicked()
{
    if (!isRadioControlActive()){
        QMessageBox::warning(this,QString::fromLocal8Bit("遥控器"), QString::fromLocal8Bit("遥控器没有激活或者打开"));
        return;
    }

    if (!m_calibrationEnabled)
    {
        if (QMessageBox::question(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("您即将开始遥控器的校准.\n请确保所有的马达电源都断开并且所有的属性都从无人机上移除.\n更要确保遥控器传输模块的连接正常\n\n点击确定进行确认, 或者终止来取消校准"),QMessageBox::Ok,QMessageBox::Cancel) != QMessageBox::Ok)
        {
            QMessageBox::information(this,QString::fromLocal8Bit("警告!"),QString::fromLocal8Bit("校准终止"));
            return;
        }
        ui.calibrateButton->setText(QString::fromLocal8Bit("校准完成"));
        int nf = 100;
        float f;
        memcpy(&f, (void*)&nf, sizeof(int));
        FrmMainController::Instance()->set_parameter((char*)"DL_MSG_GRP3_MS", f,MAV_PARAM_TYPE_INT32);
        m_calibrationEnabled = true;
        for (int i=0;i<RC_CHANNEL_NUM_MAX;i++)
        {
            rcMin[i] = 1500;
            rcMax[i] = 1500;
        }
        ui.rollWidget->showMinMax();
        m_pitchWidget->showMinMax();
        ui.yawWidget->showMinMax();
        ui.radio5Widget->showMinMax();
        ui.radio6Widget->showMinMax();
        ui.radio7Widget->showMinMax();
        m_throttleWidget->showMinMax();
        ui.radio8Widget->showMinMax();
        QMessageBox::information(this,QString::fromLocal8Bit("信息"),QString::fromLocal8Bit("点击确定, 然后移动所有的杆到极限位置, 查看最小最大值来确保您获得最大范围的控制器. 这将包括所有的开关"));
    }
    else
    {
        ui.calibrateButton->setText(QString::fromLocal8Bit("校准"));
        QMessageBox::information(this,QString::fromLocal8Bit("调整"),QString::fromLocal8Bit("确保所有的杆子位于中间位置并且油门在最小位置, 点击确定以继续"));

        int nf = 1000;
        float f;
        memcpy(&f, (void*)&nf, sizeof(int));
        FrmMainController::Instance()->set_parameter((char*)"DL_MSG_GRP3_MS", f,MAV_PARAM_TYPE_INT32);
        m_calibrationEnabled = false;
        ui.rollWidget->hideMinMax();
        m_pitchWidget->hideMinMax();
        ui.yawWidget->hideMinMax();
        ui.radio5Widget->hideMinMax();
        ui.radio6Widget->hideMinMax();
        m_throttleWidget->hideMinMax();
        ui.radio7Widget->hideMinMax();
        ui.radio8Widget->hideMinMax();

        //Send calibrations.
        QString minTpl("RC%1_MIN");
        QString maxTpl("RC%1_MAX");
        QString trimTpl("RC%1_TRIM");

        QString statusstr;
        statusstr = QString::fromLocal8Bit("以下您将要看到检测到的遥控器信息\n");
//        statusstr += QString::fromLocal8Bit("正常值应该在1100到1900之间, 修改关闭到1500\n\n");
        statusstr += "Channel\tMin\tCenter\tMax\n";
        statusstr += "--------------------\n";

        for (int i=0;i< RC_CHANNEL_NUM_MAX;i++)
        {
            statusstr += QString::number(i+1) + "\t" + QString::number(rcMin[i]) + "\t" + QString::number(rcValue[i]) + "\t" + QString::number(rcMax[i]) + "\n";
        }

        if (validRadioSettings()){
            for (int i=0;i< RC_CHANNEL_NUM_MAX;i++)
            {
                QLOG_DEBUG() << "SENDING MIN" << minTpl.arg(i+1) << rcMin[i];
                QLOG_DEBUG() << "SENDING TRIM" << trimTpl.arg(i+1) << rcValue[i];
                QLOG_DEBUG() << "SENDING MAX" << maxTpl.arg(i+1) << rcMax[i];

                // Send Calibrations
//                m_uas->getParamManager()->setParameter(1, minTpl.arg(i+1), rcMin[i]);
//                m_uas->getParamManager()->setParameter(1, trimTpl.arg(i+1), rcValue[i]); // Save the Trim Values.
//                m_uas->getParamManager()->setParameter(1, maxTpl.arg(i+1), rcMax[i]);
                ParameterController::Instance()->set_parameter_noanswer(minTpl.arg(i+1).toLocal8Bit().data(),rcMin[i],(MAV_PARAM_TYPE)1);
                QThread::sleep(1);
                ParameterController::Instance()->set_parameter_noanswer(trimTpl.arg(i+1).toLocal8Bit().data(),rcValue[i],(MAV_PARAM_TYPE)1);
                QThread::sleep(1);
                ParameterController::Instance()->set_parameter_noanswer(maxTpl.arg(i+1).toLocal8Bit().data(),rcMax[i],(MAV_PARAM_TYPE)1);
                QThread::sleep(1);
            }
            QMessageBox::information(this,QString::fromLocal8Bit("状态"),statusstr); // Show Calibraitions to the user
        } else {
            QMessageBox::warning(this,QString::fromLocal8Bit("状态"),QString::fromLocal8Bit("失败: 无效的PWM信号\n") + statusstr);
        }


        ui.rollWidget->setMin(800);
        ui.rollWidget->setMax(2200);
        m_pitchWidget->setMin(800);
        m_pitchWidget->setMax(2200);
        m_throttleWidget->setMin(800);
        m_throttleWidget->setMax(2200);
        ui.yawWidget->setMin(800);
        ui.yawWidget->setMax(2200);
        ui.radio5Widget->setMin(800);
        ui.radio5Widget->setMax(2200);
        ui.radio6Widget->setMin(800);
        ui.radio6Widget->setMax(2200);
        ui.radio7Widget->setMin(800);
        ui.radio7Widget->setMax(2200);
        ui.radio8Widget->setMin(800);
        ui.radio8Widget->setMax(2200);

    }
}

void RadioCalibrationConfig::pitchClicked(bool state)
{
    setParamChannelRev("RCMAP_PITCH", state);
}

void RadioCalibrationConfig::rollClicked(bool state)
{
    setParamChannelRev("RCMAP_ROLL", state);
}

void RadioCalibrationConfig::yawClicked(bool state)
{
    setParamChannelRev("RCMAP_YAW",state);
}

void RadioCalibrationConfig::throttleClicked(bool state)
{
    setParamChannelRev("RCMAP_THROTTLE", state);
}

void RadioCalibrationConfig::setParamChannelRev(const QString& param, bool state)
{

        //int channel = m_uas->getParamManager()->getParameterValue(1, param).toInt();
    ParamData t_data;
    char* argName= param.toLocal8Bit().data();
    _queryParam(t_data,argName);//need fixed xjw???

        QString channelString = QString("RC" + t_data.value + "_REV");
        if (state)
//            m_uas->setParameter(1, channelString, -1.0);
            ParameterController::Instance()->set_parameter_noanswer(channelString.toLocal8Bit().data(),-1.0,(MAV_PARAM_TYPE)9);
        else
            //m_uas->setParameter(1, channelString, 1.0); // We use 0 as the default, not 1.0 (which you can also use)
            ParameterController::Instance()->set_parameter_noanswer(channelString.toLocal8Bit().data(),1.0,(MAV_PARAM_TYPE)9);
}

void RadioCalibrationConfig::elevonsChecked(bool state)
{
    //if(m_uas){
        if (state)
            ParameterController::Instance()->set_parameter_noanswer((char*)"ELEVON_MIXING", 1.0,(MAV_PARAM_TYPE)9);
        else
            ParameterController::Instance()->set_parameter_noanswer((char*)"ELEVON_MIXING", 0.0,(MAV_PARAM_TYPE)9);
        //}
}

void RadioCalibrationConfig::elevonsReversed(bool state)
{
    //if(m_uas){
        if (state)
            //m_uas->setParameter(1, "ELEVON_REVERSE", 1.0);
         ParameterController::Instance()->set_parameter_noanswer((char*)"ELEVON_REVERSE", 1.0,(MAV_PARAM_TYPE)9);
        else
            ParameterController::Instance()->set_parameter_noanswer((char*)"ELEVON_REVERSE", 0.0,(MAV_PARAM_TYPE)9);
            //m_uas->setParameter(1, "ELEVON_REVERSE", 0.0);
//        }
}

void RadioCalibrationConfig::elevonsCh1Rev(bool state)
{
    setParamChannelRev("ELEVON_CH1_REV", state);
}

void RadioCalibrationConfig::elevonsCh2Rev(bool state)
{
    setParamChannelRev("ELEVON_CH2_REV", state);
}

void RadioCalibrationConfig::elevonOutput()
{
//    if(m_uas){
        ParameterController::Instance()->set_parameter_noanswer((char*)"ELEVON_OUTPUT", ui.elevonOutputComboBox->currentIndex(),(MAV_PARAM_TYPE)1);
//    }
}

void RadioCalibrationConfig::readSettings()
{
    QSettings settings;
    settings.beginGroup("RCCALIBRATION_VIEW");
    m_rcMode = settings.value("RC_MODE",2).toInt();
    settings.endGroup();
}

void RadioCalibrationConfig::writeSettings()
{
    QSettings settings;
    settings.beginGroup("RCCALIBRATION_VIEW");
    settings.setValue("RC_MODE",m_rcMode);
    settings.endGroup();
    settings.sync();
}

bool RadioCalibrationConfig::isRadioControlActive()
{
    // Check the lower 4 channels are active for radio connected.
    for(int count=0; count < RC_CHANNEL_LOWER_CONTROL_CH_MAX; count++){
        // Any invalid range and we abort.
        if (!isInRange(rcValue[count], RC_CHANNEL_PWM_MIN, RC_CHANNEL_PWM_MAX)){
            QLOG_ERROR() << QString().sprintf("isRadioControlActive: Error Channel %d out of range: rcValue=%f",
                                              count+1, rcValue[count]);
            return false;
        }
    }

//    for(int count=RC_CHANNEL_LOWER_CONTROL_CH_MAX; count < RC_CHANNEL_NUM_MAX; count++){
//        if ((rcValue[count]>0.0)){ // Only active channels are validated.
//            if (!isInRange(rcValue[count], RC_CHANNEL_PWM_MIN, RC_CHANNEL_PWM_MAX)){
//                QLOG_ERROR() << QString().sprintf("isRadioControlActive: Error Channel %d out of range: rcValue=%f",
//                                                  count+1, rcValue[count]);
//                return false;
//            }
//        }
//    }
    return true;
}

bool RadioCalibrationConfig::validRadioSettings()
{
    // Check lower 4 channels have been set correctly zero values not allowed.
    // i.e. Aileron (Roll), Elevator (Pitch), Throttle, Rudder (Yaw)
    for(int count=0; count< RC_CHANNEL_LOWER_CONTROL_CH_MAX; count++){
        // Any invalid range and we abort.
        if (!isInRange(rcMin[count], RC_CHANNEL_PWM_MIN, RC_CHANNEL_PWM_MAX)
                ||!isInRange(rcMax[count], RC_CHANNEL_PWM_MIN, RC_CHANNEL_PWM_MAX)
                ||!isInRange(rcTrim[count], RC_CHANNEL_PWM_MIN, RC_CHANNEL_PWM_MAX)){
            QLOG_ERROR() << QString().sprintf("validRadioSettings: Error Channel %d out of range: rcMin=%f rcMax=%f rcTrim=%f",
                                              count+1, rcMin[count], rcMax[count], rcTrim[count]);
            return false;
        }
    }

    // for channels other than the lower 4 we verify only if non-zero.
    for(int count=RC_CHANNEL_LOWER_CONTROL_CH_MAX; count< RC_CHANNEL_NUM_MAX; count++){
        // Only check if we have received a non-zero value on the channel
        // that the settings are valid.
        if ((rcValue[count]>0.0)){
            // Any invalid range and we abort.
            if (!isInRange(rcMin[count], RC_CHANNEL_PWM_MIN, RC_CHANNEL_PWM_MAX)
                ||!isInRange(rcMax[count], RC_CHANNEL_PWM_MIN, RC_CHANNEL_PWM_MAX)
                ||!isInRange(rcTrim[count], RC_CHANNEL_PWM_MIN, RC_CHANNEL_PWM_MAX)){
                QLOG_ERROR() << QString().sprintf("validRadioSettings: Error Channel %d out of range: rcMin=%f rcMax=%f rcTrim=%f",
                                                  count+1, rcMin[count], rcMax[count], rcTrim[count]);
                return false;
            }
        }
    }

    return true;
}

bool RadioCalibrationConfig::isInRange(double value, double min, double max)
{
    if ((value > min)&&(value<max)){
        return true;
    }
    return false;
}

void RadioCalibrationConfig::_queryParam(ParamData& param,char* argName)
{
    QList<ParamData>* temp= SettingManager::Instance()->getFullParameters();
    for(int i=0;i<temp->size();i++)
    {
          if(temp->at(i).key==QString::fromLocal8Bit(argName))
          {
            param=temp->at(i);
          }
    }
    param.count=0;
}

bool RadioCalibrationConfig::_ParamExist(char* argName)
{
    QList<ParamData>* temp= SettingManager::Instance()->getFullParameters();
    for(int i=0;i<temp->size();i++)
    {
          if(temp->at(i).key==QString::fromLocal8Bit(argName))
          {
            return true;
          }
    }
     return false;
}

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

//#include "QsLog.h"
#include "CompassConfig.h"
#include "CompassMotorCalibrationDialog.h"
#include <qmath.h>
#include "controllers/frmmaincontroller.h"
#include "controllers/parametercontroller.h"



CompassConfig::CompassConfig(QWidget *parent) : QWidget(parent),
    m_progressDialog(NULL),
    m_timer(NULL),
    m_calibratingCompass(false),
    m_compatibilityMode(false),
    m_haveSecondCompass(false),
    m_haveThirdCompass(false),
    m_avgSamples(0.0),
    m_rad(0.0)
{
    ui.setupUi(this);

    QList<QWidget*> widgetList = this->findChildren<QWidget*>();
    for (int i=0;i<widgetList.size();i++)
    {
        if (qobject_cast<QComboBox*>(widgetList[i]) || qobject_cast<QAbstractSpinBox*>(widgetList[i]) || qobject_cast<QAbstractSlider*>(widgetList[i]))
        {
          //  widgetList[i]->installEventFilter(QGCMouseWheelEventFilter::getFilter());
        }
    }

    ui.autoDecCheckBox->setEnabled(false);
    ui.enableCheckBox->setEnabled(false);
    ui.orientationComboBox->setEnabled(false);
    ui.degreesLineEdit->setEnabled(false);
    ui.minutesLineEdit->setEnabled(false);
    connect(ui.enableCheckBox,SIGNAL(clicked(bool)),this,SLOT(enableClicked(bool)));
    connect(ui.autoDecCheckBox,SIGNAL(clicked(bool)),this,SLOT(autoDecClicked(bool)));
    connect(ui.orientationComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(orientationComboChanged(int)));
    connect(ui.degreesLineEdit,SIGNAL(editingFinished()),this,SLOT(degreeEditFinished()));
    connect(ui.minutesLineEdit,SIGNAL(editingFinished()),this,SLOT(degreeEditFinished()));

    ui.orientationComboBox->addItem("None 0");
    ui.orientationComboBox->addItem("Yaw 45");
    ui.orientationComboBox->addItem("Yaw 90");
    ui.orientationComboBox->addItem("Yaw 135");
    ui.orientationComboBox->addItem("Yaw 180");
    ui.orientationComboBox->addItem("Yaw 225");
    ui.orientationComboBox->addItem("Yaw 270");
    ui.orientationComboBox->addItem("Yaw 315");
    ui.orientationComboBox->addItem("Roll 180");
    ui.orientationComboBox->addItem("Roll 180, Yaw 45");
    ui.orientationComboBox->addItem("Roll 180, Yaw 90");
    ui.orientationComboBox->addItem("Roll 180, Yaw 135");
    ui.orientationComboBox->addItem("Pitch 180");
    ui.orientationComboBox->addItem("Roll 180, Yaw 225");
    ui.orientationComboBox->addItem("Roll 180, Yaw 270");
    ui.orientationComboBox->addItem("Roll 180, Yaw 315");
    ui.orientationComboBox->addItem("Roll 90");
    ui.orientationComboBox->addItem("Roll 90, Yaw 45");
    ui.orientationComboBox->addItem("Roll 90, Yaw 90");
    ui.orientationComboBox->addItem("Roll 90, Yaw 135");
    ui.orientationComboBox->addItem("Roll 270");
    ui.orientationComboBox->addItem("Roll 270, Yaw 45");
    ui.orientationComboBox->addItem("Roll 270, Yaw 90");
    ui.orientationComboBox->addItem("Roll 270, Yaw 135");
    ui.orientationComboBox->addItem("Pitch 90");
    ui.orientationComboBox->addItem("Pitch 270");
//    ui.orientationComboBox->addItem("MAX");

    //initConnections();

    connect(ui.liveCalibrationButton, SIGNAL(clicked()),
            this, SLOT(liveCalibrationClicked()));

    connect(ui.onBoardApmButton, SIGNAL(clicked()),
            this,SLOT(setCompassAPMOnBoard()));
    connect(ui.gpsCompassButton, SIGNAL(clicked()),
            this,SLOT(setCompass3DRGPS()));
    connect(ui.px4Button, SIGNAL(clicked()),
            this,SLOT(setCompassPX4OnBoard()));

    connect(ui.compassMotButton, SIGNAL(clicked()), this, SLOT(showCompassMotorCalibrationDialog()));

    readSettings();

    //----------------------//
  connect(&FrmMainController::Instance()->__vehicle->mavLinkMessageInterface,
          SIGNAL(calibrate_raw_imu_recved(mavlink_raw_imu_t)),
          this,
          SLOT(rawImuMessageUpdate(mavlink_raw_imu_t)));
  connect(&FrmMainController::Instance()->__vehicle->mavLinkMessageInterface,
          SIGNAL(calibrate_scale_imu_recved(mavlink_scaled_imu_t)),
          this,
          SLOT(scaledImu2MessageUpdate(mavlink_scaled_imu2_t)));

}

void CompassConfig::readSettings()
{
    // Load defaults from settings
    QSettings settings;
    settings.beginGroup("COMPASS");
    m_compatibilityMode = settings.value("COMPATIBILITY_MODE", false).toBool();
    settings.endGroup();
    settings.sync();
}

void CompassConfig::writeSettings()
{
    // Load defaults from settings
    QSettings settings;
    settings.beginGroup("COMPASS");
    settings.setValue("COMPATIBILITY_MODE", m_compatibilityMode);
    settings.endGroup();
    settings.sync();
}

//void CompassConfig::activeUASSet(UASInterface *uas)
//{
//    AP2ConfigWidget::activeUASSet(uas);
//}

void CompassConfig::degreeEditFinished()
{
//    if (!m_uas)
//    {
//        showNullMAVErrorMessageBox();
//        return;
//    }
    //float degrees = value.toDouble() * (float)(180.0 / M_PI);
    //float minutes  = (degrees - ((int)degreees)) * 60.0;
    bool degok = false;
    bool minok = false;
    double degrees = ui.degreesLineEdit->text().toDouble(&degok);
    double minutes = ui.minutesLineEdit->text().toDouble(&minok);
    if (degrees < 0)
    {
        degrees = degrees - (minutes / 60.0);
    }
    else
    {
        degrees = degrees + (minutes / 60.0);
    }
    degrees = degrees * (M_PI/180.0);
    //m_uas->getParamManager()->setParameter(1,"COMPASS_DEC",(float)degrees);
    ParameterController::Instance()->set_parameter((char*)"COMPASS_DEC",(float)degrees,(MAV_PARAM_TYPE)9);
    if (!degok || !minok)
    {
        QMessageBox::information(this,tr("Error"),tr("Error, degrees or minutes entered are nor in valid numeric format. Please re-enter the information"));
        return;
    }
}

CompassConfig::~CompassConfig()
{
    writeSettings();
    cleanup();
    delete m_timer;
    delete m_progressDialog;
}

void CompassConfig::updateCompassSelection()
{
    ui.savedLabel->setText(tr("UPDATED"));
    QTimer::singleShot(2000,ui.savedLabel,SLOT(clear()));
}

void CompassConfig::parameterChanged(int uas, int component, QString parameterName, QVariant value)
{
    Q_UNUSED(uas);
    Q_UNUSED(component);
    if (parameterName == "MAG_ENABLE")
    {
        if (value.toInt() == 0)
        {
            ui.enableCheckBox->setChecked(false);
            ui.autoDecCheckBox->setEnabled(false);
            ui.degreesLineEdit->setEnabled(false);
            ui.minutesLineEdit->setEnabled(false);
            ui.orientationComboBox->setEnabled(false);
        }
        else
        {
            ui.enableCheckBox->setChecked(true);
            ui.autoDecCheckBox->setEnabled(true);
            ui.degreesLineEdit->setEnabled(true);
            ui.minutesLineEdit->setEnabled(true);
            ui.orientationComboBox->setEnabled(true);
        }
        ui.enableCheckBox->setEnabled(true);
    }
    else if (parameterName == "COMPASS_AUTODEC")
    {
        if (value.toInt() == 0)
        {
            ui.autoDecCheckBox->setChecked(false);
        }
        else
        {
            ui.autoDecCheckBox->setChecked(true);
        }
    }
    else if (parameterName == "COMPASS_DEC")
    {
        float degrees = value.toDouble() * (float)(180.0 / M_PI);
        float minutes  = (degrees - ((int)degrees)) * 60.0;
        if (degrees < 0)
        {
            minutes = minutes * -1;
        }
        ui.degreesLineEdit->setText(QString::number((int)degrees));
        ui.minutesLineEdit->setText(QString::number(minutes,'f',0));
    }
    else if (parameterName == "COMPASS_ORIENT")
    {
        ui.orientationComboBox->blockSignals(true);
        ui.orientationComboBox->setCurrentIndex(value.toInt());
        ui.orientationComboBox->blockSignals(false);
        updateCompassSelection();
    }
}

void CompassConfig::enableClicked(bool enabled)
{
//    if (m_uas)
//    {
        if (enabled)
        {
            ParameterController::Instance()->set_parameter_noanswer((char*)"MAG_ENABLE",1,(MAV_PARAM_TYPE)1);
            //m_uas->getParamManager()->setParameter(1,"MAG_ENABLE",QVariant(1));
            ui.autoDecCheckBox->setEnabled(true);
            if (!ui.autoDecCheckBox->isChecked())
            {
                ui.minutesLineEdit->setEnabled(true);
                ui.degreesLineEdit->setEnabled(true);
            }
        }
        else
        {
           // m_uas->getParamManager()->setParameter(1,"MAG_ENABLE",QVariant(0));
            ParameterController::Instance()->set_parameter_noanswer((char*)"MAG_ENABLE",0,(MAV_PARAM_TYPE)1);
            ui.autoDecCheckBox->setEnabled(false);
            ui.degreesLineEdit->setEnabled(false);
            ui.minutesLineEdit->setEnabled(false);
        }
//    }
}

void CompassConfig::autoDecClicked(bool enabled)
{

        if (enabled)
        {
           // m_uas->getParamManager()->setParameter(1,"COMPASS_AUTODEC",QVariant(1));
             ParameterController::Instance()->set_parameter_noanswer((char*)"COMPASS_AUTODEC",1,(MAV_PARAM_TYPE)1);
        }
        else
        {
//            m_uas->getParamManager()->setParameter(1,"COMPASS_AUTODEC",QVariant(0));
            ParameterController::Instance()->set_parameter_noanswer((char*)"COMPASS_AUTODEC",0,(MAV_PARAM_TYPE)1);
        }

}

void CompassConfig::orientationComboChanged(int index)
{
    //COMPASS_ORIENT
//    if (!m_uas)
//    {
//        return;
//    }
//    QLOG_DEBUG() << "setCompassOrientation index:" << index;
    //m_uas->getParamManager()->setParameter(1,"COMPASS_ORIENT",index);
    ParameterController::Instance()->set_parameter_noanswer((char*)"COMPASS_ORIENT",index,(MAV_PARAM_TYPE)1);

}

void CompassConfig::setCompassAPMOnBoard()
{
      QList<ParamData>* temp= SettingManager::Instance()->getParametersFromControl();
      for(int i=0;i<temp->size();i++)
      {
            if(temp->at(i).key=="COMPASS_ORIENT")
            {
                if(temp->at(i).value.toInt()!=COMPASS_ORIENT_NONE)
                {
                    ParameterController::Instance()->set_parameter_noanswer((char*)"COMPASS_ORIENT",COMPASS_ORIENT_NONE,(MAV_PARAM_TYPE)1);
                }
                else
                {
                    updateCompassSelection();
                }
            }
      }
}

void CompassConfig::setCompassPX4OnBoard()
{
    ParamData data_t;
    queryParam(data_t,(char*)"COMPASS_ORIENT");
    if (data_t.value.toInt() != COMPASS_ORIENT_NONE){
//        pm->setParameter(1,"COMPASS_ORIENT", COMPASS_ORIENT_NONE);
      ParameterController::Instance()->set_parameter_noanswer((char*)"COMPASS_ORIENT",COMPASS_ORIENT_NONE,(MAV_PARAM_TYPE)1);
    } else {
        updateCompassSelection();
    }

}

void CompassConfig::setCompass3DRGPS()
{
//    if (!m_uas)
//        return;
    // ROTATION_ROLL_180
   // QLOG_DEBUG() << "setCompass3DRGPS ROLL_180";
//    QGCUASParamManager* pm = m_uas->getParamManager();
    ParamData data_t;
    queryParam(data_t,(char*)"COMPASS_ORIENT");
    if (data_t.value.toInt() != COMPASS_ORIENT_ROLL_180){
       ParameterController::Instance()->set_parameter_noanswer((char*)"COMPASS_ORIENT", COMPASS_ORIENT_ROLL_180,(MAV_PARAM_TYPE)1);
    } else {
        updateCompassSelection();
    }

}

void CompassConfig::liveCalibrationClicked()
{
//    QLOG_DEBUG() << "live Calibration Started";
//    if (!m_uas) {
//        showNullMAVErrorMessageBox();
//        return;
//    }
//    qDebug() << "start";
    QMessageBox::information(this,QString::fromLocal8Bit("罗盘校准"),
                             QString::fromLocal8Bit("收集数据将会持续120秒, 请点击确认并且围绕飞控的所有的轴移动"));

    // Initialiase to zero
     ParameterController::Instance()->set_parameter_noanswer((char*)"COMPASS_OFS_X",0.0,(MAV_PARAM_TYPE)9);
     ParameterController::Instance()->set_parameter_noanswer((char*)"COMPASS_OFS_Y",0.0,(MAV_PARAM_TYPE)9);
     ParameterController::Instance()->set_parameter_noanswer((char*)"COMPASS_OFS_Z",0.0,(MAV_PARAM_TYPE)9);
     ParameterController::Instance()->set_parameter_noanswer((char*)"COMPASS_OFS2_X",0.0,(MAV_PARAM_TYPE)9);
     ParameterController::Instance()->set_parameter_noanswer((char*)"COMPASS_OFS2_Y",0.0,(MAV_PARAM_TYPE)9);
     ParameterController::Instance()->set_parameter_noanswer((char*)"COMPASS_OFS2_Z",0.0,(MAV_PARAM_TYPE)9);
     ParameterController::Instance()->set_parameter_noanswer((char*)"COMPASS_OFS3_X",0.0,(MAV_PARAM_TYPE)9);
     ParameterController::Instance()->set_parameter_noanswer((char*)"COMPASS_OFS3_Y",0.0,(MAV_PARAM_TYPE)9);
     ParameterController::Instance()->set_parameter_noanswer((char*)"COMPASS_OFS3_Z",0.0,(MAV_PARAM_TYPE)9);


    QTimer::singleShot(1000,this,SLOT(startDataCollection()));
}

void CompassConfig::startDataCollection()
{
    ParamData t_data;
    queryParam(t_data,(char*)"COMPASS_DEV_ID");
    m_compassId = t_data.value.toInt();
     queryParam(t_data,(char*)"COMPASS_DEV_ID2");
    m_compassId2 = t_data.value.toInt();
    queryParam(t_data,(char*)"COMPASS_DEV_ID3");
    m_compassId3 = t_data.value.toInt();

    ParamData t_data_X,t_data_Y,t_data_Z;
    queryParam(t_data_X,(char*)"COMPASS_OFS_X");
    queryParam(t_data_Y,(char*)"COMPASS_OFS_Y");
    queryParam(t_data_Z,(char*)"COMPASS_OFS_Z");
    m_compass1Offset.set(t_data_X.value.toDouble(),
                            t_data_Y.value.toDouble(),
                            t_data_Z.value.toDouble());


    queryParam(t_data_X,(char*)"COMPASS_OFS2_X");
    queryParam(t_data_Y,(char*)"COMPASS_OFS2_Y");
    queryParam(t_data_Z,(char*)"COMPASS_OFS2_Z");

    m_compass2Offset.set(t_data_X.value.toDouble(),
                         t_data_Y.value.toDouble(),
                         t_data_Z.value.toDouble());

    queryParam(t_data_X,(char*)"COMPASS_OFS3_X");
    queryParam(t_data_Y,(char*)"COMPASS_OFS3_Y");
    queryParam(t_data_Z,(char*)"COMPASS_OFS3_Z");

    m_compass3Offset.set(t_data_X.value.toDouble(),
                            t_data_Y.value.toDouble(),
                            t_data_Z.value.toDouble());

    m_compass1LastValue.set(0.0, 0.0, 0.0); // Compass 1
    m_compass2LastValue.set(0.0, 0.0, 0.0); // Compass 2
    m_compass3LastValue.set(0.0, 0.0, 0.0); // Compass 3

//    QLOG_DEBUG() << "Compass1 offsets x:" << m_compass1Offset.x() <<
//                    " y:" << m_compass1Offset.y() <<
//                    " z:" << m_compass1Offset.z() ;
//    QLOG_DEBUG() << "Compass2 offsets x:" << m_compass2Offset.x() <<
//                    " y:" << m_compass2Offset.y() <<
//                    " z:" << m_compass2Offset.z() ;

    connect(&FrmMainController::Instance()->__vehicle->mavLinkMessageInterface,
            SIGNAL(calibrate_raw_imu_recved(mavlink_raw_imu_t)),
                this, SLOT(rawImuMessageUpdate(mavlink_raw_imu_t)));
    connect(&FrmMainController::Instance()->__vehicle->mavLinkMessageInterface,
            SIGNAL(calibrate_scale_imu2_recved(mavlink_scaled_imu2_t)),
                this, SLOT(scaledImu2MessageUpdate(mavlink_scaled_imu2_t)));
//    m_uas->enableRawSensorDataTransmission(10);//xjw ???
    m_calibratingCompass = true;

    m_progressDialog = new QProgressDialog(QString::fromLocal8Bit("正在进行罗盘的校准. 请按照顺序围绕无人机的所有轴转动持续120秒."),
                                           QString::fromLocal8Bit("取消"), 0, 120, this);
    connect(m_progressDialog, SIGNAL(canceled()), this, SLOT(cancelCompassCalibration()));
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(progressCounter()));
    m_timer->start(1); // second counting progress timer
}

void CompassConfig::progressCounter()
{

    int newValue = m_progressDialog->value()+1;
    m_progressDialog->setValue(newValue);
    if (newValue < 120) {
        m_timer->start(1000);
    } else {
        finishCompassCalibration();
    }
}

void CompassConfig::cancelCompassCalibration()
{
//    QLOG_INFO() << "cancelCompassCalibration";
//    m_uas->enableRawSensorDataTransmission(2);    //xjw ??? 与下面是否一个意思
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.requestDataStreamRawSensors(1,0);
    disconnect(&FrmMainController::Instance()->__vehicle->mavLinkMessageInterface,
               SIGNAL(calibrate_raw_imu_recved(mavlink_raw_imu_t)),
                this, SLOT(rawImuMessageUpdate(mavlink_raw_imu_t)));
    disconnect(&FrmMainController::Instance()->__vehicle->mavLinkMessageInterface,
               SIGNAL(calibrate_scale_imu2_recved(mavlink_scaled_imu2_t)),
               this, SLOT(scaledImu2MessageUpdate(mavlink_scaled_imu2_t)));
    cleanup();
}

void CompassConfig::cleanup()
{
    if (m_timer) m_timer->stop();
    delete m_timer;
    m_compass1RawImuList.clear();
    m_compass2RawImuList.clear();
    m_compass3RawImuList.clear();
    delete m_progressDialog;
}

void CompassConfig::finishCompassCalibration()
{
//    QLOG_INFO() << "finishCompassCalibration with compass 1:" << m_compass1RawImuList.count() << " data points";
//    QLOG_INFO() << "finishCompassCalibration with compass 2:" << m_compass2RawImuList.count() << " data points";
    disconnect(&FrmMainController::Instance()->__vehicle->mavLinkMessageInterface, SIGNAL(calibrate_raw_imu_recved(mavlink_raw_imu_t)),
                this, SLOT(rawImuMessageUpdate(mavlink_raw_imu_t)));
    disconnect(&FrmMainController::Instance()->__vehicle->mavLinkMessageInterface, SIGNAL(calibrate_scale_imu2_recved(mavlink_scaled_imu2_t)),
                this, SLOT(scaledImu2MessageUpdate(mavlink_scaled_imu2_t)));
    //m_uas->enableRawSensorDataTransmission(2);//xjw ???
    m_calibratingCompass = false;
    m_timer->stop();

    // Calculate and send the update message
    QVariant deviceId;
    QString message; // resultant calibration message
    Vector3d centerCompass1;
    Vector3d centerCompass2;



    if ( centerCompass1.setToLeastSquaresSphericalCenter(m_compass1RawImuList)){
        saveOffsets(centerCompass1, MAV_SENSOR_OFFSET_MAGNETOMETER);
        ParamData t_data;
        queryParam(t_data,(char*)"COMPASS_DEV_ID");
        message = tr("New offsets (Compass 1) are \n\nx:") + QString::number(centerCompass1.x(),'f',3)
                            + " y:" + QString::number(centerCompass1.y(),'f',3) + " z:" + QString::number(centerCompass1.z(),'f',3)
                            + " dev id:" + t_data.value;
    }
     else
    {
        //QLOG_ERROR() << "Not enough data points for calculation of compass 1:" ;
        QMessageBox::warning(this, tr("Compass 1 Calibration Failed"), tr("Not enough data points to calibrate the compass."));
        message = "\n" + tr("Compass 1 Calibration Failed");
    }

    if(m_haveSecondCompass) {
        // Second Compass Calibration
        if ( centerCompass2.setToLeastSquaresSphericalCenter(m_compass2RawImuList)){
            saveOffsets(centerCompass2, MAV_SENSOR_OFFSET_MAGNETOMETER2);

//            paramMgr->getParameterValue(1, "COMPASS_DEV_ID2", deviceId);
            ParamData t_data;
            queryParam(t_data,(char*)"COMPASS_DEV_ID2");
            message.append(tr("\n\nNew offsets (Compass 2) are \n\nx:") + QString::number(centerCompass2.x(),'f',3)
                           + " y:" + QString::number(centerCompass2.y(),'f',3) + " z:" + QString::number(centerCompass2.z(),'f',3)
                           + " dev id:" + t_data.value);
        } else {
         //   QLOG_ERROR() << "Not enough data points for calculation of compass 2:" ;
            QMessageBox::warning(this, tr("Compass 2 Calibration Failed"), tr("Not enough data points to calibrate the compass."));
            message = "\n" + tr("Compass 2 Calibration Failed");;
        }
    }
    cleanup();

    QMessageBox::information(this, tr("New Compass Offsets"), message + tr("\n\nThese have been saved for you."));
}

void CompassConfig::saveOffsets(const Vector3d &offset, int compassId)
{
//    QGCUASParamManager* paramMgr = m_uas->getParamManager();
//    paramMgr->setParameter(1, "COMPASS_LEARN", 0);
ParameterController::Instance()->set_parameter_noanswer((char*)"COMPASS_LEARN",0,(MAV_PARAM_TYPE)1);
    // set values
//    m_uas->executeCommand(MAV_CMD_PREFLIGHT_SET_SENSOR_OFFSETS, 1, compassId,
//                          offset.x(), offset.y(), offset.z(), 0, 0, 0,
//                          MAV_COMP_ID_PRIMARY);
   FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.doCommand(MAV_CMD_PREFLIGHT_SET_SENSOR_OFFSETS,compassId,offset.x(), offset.y(), offset.z(), 0, 0, 0);
}

void CompassConfig::updateImuList(const Vector3d &currentReading, Vector3d &compassLastValue,
                                  Vector3d &compassOffset, QVector<Vector3d> &list)
{
    if (isCalibratingCompass()){
        if (compassLastValue != currentReading){
            Vector3d adjustedValue;
            // Remove the current offset from the reading.
            adjustedValue = currentReading - compassOffset;
            list.append(adjustedValue);

            compassLastValue = currentReading;
        }
    }
}

void CompassConfig::rawImuMessageUpdate(mavlink_raw_imu_t rawImu)
{
    qDebug() << "RAW IMU x:" << rawImu.xmag << " y:" << rawImu.ymag << " z:" << rawImu.zmag;
    //QLOG_TRACE() << "RAW IMU x:" << rawImu.xmag << " y:" << rawImu.ymag << " z:" << rawImu.zmag;
    const Vector3d currentReading(rawImu.xmag, rawImu.ymag, rawImu.zmag);
    updateImuList(currentReading, m_compass1LastValue,
                  m_compass1Offset, m_compass1RawImuList);
}

void CompassConfig::scaledImu2MessageUpdate(mavlink_scaled_imu2_t scaledImu)
{
   // QLOG_TRACE() << "SCALED IMU2 x:" << scaledImu.xmag << " y:" << scaledImu.ymag << " z:" << scaledImu.zmag;
   // qDebug() << "SCALED IMU2 x:" << scaledImu.xmag << " y:" << scaledImu.ymag << " z:" << scaledImu.zmag;
    if (scaledImu.xmag == 0 && scaledImu.ymag == 0 && scaledImu.zmag == 0)
    {
        //Don't use values of 0, since they could be a disconnected compass
        return;
    }
    m_haveSecondCompass = true;
    const Vector3d currentReading(scaledImu.xmag, scaledImu.ymag, scaledImu.zmag);
    updateImuList(currentReading, m_compass2LastValue,
                  m_compass2Offset, m_compass2RawImuList);

}

void CompassConfig::showCompassMotorCalibrationDialog()
{
    CompassMotorCalibrationDialog *dialog = new CompassMotorCalibrationDialog();
    if(dialog->exec() == QDialog::Accepted){
        // This modal, as you cannot do anything else while doing a compassMot
 //       QLOG_DEBUG() << "Compass Mot Success!";
    } else {
   //     QLOG_DEBUG() << "Compass Mot Cancelled!";
    }

}

//需要测试
void CompassConfig::queryParam(ParamData& param,char* argName)
{
    QList<ParamData>* temp= SettingManager::Instance()->getParametersFromControl();
    for(int i=0;i<temp->size();i++)
    {
          if(temp->at(i).key==QString(argName))
          {
            param=temp->at(i);
          }
    }
    param.count=0;
}

void CompassConfig::on_liveCalibrationButton_clicked()
{

}

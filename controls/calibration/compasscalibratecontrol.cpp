#include "compasscalibratecontrol.h"
#include "ui_compasscalibratecontrol.h"
#include "controls/myhelper.h"
#include "loghelper/logger.h"
#include "common/mavlink.h"
#include "Utils/zygroundglobalconfig.h"

CompassCalibrateControl::CompassCalibrateControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompassCalibrateControl)
{
    ui->setupUi(this);
    initial_lblImage();

    _magCalInProgress=false;

   _orientationCalDownSideDone             =false;
   _orientationCalUpsideDownSideDone       =false;
   _orientationCalLeftSideDone             =false;
   _orientationCalRightSideDone            =false;
   _orientationCalNoseDownSideDone         =false;
   _orientationCalTailDownSideDone         =false;

   _orientationCalDownSideInProgress       =false;
   _orientationCalUpsideDownSideInProgress =false;
   _orientationCalLeftSideInProgress       =false;
   _orientationCalRightSideInProgress      =false;
   _orientationCalNoseDownSideInProgress   =false;
   _orientationCalTailDownSideInProgress   =false;

   _orientationCalDownSideRotate           =false;
   _orientationCalUpsideDownSideRotate     =false;
   _orientationCalLeftSideRotate           =false;
   _orientationCalRightSideRotate          =false;
   _orientationCalNoseDownSideRotate       =false;
   _orientationCalTailDownSideRotate       =false;

   connect(this,SIGNAL(__setdown()),this,SLOT(setdown()));
   connect(this,SIGNAL(__setLeft()),this,SLOT(setLeft()));
   connect(this,SIGNAL(__setNoseDown()),this,SLOT(setNoseDown()));
   connect(this,SIGNAL(__setRight()),this,SLOT(setRight()));
   connect(this,SIGNAL(__setTrailDown()),this,SLOT(setTrailDown()));
   connect(this,SIGNAL(__setUp()),this,SLOT(setUp()));
   connect(&m_compassCal,SIGNAL(vehicleTextMessage(int,int,int,QString)),this,SLOT(vehicleTextMessage(int,int,int,QString)));
   //connect(&m_compassCal,SIGNAL(sendCompensationValue_signal(float,float,float)),this,SLOT(upDataCompensationValue(float,float,float)));
}

CompassCalibrateControl::~CompassCalibrateControl()
{
    delete ui;
}

void CompassCalibrateControl::initial_lblImage()
{
    this->ui->lbl_level->setStyleSheet("QLabel{border-image: url(:/image/calibration/compass/VehicleDown.png)}");
    this->ui->lbl_back->setStyleSheet("QLabel{border-image: url(:/image/calibration/compass/VehicleUpsideDown.png)}");
    this->ui->lbl_left->setStyleSheet("QLabel{border-image: url(:/image/calibration/compass/VehicleLeft.png)}");
    this->ui->lbl_right->setStyleSheet("QLabel{border-image: url(:/image/calibration/compass/VehicleRight.png)}");
    this->ui->lbl_noseDown->setStyleSheet("QLabel{border-image: url(:/image/calibration/compass/VehicleNoseDown.png)}");
    this->ui->lbl_traildown->setStyleSheet("QLabel{border-image: url(:/image/calibration/compass/VehicleTailDown.png)}");
}

void CompassCalibrateControl::setdown()
{
    this->ui->lbl_level->setStyleSheet("QLabel{border-image: url(:/image/calibration/compass/VehicleDownRotate.png)}");
}

void CompassCalibrateControl::setUp()
{
    this->ui->lbl_back->setStyleSheet("QLabel{border-image: url(:/image/calibration/compass/VehicleUpsideDownRotate.png)}");
}

void CompassCalibrateControl::setLeft()
{

    this->ui->lbl_left->setStyleSheet("QLabel{border-image: url(:/image/calibration/compass/VehicleLeftRotate.png)}");

}

void CompassCalibrateControl::setRight()
{

    this->ui->lbl_right->setStyleSheet("QLabel{border-image: url(:/image/calibration/compass/VehicleRightRotate.png)}");

}

void CompassCalibrateControl::setNoseDown()
{

    this->ui->lbl_noseDown->setStyleSheet("QLabel{border-image: url(:/image/calibration/compass/VehicleNoseDownRotate.png)}");

}

void CompassCalibrateControl::setTrailDown()
{

    this->ui->lbl_traildown->setStyleSheet("QLabel{border-image: url(:/image/calibration/compass/VehicleTailDownRotate.png)}");
}

void CompassCalibrateControl::on_btn_calibration_clicked()
{
    if(!ZYGroundGlobalConfig::m_SerialIsOpen)
    {
        myHelper::ShowMessageBoxInfo("请先连接无人机，再进行校准");
        return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface._isCompassCalibration = true;
    FrmMainController::Instance()->__requestDataStream(FrmMainController::Stop);
    QPushButton* btn_calibration= (QPushButton*)(sender());
    if(btn_calibration->text()==QString::fromLocal8Bit("开始校准"))
    {
        initial_lblImage();
        ui->lbl_backMsg->setText("");
        ui->lbl_leftMsg->setText("");
        ui->lbl_levelMsg->setText("");
        ui->lbl_nosedownMsg->setText("");
        ui->lbl_rightMsg->setText("");
        ui->lbl_traildownMsg->setText("");
        ui->lbl_offset->setText("");
        m_compassCal.startCalibration(ui->ck1->isChecked(),ui->ck2->isChecked());
        btn_calibration->setText(QString::fromLocal8Bit("完成校准"));
        btn_calibration->setEnabled(false);
    }
    else if(btn_calibration->text()==QString::fromLocal8Bit("完成校准"))
    {

       if(_orientationCalDownSideDone&&_orientationCalUpsideDownSideDone
            &&_orientationCalLeftSideDone&&_orientationCalRightSideDone
               &&_orientationCalNoseDownSideDone&&_orientationCalTailDownSideDone)
       {

         myHelper::ShowMessageBoxInfo("校准已经完成");


         //FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.doCommand(MAV_CMD_PREFLIGHT_STORAGE, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

         btn_calibration->setText(QString::fromLocal8Bit("开始校准"));

         FrmMainController::Instance()->__requestDataStream(FrmMainController::Fast);
       }
    }

}

void CompassCalibrateControl::vehicleTextMessage(int vehicleId, int compId, int severity, QString text)
{
    if (text.startsWith(QLatin1Literal("PreArm:")) || text.startsWith(QLatin1Literal("EKF"))
            || text.startsWith(QLatin1Literal("Arm")) || text.startsWith(QLatin1Literal("Initialising"))) {
        return;
    }

    if (text.contains(QLatin1Literal("progress <"))) {
        QString percent = text.split("<").last().split(">").first();
        bool ok;
        int p = percent.toInt(&ok);
        if (ok) {
            this->ui->progressBar->setProperty("value", (float)(p));
        }
        return;
    }
    QString anyKey(QStringLiteral("and press any"));
    if (text.contains(anyKey)) {
        text = text.left(text.indexOf(anyKey)) + QStringLiteral("and click Next to continue.");
    }

    if (text.contains(QLatin1String("Calibration successful"))) {
         ui->btn_calibration->setEnabled(true);
         ui->progressBar->setProperty("value", 100);
         this->ui->lbl_txtMsg->setText(QString::fromLocal8Bit("校准成功."));
         return;
    }
    if (text.contains(QLatin1String("failed"))) {
        ui->progressBar->setProperty("value", 0);
        this->ui->lbl_txtMsg->setText(QString::fromLocal8Bit("校准失败."));
        return;
    }
    QString calPrefix(QStringLiteral("[cal] "));
    if (!text.startsWith(calPrefix)) {
        return;
    }
    text = text.right(text.length() - calPrefix.length());

    QString calStartPrefix(QStringLiteral("calibration started: "));
    if (text.startsWith(calStartPrefix)) {
        text = text.right(text.length() - calStartPrefix.length());
         if (text == QLatin1Literal("accel") || text == QLatin1Literal("mag") || text == QLatin1Literal("gyro"))
         {
             _orientationCalDownSideDone = false;
             _orientationCalUpsideDownSideDone = false;
             _orientationCalLeftSideDone = false;
             _orientationCalRightSideDone = false;
             _orientationCalTailDownSideDone = false;
             _orientationCalNoseDownSideDone = false;
             _orientationCalDownSideInProgress = false;
             _orientationCalUpsideDownSideInProgress = false;
             _orientationCalLeftSideInProgress = false;
             _orientationCalRightSideInProgress = false;
             _orientationCalNoseDownSideInProgress = false;
             _orientationCalTailDownSideInProgress = false;

           initial_lblImage();
           this->ui->lbl_txtMsg->setText(QString::fromLocal8Bit("请将飞机的朝向以下还未校准的面的其中之一，并保持不动."));

           _magCalInProgress=true;

         }
    }

    Q_UNUSED(vehicleId);
    Q_UNUSED(compId);
    Q_UNUSED(severity);
    if (text.endsWith(QLatin1Literal("orientation detected")))
    {
        QString side = text.section(" ", 0, 0);
        qDebug() << "Side started" << side;
        //Logger::Instance()->LogInfo(QString("__________%1____________").arg(side));
        if (side == QLatin1Literal("down")) {

            _orientationCalDownSideInProgress = true;
            if(_magCalInProgress)
            {
                ui->lbl_levelMsg->setText(QString::fromLocal8Bit("正在收集数据..."));

                 _orientationCalDownSideRotate = true;
                emit __setdown();
            }


        } else if (side == QLatin1Literal("up")) {
            _orientationCalUpsideDownSideInProgress = true;

            if(_magCalInProgress)
            {
                ui->lbl_backMsg->setText(QString::fromLocal8Bit("正在收集数据..."));
                 _orientationCalUpsideDownSideRotate = true;
                emit __setUp();
            }
        } else if (side == QLatin1Literal("left")) {
             _orientationCalLeftSideInProgress = true;
            if(_magCalInProgress)
            {
                ui->lbl_leftMsg->setText(QString::fromLocal8Bit("正在收集数据..."));
                 _orientationCalLeftSideRotate = true;
               emit __setLeft();
            }
        } else if (side == QLatin1Literal("right")) {

            _orientationCalRightSideInProgress = true;
            if(_magCalInProgress)
            {
                ui->lbl_rightMsg->setText(QString::fromLocal8Bit("正在收集数据..."));
                 _orientationCalRightSideRotate = true;
                emit __setRight();
            }
        } else if (side == QLatin1Literal("front")) {

             _orientationCalNoseDownSideInProgress = true;
            if(_magCalInProgress)
            {
                _orientationCalNoseDownSideRotate = true;
                ui->lbl_nosedownMsg->setText(QString::fromLocal8Bit("正在收集数据..."));
                emit __setNoseDown();
            }
        } else if (side == QLatin1Literal("back")) {
                _orientationCalTailDownSideInProgress = true;
            if(_magCalInProgress)
            {
                ui->lbl_traildownMsg->setText(QString::fromLocal8Bit("正在收集数据..."));
                _orientationCalTailDownSideRotate = true;
                emit __setTrailDown();
            }
        }

        if (_magCalInProgress)
        {
             ui->lbl_txtMsg->setText(QString::fromLocal8Bit("逆时针方向持续转动无人机直到图标标识为完成状态."));

        }
        else
        {
             ui->lbl_txtMsg->setText(QString::fromLocal8Bit("在当前朝向保持静止."));
        }

        return;
    }

    if (text.endsWith(QLatin1Literal("side done, rotate to a different side"))) {
        QString side = text.section(" ", 0, 0);
        qDebug() << "Side finished" << side;

        if (side == QLatin1Literal("down")) {
            _orientationCalDownSideInProgress = false;
            _orientationCalDownSideDone = true;
            _orientationCalDownSideRotate = false;
             ui->lbl_levelMsg->setText(QString::fromLocal8Bit("校准完成!"));

        } else if (side == QLatin1Literal("up")) {
            _orientationCalUpsideDownSideInProgress = false;
            _orientationCalUpsideDownSideDone = true;
            _orientationCalUpsideDownSideRotate = false;
             ui->lbl_backMsg->setText(QString::fromLocal8Bit("校准完成!"));
        } else if (side == QLatin1Literal("left")) {
            _orientationCalLeftSideInProgress = false;
            _orientationCalLeftSideDone = true;
            _orientationCalLeftSideRotate = false;
            ui->lbl_leftMsg->setText(QString::fromLocal8Bit("校准完成!"));
        } else if (side == QLatin1Literal("right")) {
            _orientationCalRightSideInProgress = false;
            _orientationCalRightSideDone = true;
            _orientationCalRightSideRotate = false;
             ui->lbl_rightMsg->setText(QString::fromLocal8Bit("校准完成!"));
        } else if (side == QLatin1Literal("front")) {
            _orientationCalNoseDownSideInProgress = false;
            _orientationCalNoseDownSideDone = true;
            _orientationCalNoseDownSideRotate = false;
            ui->lbl_nosedownMsg->setText(QString::fromLocal8Bit("校准完成!"));
        } else if (side == QLatin1Literal("back")) {
            _orientationCalTailDownSideInProgress = false;
            _orientationCalTailDownSideDone = true;
            _orientationCalTailDownSideRotate = false;
             ui->lbl_traildownMsg->setText(QString::fromLocal8Bit("校准完成!"));
        }

         ui->lbl_txtMsg->setText(QString::fromLocal8Bit("请将飞机的朝向以下还未校准的面的其中之一，并保持不动."));

        return;
    }

    if (text.startsWith(QLatin1Literal("calibration done:"))) {
        //_stopCalibration(StopCalibrationSuccess);
        //m_compassCal.cancelCalibration();
        this->ui->btn_calibration->setEnabled(true);
        this->ui->lbl_txtMsg->setText(QString::fromLocal8Bit("已完成:")+m_offsetContent);

         //myHelper::ShowMessageBoxInfo(m_offsetContent.toLocal8Bit().data());
        return;
    }

    if (text.startsWith(QLatin1Literal("calibration cancelled"))) {
        //_stopCalibration(_waitingForCancel ? StopCalibrationCancelled : StopCalibrationFailed);
          this->ui->lbl_txtMsg->setText(text);
        return;
    }

    if (text.startsWith(QLatin1Literal("calibration failed"))) {
        //_stopCalibration(StopCalibrationFailed);
           this->ui->lbl_txtMsg->setText(text);
        return;
    }

    if(text.startsWith("mag #"))
    {

        this->ui->lbl_txtMsg->setText("Calibration Completed:"+text);

        m_offsetContent=m_offsetContent+" | "+text;
    }
}

bool CompassCalibrateControl::compassSetupNeeded(void) const
{
    const size_t cCompass = 3;
    const size_t cOffset = 3;
    QStringList devicesIds;
    QStringList rgOffsets[cCompass];

    devicesIds   << QStringLiteral("COMPASS_DEV_ID") << QStringLiteral("COMPASS_DEV_ID2") << QStringLiteral("COMPASS_DEV_ID3");
    rgOffsets[0] << QStringLiteral("COMPASS_OFS_X") << QStringLiteral("COMPASS_OFS_X") << QStringLiteral("COMPASS_OFS_X");
    rgOffsets[1] << QStringLiteral("COMPASS_OFS2_X") << QStringLiteral("COMPASS_OFS2_X") << QStringLiteral("COMPASS_OFS2_X");
    rgOffsets[2] << QStringLiteral("COMPASS_OFS3_X") << QStringLiteral("COMPASS_OFS3_X") << QStringLiteral("COMPASS_OFS3_X");

    for (size_t i=0; i<cCompass; i++) {
        //if (_autopilot->getParameterFact(FactSystem::defaultComponentId, devicesIds[i])->rawValue().toInt() != 0)
        {
            for (size_t j=0; j<cOffset; j++) {
               // if (_autopilot->getParameterFact(FactSystem::defaultComponentId, rgOffsets[i][j])->rawValue().toFloat() == 0.0f)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool CompassCalibrateControl::accelSetupNeeded(void) const
{
    const size_t cAccel = 3;
    const size_t cOffset = 3;
    QStringList insUse;
    QStringList rgOffsets[cAccel];

    //if (_autopilot->parameterExists(FactSystem::defaultComponentId, "INS_USE"))
    {
        insUse << QStringLiteral("INS_USE") << QStringLiteral("INS_USE2") << QStringLiteral("INS_USE3");
        rgOffsets[0] << QStringLiteral("INS_ACCOFFS_X") << QStringLiteral("INS_ACCOFFS_Y") << QStringLiteral("INS_ACCOFFS_Z");
        rgOffsets[1] << QStringLiteral("INS_ACC2OFFS_X") << QStringLiteral("INS_ACC2OFFS_Y") << QStringLiteral("INS_ACC2OFFS_Z");
        rgOffsets[2] << QStringLiteral("INS_ACC3OFFS_X") << QStringLiteral("INS_ACC3OFFS_Y") << QStringLiteral("INS_ACC3OFFS_Z");
    }
    //else
    {
        // For older firmwares which don't support the INS_USE parameter we can't determine which secondary accels are in use.
        // So we just base things off the the first accel.
        rgOffsets[0] << QStringLiteral("INS_ACCOFFS_X") << QStringLiteral("INS_ACCOFFS_Y") << QStringLiteral("INS_ACCOFFS_Z");
    }

    for (size_t i=0; i<cAccel; i++) {
        //if (insUse.count() == 0 || _autopilot->getParameterFact(FactSystem::defaultComponentId, insUse[i])->rawValue().toInt() != 0)
        {
            for (size_t j=0; j<cOffset; j++) {
                //if (rgOffsets[i].count() && _autopilot->getParameterFact(FactSystem::defaultComponentId, rgOffsets[i][j])->rawValue().toFloat() == 0.0f)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

void CompassCalibrateControl::on_btn_Cancelcalibrate_clicked()
{
     m_compassCal.cancelCalibration();
      initial_lblImage();
      ui->btn_calibration->setEnabled(true);
      ui->btn_calibration->setText(QString::fromLocal8Bit("开始校准"));
}

void CompassCalibrateControl::upDataCompensationValue(float x, float y, float z){
    QString tmp = "offset:x["+QString::number(x, 'f') +"]\n" \
            + "       y[" + QString::number(y, 'f') +"]\n" \
            + "       z["+QString::number(z, 'f') +"]\n";
    ui->lbl_offset->setText(tmp);
}

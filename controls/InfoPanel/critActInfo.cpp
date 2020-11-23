#include "critActInfo.h"
#include "ui_critActInfo.h"
#include "../../Utils/zygroundglobalconfig.h"
#include "../../controls/myhelper.h"
#include <QRegExp>

critActInfo::critActInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::critActInfo)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
    //g_bcritActInfo = false;

    ui->le_mainVoltCrit_value->setValidator(new QIntValidator(10, 100, this));
    ui->cmb_mainVoltCritAct_value->addItem(QStringLiteral("无动作"));
    ui->cmb_mainVoltCritAct_value->addItem(QStringLiteral("报警提示"));
    ui->cmb_mainVoltCritAct_value->addItem(QStringLiteral("直接降落"));
    ui->cmb_mainVoltCritAct_value->setCurrentIndex(2);
    ui->le_mainVoltScale_value->setValidator(new QDoubleValidator(0.5, 2.0,1, this));

    ui->le_bakVoltCells_value->setValidator(new QIntValidator(1, 12, this));
    ui->le_bakVoltLow_value->setValidator(new QDoubleValidator(0.0, 4.5, 2, this));
    ui->le_bakVoltCrit_value->setValidator(new QDoubleValidator(0.0, 4.5, 2, this));
    ui->cmb_bakVoltLowAct_value->addItem(QStringLiteral("无动作"));
    ui->cmb_bakVoltLowAct_value->addItem(QStringLiteral("返航降落"));
    ui->cmb_bakVoltLowAct_value->addItem(QStringLiteral("直接降落"));
    ui->cmb_bakVoltLowAct_value->setCurrentIndex(0);
    ui->cmb_bakVoltCritAct_value->addItem(QStringLiteral("无动作"));
    ui->cmb_bakVoltCritAct_value->addItem(QStringLiteral("返航降落"));
    ui->cmb_bakVoltCritAct_value->addItem(QStringLiteral("直接降落"));
    ui->cmb_bakVoltCritAct_value->setCurrentIndex(2);
    ui->le_bakVoltScale_value->setValidator(new QDoubleValidator(0.5, 2.0, 2, this));

    ui->le_RCLossTime_value->setValidator(new QDoubleValidator(0.5, 35, 2, this));
    ui->cmb_RCLossAct_value->addItem(QStringLiteral("无动作"));
    ui->cmb_RCLossAct_value->addItem(QStringLiteral("直接降落"));
    ui->cmb_RCLossAct_value->addItem(QStringLiteral("悬停"));
    ui->cmb_RCLossAct_value->setCurrentIndex(2);

    ui->le_digitalLossTime_value->setValidator(new QIntValidator(30, 300, this));
    ui->cmb_digitalLossAct_value->addItem(QStringLiteral("无动作"));
    ui->cmb_digitalLossAct_value->addItem(QStringLiteral("直接降落"));
    ui->cmb_digitalLossAct_value->setCurrentIndex(0);
	
}

critActInfo::~critActInfo()
{
    delete ui;
}


void critActInfo::updateCritActInfo(char *id, float value)
{
	QString strID;
	QString strValue;
	strID.sprintf("%s", id);
    strValue.sprintf("%.2f", value);
	if (strID.contains("PWR_CRIT_V"))
	{
        ui->le_mainVoltCrit_value->setText(strValue);
	}
	else if (strID.contains("PWR_CRIT_ACT"))
    {
        ui->cmb_mainVoltCritAct_value->setCurrentIndex((int)value);
	}
	else if (strID.contains("PWR_V_SCALE"))
	{
        ui->le_mainVoltScale_value->setText(strValue);
	}
	else if (strID.contains("BAT_N_CELLS"))
	{
        ui->le_bakVoltCells_value->setText(strValue);
	}
	else if (strID.contains("BAT_LOW_THR"))
	{
        ui->le_bakVoltLow_value->setText(strValue);
	}
	else if (strID.contains("BAT_CRIT_THR"))
	{
        ui->le_bakVoltCrit_value->setText(strValue);
	}
	else if (strID.contains("COM_LOW_BAT_ACT"))
	{
        ui->cmb_bakVoltLowAct_value->setCurrentIndex((int)value);
	}
	else if (strID.contains("COM_CRIT_BAT_ACT"))
	{
        ui->cmb_bakVoltCritAct_value->setCurrentIndex((int)value);
	}
	else if (strID.contains("BAT_V_SCALE"))
	{
        ui->le_bakVoltScale_value->setText(strValue);
	}
	else if (strID.contains("COM_RC_LOSS_T"))
	{
        ui->le_RCLossTime_value->setText(strValue);
	}
	else if (strID.contains("NAV_RCL_ACT"))
	{
        ui->cmb_RCLossAct_value->setCurrentIndex((int)value);
	}
	else if (strID.contains("COM_DL_LOSS_T"))
	{
        ui->le_digitalLossTime_value->setText(strValue);
	}
	else if (strID.contains("NAV_DLL_ACT"))
	{
        ui->cmb_digitalLossAct_value->setCurrentIndex((int)value);
	}
    else if (strID.contains("TRA_ROT_DEG"))
    {
        ui->le_TRA_ROT_DEG_value->setText(strValue);
    }
    else if (strID.contains("IMU_POS_BODY_X"))
    {
        ui->le_IMU_POS_BODY_X_value->setText(strValue);
    }
    else if (strID.contains("IMU_POS_BODY_Y"))
    {
        ui->le_IMU_POS_BODY_Y_value->setText(strValue);
    }
    else if (strID.contains("IMU_POS_BODY_Z"))
    {
        ui->le_IMU_POS_BODY_Z_value->setText(strValue);
    }
    else if (strID.contains("GPS_POS_BODY_X"))
    {
        ui->le_GPS_POS_BODY_X_value->setText(strValue);
    }
    else if (strID.contains("GPS_POS_BODY_Y"))
    {
        ui->le_GPS_POS_BODY_Y_value->setText(strValue);
    }
    else if (strID.contains("GPS_POS_BODY_Z"))
    {
        ui->le_GPS_POS_BODY_Z_value->setText(strValue);
    }
    else if (strID.contains("RTK_POS_BODY_X"))
    {
        ui->le_RTK_POS_BODY_X_value->setText(strValue);
    }
    else if (strID.contains("RTK_POS_BODY_Y"))
    {
        ui->le_RTK_POS_BODY_Y_value->setText(strValue);
    }
    else if (strID.contains("RTK_POS_BODY_Z"))
    {
        ui->le_RTK_POS_BODY_Z_value->setText(strValue);
    }
    else if (strID.contains("MC_ROLL_P"))
    {
        ui->le_MC_ROLL_P_value->setText(strValue);
    }
    else if (strID.contains("MC_ROLLRATE_P"))
    {
        ui->le_MC_ROLLRATE_P_value->setText(strValue);
    }
    else if (strID.contains("MC_ROLLRATE_I"))
    {
        ui->le_MC_ROLLRATE_I_value->setText(strValue);
    }
    else if (strID.contains("MC_ROLLRATE_D"))
    {
        ui->le_MC_ROLLRATE_D_value->setText(strValue);
    }
    else if (strID.contains("MC_PITCH_P"))
    {
        ui->le_MC_PITCH_P_value->setText(strValue);
    }
    else if (strID.contains("MC_PITCHRATE_P"))
    {
        ui->le_MC_PITCHRATE_P_value->setText(strValue);
    }
    else if (strID.contains("MC_PITCHRATE_I"))
    {
        ui->le_MC_PITCHRATE_I_value->setText(strValue);
    }
    else if (strID.contains("MC_PITCHRATE_D"))
    {
        ui->le_MC_PITCHRATE_D_value->setText(strValue);
    }
    else if (strID.contains("MC_YAW_P"))
    {
        ui->le_MC_YAW_P_value->setText(strValue);
    }
    else if (strID.contains("MC_YAWRATE_P"))
    {
        ui->le_MC_YAWRATE_P_value->setText(strValue);
    }
    else if (strID.contains("MC_YAWRATE_I"))
    {
        ui->le_MC_YAWRATE_I_value->setText(strValue);
    }
    else if (strID.contains("MC_YAWRATE_D"))
    {
        ui->le_MC_YAWRATE_D_value->setText(strValue);
    }
    else if (strID.contains("MPC_XY_P"))
    {
        ui->le_MPC_XY_P_value->setText(strValue);
    }
    else if (strID.contains("MPC_XY_VEL_P"))
    {
        ui->le_MPC_XY_VEL_P_value->setText(strValue);
    }
    else if (strID.contains("MPC_XY_VEL_I"))
    {
        ui->le_MPC_XY_VEL_I_value->setText(strValue);
    }
    else if (strID.contains("MPC_XY_VEL_D"))
    {
        ui->le_MPC_XY_VEL_D_value->setText(strValue);
    }
    else if (strID.contains("MPC_Z_P"))
    {
        ui->le_MPC_Z_P_value->setText(strValue);
    }
    else if (strID.contains("MPC_Z_VEL_P"))
    {
        ui->le_MPC_Z_VEL_P_value->setText(strValue);
    }
    else if (strID.contains("MPC_Z_VEL_I"))
    {
        ui->le_MPC_Z_VEL_I_value->setText(strValue);
    }
    else if (strID.contains("MPC_Z_VEL_D"))
    {
        ui->le_MPC_Z_VEL_D_value->setText(strValue);
    }
}

void critActInfo::on_btn_readCritAct_clicked()
{
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"BAT_N_CELLS");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"BAT_LOW_THR");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"BAT_CRIT_THR");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"COM_LOW_BAT_ACT");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"COM_CRIT_BAT_ACT");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"BAT_V_SCALE");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"PWR_CRIT_V");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"PWR_CRIT_ACT");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"PWR_V_SCALE");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"COM_RC_LOSS_T");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"NAV_RCL_ACT");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"COM_DL_LOSS_T");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"NAV_DLL_ACT");
}

void critActInfo::on_btn_readPID_clicked()
{
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"MC_ROLL_P");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"MC_ROLLRATE_P");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"MC_ROLLRATE_I");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"MC_ROLLRATE_D");

    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"MC_PITCH_P");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"MC_PITCHRATE_P");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"MC_PITCHRATE_I");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"MC_PITCHRATE_D");

    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"MC_YAW_P");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"MC_YAWRATE_P");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"MC_YAWRATE_I");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"MC_YAWRATE_D");

    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"MPC_XY_P");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"MPC_XY_VEL_P");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"MPC_XY_VEL_I");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"MPC_XY_VEL_D");

    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"MPC_Z_P");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"MPC_Z_VEL_P");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"MPC_Z_VEL_I");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"MPC_Z_VEL_D");
//    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestList();
}

void critActInfo::on_btn_readPosition_clicked()
{
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"TRA_ROT_DEG");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"IMU_POS_BODY_X");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"IMU_POS_BODY_Y");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"IMU_POS_BODY_Z");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"GPS_POS_BODY_X");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"GPS_POS_BODY_Y");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"GPS_POS_BODY_Z");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"RTK_POS_BODY_X");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"RTK_POS_BODY_Y");
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramRequestRead((char*)"RTK_POS_BODY_Z");
}

void critActInfo::on_btn_setCritAct_clicked()
{
    QString strValue = ui->le_mainVoltCrit_value->text();
    qreal value = strValue.toFloat();
    if(strValue.length() == 0 || value > 100 || value < 10)
    {
       myHelper::ShowMessageBoxError("主电电压过低电压门限设置范围为10-100");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(value, (char *)"PWR_CRIT_V", MAV_PARAM_TYPE_INT32);

    int index = ui->cmb_mainVoltCritAct_value->currentIndex();
    float f;
    memcpy(&f, (void*)&index, sizeof(int));
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(f, (char *)"PWR_CRIT_ACT", MAV_PARAM_TYPE_INT32);

    strValue = ui->le_mainVoltScale_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 2 || value < 0.5)
    {
       myHelper::ShowMessageBoxError("主电电压校准系数设置范围为0.5-2");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"PWR_V_SCALE",MAV_PARAM_TYPE_REAL32);

    strValue = ui->le_bakVoltCells_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 12 || value < 1)
    {
       myHelper::ShowMessageBoxError("备电电池S数设置范围为1-12");
       return;
    }
    int i = strValue.toInt();
    memcpy(&f, (void*)&i, sizeof(int));
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(f, (char *)"BAT_N_CELLS", MAV_PARAM_TYPE_INT32);

    strValue = ui->le_bakVoltLow_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 4.5 || value < 0)
    {
       myHelper::ShowMessageBoxError("备电电池低电压门限（单片）设置范围为0-4.5");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"BAT_LOW_THR",MAV_PARAM_TYPE_REAL32);

    strValue = ui->le_bakVoltCrit_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 4.5 || value < 0)
    {
       myHelper::ShowMessageBoxError("备电电池过低电压门限（单片）设置范围为0-4.5");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"BAT_CRIT_THR",MAV_PARAM_TYPE_REAL32);

    index = ui->cmb_bakVoltLowAct_value->currentIndex();
    memcpy(&f, (void*)&index, sizeof(int));
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(f, (char *)"COM_LOW_BAT_ACT", MAV_PARAM_TYPE_INT32);

    index = ui->cmb_bakVoltCritAct_value->currentIndex();
    memcpy(&f, (void*)&index, sizeof(int));
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(f, (char *)"COM_CRIT_BAT_ACT", MAV_PARAM_TYPE_INT32);

    strValue = ui->le_bakVoltScale_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 2 || value < 0.5)
    {
       myHelper::ShowMessageBoxError("备电电压校准系数设置范围为0.5-2");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"BAT_V_SCALE",MAV_PARAM_TYPE_REAL32);

    strValue = ui->le_RCLossTime_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 35 || value < 0.5)
    {
       myHelper::ShowMessageBoxError("遥控器断开确认时间(秒）设置范围为0.5-35");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"COM_RC_LOSS_T",MAV_PARAM_TYPE_REAL32);

    index = ui->cmb_RCLossAct_value->currentIndex();
    memcpy(&f, (void*)&index, sizeof(int));
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(f, (char *)"NAV_RCL_ACT", MAV_PARAM_TYPE_INT32);

    strValue = ui->le_digitalLossTime_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 300 || value < 30)
    {
       myHelper::ShowMessageBoxError("数传断开确认时间(秒）设置范围为30-300");
       return;
    }
    i = strValue.toInt();
    memcpy(&f, (void*)&i, sizeof(int));
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(f, (char *)"COM_DL_LOSS_T", MAV_PARAM_TYPE_INT32);

    index = ui->cmb_digitalLossAct_value->currentIndex();
    memcpy(&f, (void*)&index, sizeof(int));
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(f, (char *)"NAV_DLL_ACT", MAV_PARAM_TYPE_INT32);
}

void critActInfo::on_btn_setPosition_clicked()
{
    QString strValue = ui->le_TRA_ROT_DEG_value->text();
    float value = strValue.toFloat();
    if(strValue.length() == 0 || value > 360 || value < 0)
    {
       myHelper::ShowMessageBoxError("RTK航向设置范围为0-360");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"TRA_ROT_DEG",MAV_PARAM_TYPE_REAL32);
    strValue = ui->le_IMU_POS_BODY_X_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 1 || value < -1)
    {
       myHelper::ShowMessageBoxError("陀螺仪X设置范围为-1到1");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"IMU_POS_BODY_X",MAV_PARAM_TYPE_REAL32);
    strValue = ui->le_IMU_POS_BODY_Y_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 1 || value < -1)
    {
       myHelper::ShowMessageBoxError("陀螺仪Y设置范围为-1到1");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"IMU_POS_BODY_Y",MAV_PARAM_TYPE_REAL32);
    strValue = ui->le_IMU_POS_BODY_Z_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 1 || value < -1)
    {
       myHelper::ShowMessageBoxError("陀螺仪Z设置范围为-1到1");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"IMU_POS_BODY_Z",MAV_PARAM_TYPE_REAL32);
    strValue = ui->le_GPS_POS_BODY_X_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 1 || value < -1)
    {
       myHelper::ShowMessageBoxError("GPS X设置范围为-1到1");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"GPS_POS_BODY_X",MAV_PARAM_TYPE_REAL32);
    strValue = ui->le_GPS_POS_BODY_Y_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 1 || value < -1)
    {
       myHelper::ShowMessageBoxError("GPS Y设置范围为-1到1");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"GPS_POS_BODY_Y",MAV_PARAM_TYPE_REAL32);
    strValue = ui->le_GPS_POS_BODY_Z_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 1 || value < -1)
    {
       myHelper::ShowMessageBoxError("GPS Z设置范围为-1到1");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"GPS_POS_BODY_Z",MAV_PARAM_TYPE_REAL32);
    strValue = ui->le_RTK_POS_BODY_X_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 1 || value < -1)
    {
       myHelper::ShowMessageBoxError("RTK X设置范围为-1到1");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"RTK_POS_BODY_X",MAV_PARAM_TYPE_REAL32);
    strValue = ui->le_RTK_POS_BODY_Y_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 1 || value < -1)
    {
       myHelper::ShowMessageBoxError("RTK Y设置范围为-1到1");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"RTK_POS_BODY_Y",MAV_PARAM_TYPE_REAL32);
    strValue = ui->le_RTK_POS_BODY_Z_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 4.5 || value < 0)
    {
       myHelper::ShowMessageBoxError("RTK Z设置范围为-1到1");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"RTK_POS_BODY_Z",MAV_PARAM_TYPE_REAL32);
}

void critActInfo::on_btn_setPID_clicked()
{
    QString strValue = ui->le_MC_ROLL_P_value->text();
    float value = strValue.toFloat();
    if(strValue.length() == 0 || value > 10 || value < 0)
    {
       myHelper::ShowMessageBoxError("滚转自稳P设置范围为0-10");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"MC_ROLL_P",MAV_PARAM_TYPE_REAL32);
    strValue = ui->le_MC_ROLLRATE_P_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 0.6 || value < 0)
    {
       myHelper::ShowMessageBoxError("滚转P设置范围为0-0.6");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"MC_ROLLRATE_P",MAV_PARAM_TYPE_REAL32);
    strValue = ui->le_MC_ROLLRATE_I_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 3 || value < 0)
    {
       myHelper::ShowMessageBoxError("滚转I设置范围为0-3");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"MC_ROLLRATE_I",MAV_PARAM_TYPE_REAL32);
    strValue = ui->le_MC_ROLLRATE_D_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 4 || value < 0)
    {
       myHelper::ShowMessageBoxError("滚转D设置范围为0-4");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"MC_ROLLRATE_D",MAV_PARAM_TYPE_REAL32);
    strValue = ui->le_MC_PITCH_P_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 10 || value < 0)
    {
       myHelper::ShowMessageBoxError("俯仰自稳P设置范围为0-10");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"MC_PITCH_P",MAV_PARAM_TYPE_REAL32);
    strValue = ui->le_MC_PITCHRATE_P_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 0.6 || value < 0)
    {
       myHelper::ShowMessageBoxError("俯仰P设置范围为0-0.6");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"MC_PITCHRATE_P",MAV_PARAM_TYPE_REAL32);
    strValue = ui->le_MC_PITCHRATE_I_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 3 || value < 0)
    {
       myHelper::ShowMessageBoxError("俯仰I设置范围为0-3");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"MC_PITCHRATE_I",MAV_PARAM_TYPE_REAL32);
    strValue = ui->le_MC_PITCHRATE_D_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 4 || value < 0)
    {
       myHelper::ShowMessageBoxError("俯仰D设置范围为0-4");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"MC_PITCHRATE_D",MAV_PARAM_TYPE_REAL32);
    strValue = ui->le_MC_YAW_P_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 4 || value < 0)
    {
       myHelper::ShowMessageBoxError("偏航自稳P设置范围为0-4");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"MC_YAW_P",MAV_PARAM_TYPE_REAL32);
    strValue = ui->le_MC_YAWRATE_P_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 0.6 || value < 0)
    {
       myHelper::ShowMessageBoxError("偏航P设置范围为0-0.6");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"MC_YAWRATE_P",MAV_PARAM_TYPE_REAL32);
    strValue = ui->le_MC_YAWRATE_I_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 2 || value < 0)
    {
       myHelper::ShowMessageBoxError("偏航I设置范围为0-2");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"MC_YAWRATE_I",MAV_PARAM_TYPE_REAL32);
    strValue = ui->le_MC_YAWRATE_D_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 2 || value < 0)
    {
       myHelper::ShowMessageBoxError("偏航D设置范围为0-2");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"MC_YAWRATE_D",MAV_PARAM_TYPE_REAL32);
    strValue = ui->le_MPC_XY_P_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 0.7 || value < 0)
    {
       myHelper::ShowMessageBoxError("水平自稳P设置范围为0-0.7");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"MPC_XY_P",MAV_PARAM_TYPE_REAL32);
    strValue = ui->le_MPC_XY_VEL_P_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 0.2 || value < 0)
    {
       myHelper::ShowMessageBoxError("水平P设置范围为0-0.2");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"MPC_XY_VEL_P",MAV_PARAM_TYPE_REAL32);
    strValue = ui->le_MPC_XY_VEL_I_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 0.1 || value < 0)
    {
       myHelper::ShowMessageBoxError("水平I设置范围为0-0.2");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"MPC_XY_VEL_I",MAV_PARAM_TYPE_REAL32);
    strValue = ui->le_MPC_XY_VEL_D_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 0.1 || value < 0)
    {
       myHelper::ShowMessageBoxError("水平D设置范围为0-0.2");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"MPC_XY_VEL_D",MAV_PARAM_TYPE_REAL32);
    strValue = ui->le_MPC_Z_P_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 1.5 || value < 0)
    {
       myHelper::ShowMessageBoxError("垂直自稳P设置范围为0-1.5");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"MPC_Z_P",MAV_PARAM_TYPE_REAL32);
    strValue = ui->le_MPC_Z_VEL_P_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 0.4 || value < 0)
    {
       myHelper::ShowMessageBoxError("垂直P设置范围为0-0.4");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"MPC_Z_VEL_P",MAV_PARAM_TYPE_REAL32);
    strValue = ui->le_MPC_Z_VEL_I_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 0.1 || value < 0)
    {
       myHelper::ShowMessageBoxError("垂直I设置范围为0-0.1");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"MPC_Z_VEL_I",MAV_PARAM_TYPE_REAL32);

    strValue = ui->le_MPC_Z_VEL_D_value->text();
    value = strValue.toFloat();
    if(strValue.length() == 0 || value > 0.1 || value < 0)
    {
       myHelper::ShowMessageBoxError("垂直D设置范围为0-0.1");
       return;
    }
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(strValue.toFloat(),(char *)"MPC_Z_VEL_D",MAV_PARAM_TYPE_REAL32);

}

void critActInfo::on_btn_resetCritAct_clicked()
{
    ui->le_mainVoltCrit_value->setText("20.00");
    ui->cmb_mainVoltCritAct_value->setCurrentIndex(2);
    ui->le_mainVoltScale_value->setText("1.00");
    ui->le_bakVoltCells_value->setText("11");
    ui->le_bakVoltLow_value->setText("4.00");
    ui->le_bakVoltCrit_value->setText("3.90");
    ui->cmb_bakVoltLowAct_value->setCurrentIndex(0);
    ui->cmb_bakVoltCritAct_value->setCurrentIndex(2);
    ui->le_bakVoltScale_value->setText("1.00");
    ui->le_RCLossTime_value->setText("0.50");
    ui->cmb_RCLossAct_value->setCurrentIndex(2);
    ui->le_digitalLossTime_value->setText("60");
    ui->cmb_digitalLossAct_value->setCurrentIndex(0);
}

void critActInfo::on_btn_resetPosition_clicked()
{
    ui->le_TRA_ROT_DEG_value->setText("0.00");
    ui->le_IMU_POS_BODY_X_value->setText("0.00");
    ui->le_IMU_POS_BODY_Y_value->setText("0.00");
    ui->le_IMU_POS_BODY_Z_value->setText("0.00");
    ui->le_GPS_POS_BODY_X_value->setText("0.00");
    ui->le_GPS_POS_BODY_Y_value->setText("0.00");
    ui->le_GPS_POS_BODY_Z_value->setText("0.00");
    ui->le_RTK_POS_BODY_X_value->setText("0.00");
    ui->le_RTK_POS_BODY_Y_value->setText("0.00");
    ui->le_RTK_POS_BODY_Z_value->setText("0.00");
}

void critActInfo::on_btn_resetPID_clicked()
{
    ui->le_MC_ROLL_P_value->setText("3.50");
    ui->le_MC_ROLLRATE_P_value->setText("0.10");
    ui->le_MC_ROLLRATE_I_value->setText("0.04");
    ui->le_MC_ROLLRATE_D_value->setText("0");
    ui->le_MC_PITCH_P_value->setText("3.50");
    ui->le_MC_PITCHRATE_P_value->setText("0.10");
    ui->le_MC_PITCHRATE_I_value->setText("0.04");
    ui->le_MC_PITCHRATE_D_value->setText("0");
    ui->le_MC_YAW_P_value->setText("4.00");
    ui->le_MC_YAWRATE_P_value->setText("0.50");
    ui->le_MC_YAWRATE_I_value->setText("0");
    ui->le_MC_YAWRATE_D_value->setText("0");
    ui->le_MPC_XY_P_value->setText("0.70");
    ui->le_MPC_XY_VEL_P_value->setText("0.08");
    ui->le_MPC_XY_VEL_I_value->setText("0.02");
    ui->le_MPC_XY_VEL_D_value->setText("0.01");
    ui->le_MPC_Z_P_value->setText("0.80");
    ui->le_MPC_Z_VEL_P_value->setText("0.20");
    ui->le_MPC_Z_VEL_I_value->setText("0.02");
    ui->le_MPC_Z_VEL_D_value->setText("0");
}

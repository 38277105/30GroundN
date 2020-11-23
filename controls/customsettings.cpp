#include "customsettings.h"
#include "ui_customsettings.h"
#include "controllers/parametercontroller.h"

CustomSettings::CustomSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomSettings)
{
    ui->setupUi(this);

    ui->flightmode1->setMinimum(0.0);
    ui->flightmode1->setMaximum(1230.0);

    ui->flightmode2->setMinimum(1231.0);
    ui->flightmode2->setMaximum(1360.0);

    ui->flightmode3->setMinimum(1261.0);
    ui->flightmode3->setMaximum(1490.0);

    ui->flightmode4->setMinimum(1491.0);
    ui->flightmode4->setMaximum(1620.0);

    ui->flightmode5->setMinimum(1621.0);
    ui->flightmode5->setMaximum(1749.0);

    ui->flightmode6->setMinimum(1750.0);
    ui->flightmode6->setMaximum(2000.0);

    ui->slider_n_hspeed->setMaximum(500.0);
    ui->slider_n_distance->setMaximum(500.0);
    ui->slider_n_climb->setMaximum(500.0);
    ui->slider_n_downspeed->setMaximum(500.0);
    ui->slider_n_hoverspeed->setMaximum(500.0);

    ui->slider_r_returnheight->setMaximum(100);
    ui->slider_r_lasthoverheight->setMaximum(100);
    ui->slider_r_lasthovertime->setMaximum(100);
    ui->slider_r_lastdownspeed->setMaximum(30);

    ui->slider_handler_maxspeed->setMaximum(500);
    ui->slider_handler_sense->setMaximum(10);
    ui->slider_handler_safebutton->setMaximum(20);

    ui->slider_stable_pitch_angle->setMaximum(10);
    ui->slider_stable_roll_angle->setMaximum(10);
    ui->slider_stable_yaw_angle->setMaximum(10);
    ui->slider_stable_stablectrl->setMaximum(10);

    ui->slider_pitch_speedctrl_P->setMaximum(10.0);
    ui->slider_pitch_speedctrl_I->setMaximum(10.0);
    ui->slider_pitch_speedctrl_D->setMaximum(10.0);
    ui->slider_pitch_speedctrl_P->setMinimum(0.0);
    ui->slider_pitch_speedctrl_I->setMinimum(0.0);
    ui->slider_pitch_speedctrl_D->setMinimum(0.0);

    ui->slider_roll_speedctrl_P->setMaximum(50.0);
    ui->slider_roll_speedctrl_I->setMaximum(50.0);
    ui->slider_roll_speedctrl_D->setMaximum(50.0);
    ui->slider_roll_speedctrl_P->setMinimum(0.0);
    ui->slider_roll_speedctrl_I->setMinimum(0.0);
    ui->slider_roll_speedctrl_D->setMinimum(0.0);

    ui->slider_yaw_speedctrl_P->setMaximum(50.0);
    ui->slider_yaw_speedctrl_I->setMaximum(50.0);
    ui->slider_yaw_speedctrl_D->setMaximum(50.0);
    ui->slider_yaw_speedctrl_P->setMinimum(0.0);
    ui->slider_yaw_speedctrl_I->setMinimum(0.0);
    ui->slider_yaw_speedctrl_D->setMinimum(0.0);

    ui->slider_stable_speedctrl_P->setMaximum(50.0);
    ui->slider_stable_speedctrl_I->setMaximum(50.0);
    ui->slider_stable_speedctrl_D->setMaximum(50.0);
    ui->slider_stable_speedctrl_P->setMinimum(0.0);
    ui->slider_stable_speedctrl_I->setMinimum(0.0);
    ui->slider_stable_speedctrl_D->setMinimum(0.0);

    ui->throttle_controlacc_P->setMaximum(50.0);
    ui->throttle_controlacc_I->setMaximum(50.0);
    ui->throttle_controlacc_D->setMaximum(50.0);
    ui->throttle_controlspeed_P->setMaximum(50.0);
    ui->throttle_controlacc_P->setMinimum(0.0);
    ui->throttle_controlacc_I->setMinimum(0.0);
    ui->throttle_controlacc_D->setMinimum(0.0);
    ui->throttle_controlspeed_P->setMinimum(0.0);
}

CustomSettings::~CustomSettings()
{
    delete ui;
}
void CustomSettings::valueComing()
{
    SettingManager::Instance()->getFullParameters();

    setcontrol("FLTMODE1",ui->flightmode1);
    setcontrol("FLTMODE2",ui->flightmode2);
    setcontrol("FLTMODE3",ui->flightmode3);
    setcontrol("FLTMODE4",ui->flightmode4);
    setcontrol("FLTMODE5",ui->flightmode5);
    setcontrol("FLTMODE6",ui->flightmode6);

    setcontrol_slider("WPNAV_SPEED",     ui->slider_n_hspeed);
    setcontrol_slider("WPNAV_RADIUS",    ui->slider_n_distance);
    setcontrol_slider("WPNAV_SPEED_UP",  ui->slider_n_climb);
    setcontrol_slider("WPNAV_SPEED_DN",  ui->slider_n_downspeed);
    setcontrol_slider("WPNAV_LOIT_SPEED",ui->slider_n_hoverspeed);

    setcontrol_slider("RTL_ALT",         ui->slider_r_returnheight);
    setcontrol_slider("RTL_ALT_FINAL",   ui->slider_r_lasthoverheight);
    setcontrol_slider("RTL_LOIT_TIME",   ui->slider_r_lasthovertime);
    setcontrol_slider("LAND_SPEED",      ui->slider_r_lastdownspeed);

    setcontrol_slider("PILOT_VELZ_MAX",  ui->slider_handler_maxspeed);
    setcontrol("RC_FEEL_UP",             ui->slider_handler_sense);
    setcontrol("BRD_SAFETYENABLE",       ui->slider_handler_safebutton);

    setcontrol_slider("STB_PIT_P",       ui->slider_stable_pitch_angle);
    setcontrol_slider("STB_RLL_P",       ui->slider_stable_roll_angle);
    setcontrol_slider("STB_YAW_P",       ui->slider_stable_yaw_angle);
    setcontrol_slider("HLD_LAT_P",       ui->slider_stable_stablectrl);

    setcontrol_slider("RATE_PIT_P",      ui->slider_pitch_speedctrl_P);
    setcontrol_slider("RATE_PIT_I",      ui->slider_pitch_speedctrl_I);
    setcontrol_slider("RATE_PIT_D",      ui->slider_pitch_speedctrl_D);

    setcontrol_slider("RATE_RLL_P",      ui->slider_roll_speedctrl_P);
    setcontrol_slider("RATE_RLL_I",      ui->slider_roll_speedctrl_I);
    setcontrol_slider("RATE_RLL_D",      ui->slider_roll_speedctrl_D);

    setcontrol_slider("RATE_YAW_P",      ui->slider_yaw_speedctrl_P);
    setcontrol_slider("RATE_YAW_I",      ui->slider_yaw_speedctrl_I);
    setcontrol_slider("RATE_YAW_D",      ui->slider_yaw_speedctrl_D);

    setcontrol_slider("LOITER_LAT_P/LOITER_LON_P",ui->slider_stable_speedctrl_P);
    setcontrol_slider("LOITER_LAT_I/LOITER_LON_I",ui->slider_stable_speedctrl_I);
    setcontrol_slider("LOITER_LAT_D/LOITER_LON_D",ui->slider_stable_speedctrl_D);

    setcontrol_slider("THR_ACCEL_P",ui->throttle_controlacc_P);
    setcontrol_slider("THR_ACCEL_I",ui->throttle_controlacc_I);
    setcontrol_slider("THR_ACCEL_D",ui->throttle_controlacc_D);
    setcontrol_slider("THR_RATE_P", ui->throttle_controlspeed_P);
}

void CustomSettings::valueSendToPlane()
{
    QMap<QString,ParamData>::iterator itr=m_params.begin();
    for(;itr!=m_params.end();itr++)
    {
        ParamData tmp=*itr;
        if(tmp.IsUpdated==true)
        {
            ParameterController::Instance()->set_parameter_noanswer(tmp.key.toLocal8Bit().data(),(float)tmp.value.toDouble(),MAV_PARAM_TYPE_REAL32);
        }
        (*itr).IsUpdated=false;
    }
}

void CustomSettings::_queryParam(ParamData& param,char* argName)
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

void CustomSettings::on_flightmode1_editingFinished()
{
     shareMethod_DictFillor("FLTMODE1",ui->flightmode1);
}

void CustomSettings::on_flightmode2_editingFinished()
{
     shareMethod_DictFillor("FLTMODE2",ui->flightmode2);
}

void CustomSettings::on_flightmode3_editingFinished()
{
     shareMethod_DictFillor("FLTMODE3",ui->flightmode3);
}

void CustomSettings::on_flightmode4_editingFinished()
{
     shareMethod_DictFillor("FLTMODE4",ui->flightmode4);
}

void CustomSettings::on_flightmode5_editingFinished()
{
     shareMethod_DictFillor("FLTMODE5",ui->flightmode5);
}

void CustomSettings::on_flightmode6_editingFinished()
{
     shareMethod_DictFillor("FLTMODE6",ui->flightmode6);
}

void CustomSettings::shareMethod_DictFillor(QString key,QDoubleSpinBox* boxcontrol)
 {
     if(m_params.contains(key))
     {
         ParamData t_data;
         t_data.key=key;
         t_data.value=boxcontrol->value();
         m_params[key]=t_data;
         m_params[key].IsUpdated=true;
     }
     else
     {
          m_params[key].value=boxcontrol->value();
          m_params[key].IsUpdated=true;
     }
 }

void CustomSettings::on_slider_n_hspeed_valueChanged(int value)
{
    shareMethod_DictFillor_slider("WPNAV_SPEED",ui->slider_n_hspeed);
}
void CustomSettings::on_slider_n_distance_valueChanged(int value)
{
    shareMethod_DictFillor_slider("WPNAV_RADIUS",ui->slider_n_distance);
}

void CustomSettings::on_slider_n_climb_valueChanged(int value)
{
    shareMethod_DictFillor_slider("WPNAV_SPEED_UP",ui->slider_n_climb);
}

void CustomSettings::on_slider_n_downspeed_valueChanged(int value)
{
    shareMethod_DictFillor_slider("WPNAV_SPEED_DN",ui->slider_n_downspeed);
}
void CustomSettings::on_slider_n_hoverspeed_valueChanged(int value)
{
    shareMethod_DictFillor_slider("WPNAV_LOIT_SPEED",ui->slider_n_hoverspeed);
}

void CustomSettings::shareMethod_DictFillor_slider(QString key,QSlider* boxcontrol)
{
    if( m_params.contains(key))
    {
        ParamData t_data;
        t_data.key=key;
        t_data.value=boxcontrol->value();
        m_params[key]=t_data;
        m_params[key].IsUpdated=true;
    }
    else
    {
         m_params[key].value=boxcontrol->value();
         m_params[key].IsUpdated=true;
    }
}

void CustomSettings::on_slider_r_returnheight_valueChanged(int value)
{

        shareMethod_DictFillor_slider("RTL_ALT",ui->slider_r_returnheight);
}

void CustomSettings::on_slider_r_lasthoverheight_valueChanged(int value)
{
 shareMethod_DictFillor_slider("RTL_ALT_FINAL",ui->slider_r_lasthoverheight);
}

void CustomSettings::on_slider_r_lasthovertime_valueChanged(int value)
{
 shareMethod_DictFillor_slider("RTL_LOIT_TIME",ui->slider_r_lasthovertime);
}

void CustomSettings::on_slider_r_lastdownspeed_valueChanged(int value)
{
 shareMethod_DictFillor_slider("LAND_SPEED",ui->slider_r_lastdownspeed);
}
//-----------------stick handler----------------------
void CustomSettings::on_slider_handler_maxspeed_valueChanged(int value)
{
 shareMethod_DictFillor_slider("PILOT_VELZ_MAX",ui->slider_handler_maxspeed);
}

void CustomSettings::on_slider_handler_sense_editingFinished()
{
shareMethod_DictFillor("RC_FEEL_UP",ui->slider_handler_sense);
}

void CustomSettings::on_slider_handler_safebutton_editingFinished()
{
shareMethod_DictFillor("BRD_SAFETYENABLE",ui->slider_handler_safebutton);
}

//-----------------------stable mode----------------
void CustomSettings::on_slider_stable_pitch_angle_valueChanged(int value)
{
    shareMethod_DictFillor_slider("STB_PIT_P",ui->slider_stable_pitch_angle);
}

void CustomSettings::on_slider_stable_roll_angle_valueChanged(int value)
{
    shareMethod_DictFillor_slider("STB_RLL_P",ui->slider_stable_roll_angle);
}

void CustomSettings::on_slider_stable_yaw_angle_valueChanged(int value)
{
    shareMethod_DictFillor_slider("STB_YAW_P",ui->slider_stable_yaw_angle);
}

void CustomSettings::on_slider_stable_stablectrl_valueChanged(int value)
{
    shareMethod_DictFillor_slider("HLD_LAT_P",ui->slider_stable_stablectrl);
}
//-----------------------pitch angle --PID----------------

void CustomSettings::on_slider_pitch_speedctrl_P_valueChanged(int value)
{

    shareMethod_DictFillor_slider("RATE_PIT_P",ui->slider_pitch_speedctrl_P);
}

void CustomSettings::on_slider_pitch_speedctrl_I_valueChanged(int value)
{
    shareMethod_DictFillor_slider("RATE_PIT_I",ui->slider_pitch_speedctrl_I);
}

void CustomSettings::on_slider_pitch_speedctrl_D_valueChanged(int value)
{
    shareMethod_DictFillor_slider("RATE_PIT_D",ui->slider_pitch_speedctrl_D);
}


//----------------------ROLL PID----------------------
void CustomSettings::on_slider_roll_speedctrl_P_valueChanged(int value)
{
    shareMethod_DictFillor_slider("RATE_RLL_P",ui->slider_roll_speedctrl_P);
}

void CustomSettings::on_slider_roll_speedctrl_I_valueChanged(int value)
{
    shareMethod_DictFillor_slider("RATE_RLL_I",ui->slider_roll_speedctrl_I);
}

void CustomSettings::on_slider_roll_speedctrl_D_valueChanged(int value)
{
    shareMethod_DictFillor_slider("RATE_RLL_D",ui->slider_roll_speedctrl_D);
}

//----------------------YAW PID----------------------
void CustomSettings::on_slider_yaw_speedctrl_P_valueChanged(int value)
{
    shareMethod_DictFillor_slider("RATE_YAW_P",ui->slider_yaw_speedctrl_P);
}

void CustomSettings::on_slider_yaw_speedctrl_I_valueChanged(int value)
{
    shareMethod_DictFillor_slider("RATE_YAW_I",ui->slider_yaw_speedctrl_I);
}

void CustomSettings::on_slider_yaw_speedctrl_D_valueChanged(int value)
{
    shareMethod_DictFillor_slider("RATE_YAW_D",ui->slider_yaw_speedctrl_D);
}
//----------------------Stable PID----------------------
void CustomSettings::on_slider_stable_speedctrl_P_valueChanged(int value)
{
   shareMethod_DictFillor_slider("LOITER_LAT_P/LOITER_LON_P",ui->slider_stable_speedctrl_P);
}

void CustomSettings::on_slider_stable_speedctrl_I_valueChanged(int value)
{
    shareMethod_DictFillor_slider("LOITER_LAT_I/LOITER_LON_I",ui->slider_stable_speedctrl_I);
}

void CustomSettings::on_slider_stable_speedctrl_D_valueChanged(int value)
{
    shareMethod_DictFillor_slider("LOITER_LAT_D/LOITER_LON_D",ui->slider_stable_speedctrl_D);
}

void CustomSettings::on_throttle_controlacc_P_valueChanged(int value)
{

    shareMethod_DictFillor_slider("THR_ACCEL_P",ui->throttle_controlacc_P);
}

void CustomSettings::on_throttle_controlacc_I_valueChanged(int value)
{
    shareMethod_DictFillor_slider("THR_ACCEL_I",ui->throttle_controlacc_I);

}

void CustomSettings::on_throttle_controlacc_D_valueChanged(int value)
{
    shareMethod_DictFillor_slider("THR_ACCEL_D",ui->throttle_controlacc_D);
}

void CustomSettings::on_throttle_controlspeed_P_valueChanged(int value)
{
    shareMethod_DictFillor_slider("THR_RATE_P",ui->throttle_controlspeed_P);
}

void CustomSettings::setcontrol(QString key,QDoubleSpinBox* boxcontrol)
{
    ParamData t_data;
    _queryParam(t_data,(char*)key.toLocal8Bit().data());
    double id_value=t_data.value.toDouble();
    boxcontrol->setValue(id_value);

    qDebug()<<key<<":"<<id_value<<"------------";
}

void CustomSettings::setcontrol_slider(QString key,QSlider* boxcontrol)
{

    ParamData t_data;
    _queryParam(t_data,(char*)key.toLocal8Bit().data());
    double id_value=(double)t_data.value.toDouble();
    boxcontrol->setValue(id_value);

    if(key==QString::fromLocal8Bit("RATE_PIT_P"))
    {
       qDebug()<<t_data.value<<"hello world .....xjw debug info...";
    }

}

void CustomSettings::on_btn_Save_clicked()
{
    valueSendToPlane();

}

void CustomSettings::on_btn_Loadargs_clicked()
{
    valueComing();
    qDebug()<<"setting control"<<"end";
}

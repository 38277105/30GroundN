#include "accelcalibrationofindependent.h"
#include "ui_accelcalibrationofindependent.h"
#include "controllers/frmmaincontroller.h"
#include "connections/mavlinkmessageinterface.h"
#include <synchapi.h>
AccelCalibrationOfIndependent::AccelCalibrationOfIndependent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AccelCalibrationOfIndependent)
{
    ui->setupUi(this);
    m_cutdownCount = CALIBRATION_TIMEOUT_SEC;
    m_uavState = 0;
    memset(m_uavRawData, 0, sizeof(m_uavRawData));
    connect(ui->btn_next,SIGNAL(clicked()),this,SLOT(calibrateButtonClicked()));
    connect(&m_cutdownTimer,SIGNAL(timeout()),this,SLOT(cutdownTimerTick()));
    for(int i=0; i<3; i++){
        m_accel_offsets[i] = 0.0f;
        m_accel_scale[i] = 0.0f;
    }
    m_setParamIndex = 0;
    m_accumulativeSendNum = 0;
}

AccelCalibrationOfIndependent::~AccelCalibrationOfIndependent()
{
    delete ui;
}

void AccelCalibrationOfIndependent::calibrateButtonClicked(){
//    Calibration();
//    if(!ZYGroundGlobalConfig::m_SerialIsOpen)
//    {
//        myHelper::ShowMessageBoxInfo("请先连接无人机，再进行校准");
//        return;
//    }
    ui->lb_tipText->clear();
    if(m_uavState > 11){
        m_cutdownTimer.stop();
        Calibration();
        ui->lb_cutdownText->setText("");
        setRawIMUFrequency(1000);
        return;
    }
    if(0 == m_uavState)
    {
        setRawIMUFrequency(100);
    }

    if(m_uavState%2 == 0){
        nextFace(m_uavState++);
        ui->lb_cutdownText->setText("");
        ui->btn_next->setText(QString::fromLocal8Bit("点击\n开始采集数据"));
    }else{
        m_cutdownCount = CALIBRATION_TIMEOUT_SEC;
        int uav_id=FrmMainController::Instance()->__vehicle->m_State.m_Id;
        QString tempStr=QString::fromLocal8Bit("<h3>校准MAV")+QString("%1").arg(uav_id)+QString::fromLocal8Bit("超时时间剩余: <b>")+QString("%1").arg(m_cutdownCount--);
        ui->lb_cutdownText->setText(tempStr);
        ui->btn_next->setText(QString::fromLocal8Bit("正在采集\n请等待"));
        m_cutdownTimer.start(2000);
        m_maxSampleNum = 0;
        Sleep(1000);
        connect(&FrmMainController::Instance()->__vehicle->mavLinkMessageInterface,SIGNAL(calibrate_raw_imu_recved(mavlink_raw_imu_t)),this, SLOT(MavRawImu(mavlink_raw_imu_t)));
    }
}

void AccelCalibrationOfIndependent::cutdownTimerTick()
{
    if(FrmMainController::Instance()->__vehicle!=NULL)
    {
       int uav_id=FrmMainController::Instance()->__vehicle->m_State.m_Id;
       QString tempStr=QString::fromLocal8Bit("<h3>校准MAV")+QString("%1").arg(uav_id)+QString::fromLocal8Bit("超时时间剩余: <b>")+QString("%1").arg(m_cutdownCount--);
       ui->lb_cutdownText->setText(tempStr);
       if (m_cutdownCount <= 0)
        {
            ui->lb_cutdownText->setText(QString::fromLocal8Bit("命令超时,请重新再试."));
            ui->lb_tipText->setText("");
            reCalibration(false);
        }
    }
}

void AccelCalibrationOfIndependent::MavRawImu(mavlink_raw_imu_t message){
    if(m_maxSampleNum == MAXSAMPLENUM | m_maxSampleNum > MAXSAMPLENUM){
        disconnect(&FrmMainController::Instance()->__vehicle->mavLinkMessageInterface,SIGNAL(calibrate_raw_imu_recved(mavlink_raw_imu_t)),this, SLOT(MavRawImu(mavlink_raw_imu_t)));
        m_uavRawData[m_uavState/2][0] /= MAXSAMPLENUM;
        m_uavRawData[m_uavState/2][1] /= MAXSAMPLENUM;
        m_uavRawData[m_uavState/2][2] /= MAXSAMPLENUM;
        qDebug() << QString::fromLocal8Bit("量化后：") << m_uavRawData[m_uavState/2][0]*(GRAVITY_MSS/4096)\
                << "/" << m_uavRawData[m_uavState/2][1]*(GRAVITY_MSS/4096)\
                << "/" << m_uavRawData[m_uavState/2][2]*(GRAVITY_MSS/4096);
        ui->btn_next->setText(QString::fromLocal8Bit("采集数据完毕\n点击继续 "));
        nextFace(m_uavState++);
        emit ui->btn_next->clicked();
        return;
    }
    qDebug() << m_maxSampleNum;
    m_uavRawData[m_uavState/2][0] += message.xacc;
    m_uavRawData[m_uavState/2][1] += message.yacc;
    m_uavRawData[m_uavState/2][2] += message.zacc;
    m_maxSampleNum++;
}

void AccelCalibrationOfIndependent::PostBackParamValue(mavlink_param_value_t paramValue){
    //qDebug() << "postback: " << paramValue.param_id << "/" << paramValue.param_value << "/" << m_setParamIndex << "\n";
    if(isSetParamSuccess(m_setParamIndex, paramValue)){
        if(m_setParamIndex == 5){
            ui->lb_cutdownText->setText(QString::fromLocal8Bit("更新飞控数据飞控成功"));
            connect(&(FrmMainController::Instance()->__vehicle->mavLinkMessageInterface),SIGNAL(onParamValue(mavlink_param_value_t)),FrmMainController::Instance()->m_paramcontroller,SLOT(onParamValue(mavlink_param_value_t)));
            disconnect(&(FrmMainController::Instance()->__vehicle->mavLinkMessageInterface),SIGNAL(onParamValue(mavlink_param_value_t)),this,SLOT(PostBackParamValue(mavlink_param_value_t)));
            setRawIMUFrequency(100);
            return;
        }
        m_setParamIndex++;
        m_accumulativeSendNum = 0;
        setParamOfCaliData(m_setParamIndex);
        return;
    }
    m_accumulativeSendNum++;
    if(m_accumulativeSendNum > 2){
       ui->lb_cutdownText->setText(QString::fromLocal8Bit("更新飞控数据飞控失败"));
       connect(&(FrmMainController::Instance()->__vehicle->mavLinkMessageInterface),SIGNAL(onParamValue(mavlink_param_value_t)),FrmMainController::Instance()->m_paramcontroller,SLOT(onParamValue(mavlink_param_value_t)));
       disconnect(&(FrmMainController::Instance()->__vehicle->mavLinkMessageInterface),SIGNAL(onParamValue(mavlink_param_value_t)),this,SLOT(PostBackParamValue(mavlink_param_value_t)));
       return;
    }
    {
       Sleep(1000);
       qDebug() << "chong fa: " << m_accumulativeSendNum;
       setParamOfCaliData(m_setParamIndex);
    }
}

void AccelCalibrationOfIndependent::nextFace(int index){
    switch (index) {
    case 0:
        ui->lb_tipPicture->setStyleSheet("QLabel{border-image:url(:/image/calibration/accel_down.png)}");
        ui->lb_tipText->setText(QString::fromLocal8Bit("请将飞机水平放置."));
        break;
    case 2:
        ui->lb_tipPicture->setStyleSheet("QLabel{border-image:url(:/image/calibration/accel_left.png)}");
        ui->lb_tipText->setText(QString::fromLocal8Bit("请将飞机左侧朝下放置."));
        break;
    case 4:
        ui->lb_tipPicture->setStyleSheet("QLabel{border-image:url(:/image/calibration/accel_right.png)}");
        ui->lb_tipText->setText(QString::fromLocal8Bit("请将飞机右侧朝下放置."));
        break;
    case 6:
        ui->lb_tipPicture->setStyleSheet("QLabel{border-image:url(:/image/calibration/accel_front.png)}");
        ui->lb_tipText->setText(QString::fromLocal8Bit("请将飞机头朝下放置."));
        break;
    case 8:
        ui->lb_tipPicture->setStyleSheet("QLabel{border-image:url(:/image/calibration/accel_back.png)}");
        ui->lb_tipText->setText(QString::fromLocal8Bit("请将飞机头朝上放置."));
        break;
    case 10:
        ui->lb_tipPicture->setStyleSheet("QLabel{border-image:url(:/image/calibration/accel_up.png)}");
        ui->lb_tipText->setText(QString::fromLocal8Bit("请将飞机背面朝上放置."));
        break;
    default:
        break;
    }
}


void AccelCalibrationOfIndependent::Calibration(){
    float accel_offsets[3] = {0};
    float accel_scale[3] = {0};
    for(int i=0;i<6;i++){
        m_uavRawData[i][0] *= (GRAVITY_MSS/4096);
        m_uavRawData[i][1] *= (GRAVITY_MSS/4096);
        m_uavRawData[i][2] *= (GRAVITY_MSS/4096);
    }
//    m_uavRawData[0][0] = 0.180164f;
//    m_uavRawData[0][1] = 0.172383f;
//    m_uavRawData[0][2] = -9.89262f;

//    m_uavRawData[1][0] = 0.0264859f;
//    m_uavRawData[1][1] = 9.79012f;
//    m_uavRawData[1][2] = 0.341922f;

//    m_uavRawData[2][0] = 0.107141f;
//    m_uavRawData[2][1] = -9.84421f;
//    m_uavRawData[2][2] = 0.174253f;

//    m_uavRawData[3][0] = -9.81548f;
//    m_uavRawData[3][1] = -0.104597f;
//    m_uavRawData[3][2] = 0.166098f;

//    m_uavRawData[4][0] = 9.81885f;
//    m_uavRawData[4][1] = 0.0418237f;
//    m_uavRawData[4][2] = 0.725443f;

//    m_uavRawData[5][0] = -0.15465f;
//    m_uavRawData[5][1] = -0.128464f;
//    m_uavRawData[5][2] = 9.961f;

    if (!calibrate_accel(m_uavRawData, accel_offsets, accel_scale)){
        reCalibration(false);
        return;
    }else{
        for(int i=0; i<3; i++){
            m_accel_offsets[i] = accel_offsets[i];
            m_accel_scale[i] = accel_scale[i];
        }
        reCalibration(true);
        sendCaliData();
    }
}

// _calibrate_reset_matrices - clears matrices
void calibrate_reset_matrices(float (&dS)[6], float (&JS)[6][6]){
    int j, k;
    for (j = 0; j < 6; j++){
        dS[j] = 0.0f;
        for (k = 0; k < 6; k++){
            JS[j][k] = 0.0f;
        }
    }
}

void calibrate_update_matrices(float (&dS)[6], float(&JS)[6][6], float (&beta)[6], float (&data)[3]){
    int j, k;
    float dx, b;
    float residual = 1.0f;
    float jacobian[6] = {0};

    for (j = 0; j < 3; j++){
        b = beta[3 + j];
        dx = (float)data[j] - beta[j];
        residual -= b * b * dx * dx;
        jacobian[j] = 2.0f * b * b * dx;
        jacobian[3 + j] = -2.0f * b * dx * dx;
    }

    for (j = 0; j < 6; j++){
        dS[j] += jacobian[j] * residual;
        for (k = 0; k < 6; k++){
            JS[j][k] += jacobian[j] * jacobian[k];
        }
     }
}

void calibrate_find_delta(float (&dS)[6], float (&JS)[6][6], float (&delta)[6] ){
    //Solve 6-d matrix equation JS*x = dS
    //first put in upper triangular form
    int i, j, k;
    float mu;

    //make upper triangular
    for (i = 0; i < 6; i++){
        //eliminate all nonzero entries below JS[i][i]
        for (j = i + 1; j < 6; j++){
            mu = JS[i][j] / JS[i][i];
            if (mu != 0.0f){
                dS[j] -= mu * dS[i];
                for (k = j; k < 6; k++){
                    JS[k][j] -= mu * JS[k][i];
                }
            }
        }
    }
    //back-substitute
    for (i = 5; i >= 0; i--){
        dS[i] /= JS[i][i];
        JS[i][i] = 1.0f;

        for (j = 0; j < i; j++){
            mu = JS[i][j];
            dS[j] -= mu * dS[i];
            JS[i][j] = 0.0f;
         }
     }
    for (i = 0; i < 6; i++){
        delta[i] = dS[i];
    }
}

bool AccelCalibrationOfIndependent::calibrate_accel(float (&accel_sample)[6][3], float (&accel_offsets)[3] , float (&accel_scale)[3] ){
    int i;
    int num_iterations = 0;
    float eps = 0.000000001f;
    float change = 100.0f;
    float data[3]  = {0};
    float beta[6]  = {0};
    float delta[6] = {0};
    float ds[6] = {0};
    float JS[6][6] = {0};
    bool success = true;

    // reset
    beta[0] = beta[1] = beta[2] = 0;
    beta[3] = beta[4] = beta[5] = 1.0f / GRAVITY_MSS;

    while (num_iterations < 20 && change > eps){
        num_iterations++;
        calibrate_reset_matrices(ds, JS);
        for (i = 0; i < 6; i++){
            data[0] = accel_sample[i][0];
            data[1] = accel_sample[i][1];
            data[2] = accel_sample[i][2];
            calibrate_update_matrices(ds, JS, beta, data);
        }
        calibrate_find_delta(ds,JS, delta);
        change = delta[0] * delta[0] +
                delta[0] * delta[0] +
                delta[1] * delta[1] +
                delta[2] * delta[2] +
                delta[3] * delta[3] / (beta[3] * beta[3]) +
                delta[4] * delta[4] / (beta[4] * beta[4]) +
                delta[5] * delta[5] / (beta[5] * beta[5]);

        for (i = 0; i < 6; i++){
            beta[i] -= delta[i];
        }
    }
    accel_scale[0] = beta[3] * GRAVITY_MSS;
    accel_scale[1] = beta[4] * GRAVITY_MSS;
    accel_scale[2] = beta[5] * GRAVITY_MSS;
    accel_offsets[0] = beta[0] * accel_scale[0];
    accel_offsets[1] = beta[1] * accel_scale[1];
    accel_offsets[2] = beta[2] * accel_scale[2];

    //qDebug() << accel_scale[0] << "/" << accel_scale[1] << "/" << accel_scale[2];
    //qDebug() << accel_offsets[0] << "/" << accel_offsets[1] << "/" << accel_offsets[2];
    // sanity check scale
    if ((accel_scale[0] == 0 || accel_scale[1] == 0 || accel_scale[2] == 0) || abs(accel_scale[0] - 1.0f) > 0.1f || abs(accel_scale[1] - 1.0f) > 0.1f || abs(accel_scale[2] - 1.0f) > 0.1f){
        success = false;
    }
    // sanity check offsets (3.5 is roughly 3/10th of a G, 5.0 is roughly half a G)
    if ((accel_offsets[0] == 0 || accel_offsets[1] == 0 || accel_offsets[2] == 0) || abs(accel_offsets[0]) > 3.5f || abs(accel_offsets[1]) > 3.5f || abs(accel_offsets[2]) > 3.5f){
        success = false;
    }

    // return success or failure
    return success;
}


void AccelCalibrationOfIndependent::setParamOfCaliData(int index){
    switch (index) {
    case 0:
    {
        QString tmp(ACC_XOFF);
        //FrmMainController::Instance()->set_parameter(tmp.toLocal8Bit().data(), m_accel_offsets[0],MAV_PARAM_TYPE_REAL32);
        ParameterController::Instance()->set_parameter_noanswer(tmp.toLocal8Bit().data(), m_accel_offsets[0],MAV_PARAM_TYPE_REAL32);
        //qDebug() << "send off x:" << m_accel_offsets[0];
        break;
    }
    case 1:
    {
        QString tmp(ACC_YOFF);
        ParameterController::Instance()->set_parameter_noanswer(tmp.toLocal8Bit().data(), m_accel_offsets[1],MAV_PARAM_TYPE_REAL32);
        //qDebug() << "send off y:" << m_accel_offsets[1];
        break;
    }
    case 2:
    {
        QString tmp(ACC_ZOFF);
        ParameterController::Instance()->set_parameter_noanswer(tmp.toLocal8Bit().data(), m_accel_offsets[2],MAV_PARAM_TYPE_REAL32);
        //qDebug() << "send off z:" << m_accel_offsets[2];
        break;
    }
    case 3:
    {
        QString tmp(ACC_XSCALE);
        ParameterController::Instance()->set_parameter_noanswer(tmp.toLocal8Bit().data(), m_accel_scale[0],MAV_PARAM_TYPE_REAL32);
       // qDebug() << "send scale x:" << m_accel_scale[0];
        break;
    }
    case 4:
    {
        QString tmp(ACC_YSCALE);
        ParameterController::Instance()->set_parameter_noanswer(tmp.toLocal8Bit().data(), m_accel_scale[1],MAV_PARAM_TYPE_REAL32);
        //qDebug() << "send scale y:" << m_accel_scale[1];
        break;
    }
    case 5:
    {
        QString tmp(ACC_ZSCALE);
        ParameterController::Instance()->set_parameter_noanswer(tmp.toLocal8Bit().data(), m_accel_scale[2],MAV_PARAM_TYPE_REAL32);
        //qDebug() << "send scale z:" << m_accel_scale[2];
        break;
    }
    default:
        break;
    }
}

void AccelCalibrationOfIndependent::sendCaliData(){
    disconnect(&(FrmMainController::Instance()->__vehicle->mavLinkMessageInterface),SIGNAL(onParamValue(mavlink_param_value_t)),FrmMainController::Instance()->m_paramcontroller,SLOT(onParamValue(mavlink_param_value_t)));
    connect(&(FrmMainController::Instance()->__vehicle->mavLinkMessageInterface),SIGNAL(onParamValue(mavlink_param_value_t)),this,SLOT(PostBackParamValue(mavlink_param_value_t)));
    //setParamOfCaliData(m_setParamIndex);
    setParamOfCaliData(0);
    setParamOfCaliData(1);
    setParamOfCaliData(2);
    setParamOfCaliData(3);
    setParamOfCaliData(4);
    setParamOfCaliData(5);
}

int AccelCalibrationOfIndependent::isSetParamSuccess(int index, mavlink_param_value_t paramValue){
    switch (index) {
    case 0:
        if(m_accel_offsets[0] == paramValue.param_value && !strcmp(ACC_XOFF, paramValue.param_id)){
            return true;
        }else{
            return false;
        }
        break;
    case 1:
        if(m_accel_offsets[1] == paramValue.param_value && !strcmp(ACC_YOFF, paramValue.param_id)){
            return true;
        }else{
            return false;
        }
        break;
    case 2:
        if(m_accel_offsets[2] == paramValue.param_value && !strcmp(ACC_ZOFF, paramValue.param_id)){
            return true;
        }else{
            return false;
        }
        break;
    case 3:
        if(m_accel_scale[0] == paramValue.param_value && !strcmp(ACC_XSCALE, paramValue.param_id)){
            return true;
        }else{
            return false;
        }
        break;
    case 4:
        if(m_accel_scale[1] == paramValue.param_value && !strcmp(ACC_YSCALE, paramValue.param_id)){
            return true;
        }else{
            return false;
        }
        break;
    case 5:
        if(m_accel_scale[2] == paramValue.param_value && !strcmp(ACC_ZSCALE, paramValue.param_id)){
            return true;
        }else{
            return false;
        }
        break;
    default:
        break;
    }
    return false;
}

void AccelCalibrationOfIndependent::reCalibration(bool flag){
    if(flag){//校准成功 重新校准
        QString tmp = "off:x["+QString::number(m_accel_offsets[0], 'f') +"]" \
                + " y[" + QString::number(m_accel_offsets[1], 'f') +"]" \
                + " z["+QString::number(m_accel_offsets[2], 'f') +"]\n"\
                + "scale:x["+QString::number(m_accel_scale[0], 'f') +"]" \
                + " y[" + QString::number(m_accel_scale[1], 'f') +"]" \
                + " z["+QString::number(m_accel_scale[2], 'f') +"]\n";
        ui->lb_tipText->setText(tmp+QString::fromLocal8Bit("校准成功"));
        ui->lb_cutdownText->setText(QString::fromLocal8Bit("正在更新飞控数据"));
    }else{//校准失败 重新校准
        ui->lb_tipText->setText(QString::fromLocal8Bit("校准失败"));
    }
    m_cutdownTimer.stop();
    m_uavState = 0;
    ui->btn_next->setText(QString::fromLocal8Bit("重新校准"));
}

void AccelCalibrationOfIndependent::setRawIMUFrequency(int nf){
    float f;
    memcpy(&f, (void*)&nf, sizeof(int));
    FrmMainController::Instance()->set_parameter((char*)"DL_MSG_GRP3_MS", f,MAV_PARAM_TYPE_INT32);
}


#ifndef ACCELCALIBRATIONOFINDEPENDENT_H
#define ACCELCALIBRATIONOFINDEPENDENT_H

#include <QWidget>
#include <QTimer>
#include "connections/mavlinkmessageinterface.h"
namespace Ui {
class AccelCalibrationOfIndependent;
}
enum FaceType{
    backup,     // 背面向上即水平
    headup,     //机头向上
    rightdown,  //右翼向下
    headdown,   //机头向下
    leftdown,   //左翼向下
    backdown    // 背面向下
};

#define MAXSAMPLENUM    32.0f
#define GRAVITY_MSS     9.80665f
#define ACC_XOFF    "CAL_ACC0_XOFF"
#define ACC_YOFF    "CAL_ACC0_YOFF"
#define ACC_ZOFF    "CAL_ACC0_ZOFF"
#define ACC_XSCALE  "CAL_ACC0_XSCALE"
#define ACC_YSCALE  "CAL_ACC0_YSCALE"
#define ACC_ZSCALE  "CAL_ACC0_ZSCALE"

class AccelCalibrationOfIndependent : public QWidget
{
    Q_OBJECT
    static const int CALIBRATION_TIMEOUT_SEC = 40;
public:
    explicit AccelCalibrationOfIndependent(QWidget *parent = 0);
    ~AccelCalibrationOfIndependent();
private slots:
    void calibrateButtonClicked();
    void cutdownTimerTick();
    void MavRawImu(mavlink_raw_imu_t message);
    void PostBackParamValue(mavlink_param_value_t paramValue);

private:
    void nextFace(int index);
    void Calibration();
    bool calibrate_accel(float (&accel_sample)[6][3], float (&accel_offsets)[3] , float (&accel_scale)[3] );
    void setParamOfCaliData(int index);
    void sendCaliData();
    int isSetParamSuccess(int index, mavlink_param_value_t paramValue);
    void reCalibration(bool flag);
    void setRawIMUFrequency(int nf);
private:
    Ui::AccelCalibrationOfIndependent *ui;
    QTimer m_cutdownTimer;
    int m_uavState;
    int m_cutdownCount;
    int m_maxSampleNum;
    float m_uavRawData[6][3];
    float m_accel_offsets[3];
    float m_accel_scale[3];
    int  m_setParamIndex;
    int m_accumulativeSendNum;
};

#endif // ACCELCALIBRATIONOFINDEPENDENT_H

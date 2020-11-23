/*=====================================================================
 
 QGroundControl Open Source Ground Control Station
 
 (c) 2009, 2015 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 
 This file is part of the QGROUNDCONTROL project
 
 QGROUNDCONTROL is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 QGROUNDCONTROL is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with QGROUNDCONTROL. If not, see <http://www.gnu.org/licenses/>.
 
 ======================================================================*/

#ifndef APMCompassCal_H
#define APMCompassCal_H

#include <QObject>
#include <QThread>
#include <QVector3D>
#include "connections/mavlinkmessageinterface.h"
#include "controllers/frmmaincontroller.h"
#include "controllers/parametercontroller.h"

#define MAX_MAGS 3         //罗盘数量
#define MAG_PARA_NUM 11     //每个罗盘参数个数
#define SIDE_COUNT 6       //需要识别的面数
class CalWorkerThread : public QThread
{
    Q_OBJECT

public:
    CalWorkerThread(QObject* parent = NULL);

    // Cancel currently in progress calibration
    void cancel(void) { _cancel = true; }
    void _ParamSet(const char* parId,int val);
    void _ParamSet(const char* parId,float val);

    // Overrides from QThread
    void run(void) Q_DECL_FINAL;

    bool                    rgCompassAvailable[MAX_MAGS];    //罗盘是否可用的标识
    QMutex                  lastScaledImuMutex;
    mavlink_raw_imu_t       lastRawImu;                      //上一次的IMU数据 三轴的加速度，角速度，磁场域
    mavlink_scaled_imu_t    rgLastScaledImu[MAX_MAGS];       //三个罗盘上一次的ScaleIMU数据

    static const char*      rgCompassParams[MAX_MAGS][MAG_PARA_NUM];

signals:
    void vehicleTextMessage(int vehicleId, int compId, int severity, QString text);
    void sendCompensationValue_signal(float x, float y, float z); //发送计算结果-补偿值
private:
    void _emitVehicleTextMessage(const QString& message);

    static const float mag_sphere_radius;                   //磁的球半径
    static const unsigned int calibration_sides;			//总的面数
    static const unsigned int calibration_total_points;     //每个磁传感器需要的点数
    static const unsigned int calibraton_duration_seconds;  //校验允许的总时间

    enum detect_orientation_return {
        DETECT_ORIENTATION_TAIL_DOWN,
        DETECT_ORIENTATION_NOSE_DOWN,
        DETECT_ORIENTATION_LEFT,
        DETECT_ORIENTATION_RIGHT,
        DETECT_ORIENTATION_UPSIDE_DOWN,
        DETECT_ORIENTATION_RIGHTSIDE_UP,
        DETECT_ORIENTATION_ERROR
    };
    // Data passed to calibration worker routine
    typedef struct  {
        unsigned        done_count;
        unsigned int	calibration_points_perside;
        unsigned int	calibration_interval_perside_seconds;
        uint64_t        calibration_interval_perside_useconds;
        unsigned int	calibration_counter_total[MAX_MAGS];
        bool            side_data_collected[SIDE_COUNT];
        float*          x[MAX_MAGS];
        float*          y[MAX_MAGS];
        float*          z[MAX_MAGS];
    } mag_worker_data_t;

    enum calibrate_return {
        calibrate_return_ok,
        calibrate_return_error,
        calibrate_return_cancelled
    };


    int sphere_fit_least_squares(const float x[], const float y[], const float z[],
                     unsigned int size, unsigned int max_iterations, float delta, float *sphere_x, float *sphere_y, float *sphere_z,
                     float *sphere_radius);


    enum detect_orientation_return detect_orientation(void);

    const char* detect_orientation_str(enum detect_orientation_return orientation);


    calibrate_return calibrate_from_orientation(
                            bool	side_data_collected[SIDE_COUNT],
                            void*	worker_data);

    calibrate_return calibrate(void);
    calibrate_return mag_calibration_worker(detect_orientation_return orientation, void* data);
    unsigned progress_percentage(mag_worker_data_t* worker_data);

    bool        _cancel;

    void _queryParam(ParamData& param,char* argName);
};


class APMCompassCal : public QObject
{
    Q_OBJECT
    
public:
    APMCompassCal(void);
    ~APMCompassCal();

    void startCalibration(bool bMag1, bool bMag2=false,bool bMag0=false);
    void cancelCalibration(void);
    
signals:
    void vehicleTextMessage(int vehicleId, int compId, int severity, QString text);
    void sendCompensationValue_signal(float,float,float);//发送计算结果-补偿值
public slots:
    void _handleMavlinkRawImu(mavlink_raw_imu_t message);
    void _handleMavlinkScaledImu(mavlink_scaled_imu_t message);
    void _handleMavlinkScaledImu2(mavlink_scaled_imu2_t message);
    void _handleMavlinkScaledImu3(mavlink_scaled_imu3_t message);

private:
    void _setSensorTransmissionSpeed(bool fast);
    void _stopCalibration(void);
    void _emitVehicleTextMessage(const QString& message);
    void _queryParam(ParamData& param,char* argName);
    bool _ParamExist(char* argName);


    CalWorkerThread*    _calWorkerThread;
    //float               _rgSavedCompassOffsets[MAX_MAGS][MAG_PARA_NUM];
};

#endif


#include "APMCompassCal.h"
#include "QGC.h"
#include "vehicles/vehiclemanager.h"
#include "loghelper/logger.h"
#include "libeigen/ellipsoidmag.h"
#include "connections/mavlinkmessageinterface.h"


const float CalWorkerThread::mag_sphere_radius = 0.2f;
const unsigned int CalWorkerThread::calibration_sides = 6;
const unsigned int CalWorkerThread::calibration_total_points = 6000;
const unsigned int CalWorkerThread::calibraton_duration_seconds = CalWorkerThread::calibration_sides * 10;
const int _vehicleId=1;

const char* CalWorkerThread::rgCompassParams[MAX_MAGS][MAG_PARA_NUM] = {
    { "CAL_MAG0_B0", "CAL_MAG0_B1", "CAL_MAG0_B2","CAL_MAG0_B3","CAL_MAG0_B4","CAL_MAG0_B5", "CAL_MAG0_B6", "CAL_MAG0_B7", "CAL_MAG0_B8","CAL_MAG0_9P_EN ", "CAL_MAG0_ROT_DEG " },
    { "CAL_MAG1_B0", "CAL_MAG1_B1", "CAL_MAG1_B2","CAL_MAG1_B3","CAL_MAG1_B4","CAL_MAG1_B5", "CAL_MAG1_B6", "CAL_MAG1_B7", "CAL_MAG1_B8","CAL_MAG1_9P_EN ", "CAL_MAG1_ROT_DEG " },
    { "CAL_MAG2_B0", "CAL_MAG2_B1", "CAL_MAG2_B2","CAL_MAG2_B3","CAL_MAG2_B4","CAL_MAG2_B5", "CAL_MAG2_B6", "CAL_MAG2_B7", "CAL_MAG2_B8","CAL_MAG2_9P_EN ", "CAL_MAG2_ROT_DEG " }
};

CalWorkerThread::CalWorkerThread( QObject* parent)
    : QThread(parent)
    , _cancel(false)
{

}

void CalWorkerThread::run(void)
{
    if (calibrate() == calibrate_return_ok) {
        _emitVehicleTextMessage(QStringLiteral("[cal] progress <100>"));
        _emitVehicleTextMessage(QStringLiteral("[cal] calibration done: mag"));
    }

}

void CalWorkerThread::_emitVehicleTextMessage(const QString& message)
{
    //qDebug() << "thread: " << message;
    emit vehicleTextMessage(_vehicleId, 0, MAV_SEVERITY_INFO, message);
}

unsigned CalWorkerThread::progress_percentage(mag_worker_data_t* worker_data)
{
    return 100 * ((float)worker_data->done_count) / calibration_sides;
}

CalWorkerThread::calibrate_return CalWorkerThread::calibrate(void)
{
    calibrate_return result = calibrate_return_ok;

    mag_worker_data_t worker_data;

    worker_data.done_count = 0;
    worker_data.calibration_points_perside = calibration_total_points / calibration_sides;
    worker_data.calibration_interval_perside_seconds = calibraton_duration_seconds / calibration_sides;
    worker_data.calibration_interval_perside_useconds = worker_data.calibration_interval_perside_seconds * 1000 * 1000;

    // Collect data for all sides
    worker_data.side_data_collected[DETECT_ORIENTATION_RIGHTSIDE_UP] =  false;
    worker_data.side_data_collected[DETECT_ORIENTATION_LEFT] =          false;
    worker_data.side_data_collected[DETECT_ORIENTATION_NOSE_DOWN] =     false;
    worker_data.side_data_collected[DETECT_ORIENTATION_TAIL_DOWN] =     false;
    worker_data.side_data_collected[DETECT_ORIENTATION_UPSIDE_DOWN] =   false;
    worker_data.side_data_collected[DETECT_ORIENTATION_RIGHT] =         false;

    for (size_t cur_mag=0; cur_mag<MAX_MAGS; cur_mag++) {
        // Initialize to no memory allocated
        worker_data.x[cur_mag] = NULL;
        worker_data.y[cur_mag] = NULL;
        worker_data.z[cur_mag] = NULL;
        worker_data.calibration_counter_total[cur_mag] = 0;
    }

    const unsigned int calibration_points_maxcount = calibration_sides * worker_data.calibration_points_perside;

    for (size_t cur_mag=0; cur_mag<MAX_MAGS; cur_mag++) {
        if (rgCompassAvailable[cur_mag]) {
            worker_data.x[cur_mag] = reinterpret_cast<float *>(malloc(sizeof(float) * calibration_points_maxcount));
            worker_data.y[cur_mag] = reinterpret_cast<float *>(malloc(sizeof(float) * calibration_points_maxcount));
            worker_data.z[cur_mag] = reinterpret_cast<float *>(malloc(sizeof(float) * calibration_points_maxcount));
            if (worker_data.x[cur_mag] == NULL || worker_data.y[cur_mag] == NULL || worker_data.z[cur_mag] == NULL)
            {
                _emitVehicleTextMessage(QStringLiteral("[cal] ERROR: out of memory"));
                result = calibrate_return_error;
            }
        }
    }

    if (result == calibrate_return_ok) {
        result = calibrate_from_orientation(
                    worker_data.side_data_collected,    // Sides to calibrate
                    &worker_data);                      // Opaque data for calibration worked
    }

    // Calculate calibration values for each mag
    float sphere_b[MAX_MAGS][MAG_PARA_NUM];
    //float meanErr[MAX_MAGS];
    // Sphere fit the data to get calibration values
    qDebug()<<"result do all side=="<<result;
    if (result == calibrate_return_ok)
    {    
        for (unsigned cur_mag=0; cur_mag<MAX_MAGS; cur_mag++)
        {
            if (rgCompassAvailable[cur_mag])
            {
                EllipsoidMag ellipsoid(cur_mag);  //椭球算法
                //椭球法计算磁

                ellipsoid.Mag_Fit(worker_data.x[cur_mag], worker_data.y[cur_mag], worker_data.z[cur_mag],
                                worker_data.calibration_counter_total[cur_mag],sphere_b[cur_mag]);
                sphere_b[cur_mag][MAG_PARA_NUM-2]=1;
                sphere_b[cur_mag][MAG_PARA_NUM-1]=0;
                for(int i=0;i<MAG_PARA_NUM;i++)  //判断结果是否非法
                    if(qIsNaN(sphere_b[cur_mag][i]))
                    {
                        qDebug()<<"cal error "<<cur_mag;
                        _emitVehicleTextMessage(QString("[cal] ERROR: NaN in sphere fit for mag %1").arg(cur_mag));
                        result = calibrate_return_error;
                        break;
                    }
            }
        }
    }
    //emit sendCompensationValue_signal(-sphere_x[0], -sphere_y[0], -sphere_z[0]);
    // Data points are no longer needed
    for (size_t cur_mag=0; cur_mag<MAX_MAGS; cur_mag++)
    {
        if(worker_data.x[cur_mag])
        {
            free(worker_data.x[cur_mag]);
            worker_data.x[cur_mag]=NULL;
        }
        if(worker_data.y[cur_mag])
        {
            free(worker_data.y[cur_mag]);
            worker_data.y[cur_mag]=NULL;
        }
        if(worker_data.z[cur_mag])
        {
            free(worker_data.z[cur_mag]);
            worker_data.z[cur_mag]=NULL;
        }
    }

    qDebug()<<"cal result=="<<result;
    QString temp_str;
    if (result == calibrate_return_ok) {
        for (unsigned cur_mag=1; cur_mag<MAX_MAGS; cur_mag++) {
            if (rgCompassAvailable[cur_mag])
            {
                for(int i=0;i<MAG_PARA_NUM;i++)
                {
                    _ParamSet(rgCompassParams[cur_mag][i],sphere_b[cur_mag][i]);
                    QThread::msleep(100);
                }

                temp_str+=QString("[cal] mag #%1 b0:%2 b1:%3 b2:%4 b3:%5\r\n\b4:%6 b5:%7 b6:%8 b7:%9 b8:%10\r\n").\
                        arg(cur_mag).arg(sphere_b[cur_mag][0]).arg(sphere_b[cur_mag][1]).arg(sphere_b[cur_mag][2])\
                        .arg(sphere_b[cur_mag][3]).arg(sphere_b[cur_mag][4]).arg(sphere_b[cur_mag][5])\
                        .arg(sphere_b[cur_mag][6]).arg(sphere_b[cur_mag][7]).arg(sphere_b[cur_mag][8]);
                qDebug()<<temp_str;
            }          
        }
        _emitVehicleTextMessage(temp_str);
    }
    _ParamSet("DL_MSG_GRP3_MS",1000);
    return result;
}

CalWorkerThread::calibrate_return CalWorkerThread::mag_calibration_worker(detect_orientation_return orientation, void* data)
{
    calibrate_return result = calibrate_return_ok;

    unsigned int calibration_counter_side;

    mag_worker_data_t* worker_data = (mag_worker_data_t*)(data);

    _emitVehicleTextMessage(QStringLiteral("[cal] Rotate vehicle around the detected orientation"));
    _emitVehicleTextMessage(QString("[cal] Continue rotation for %1 seconds").arg(worker_data->calibration_interval_perside_seconds));

    uint64_t calibration_deadline = QGC::groundTimeUsecs() + worker_data->calibration_interval_perside_useconds;

    unsigned int loop_interval_usecs = (worker_data->calibration_interval_perside_seconds * 1000000) / worker_data->calibration_points_perside;

    calibration_counter_side = 0;

    while (QGC::groundTimeUsecs() < calibration_deadline && calibration_counter_side < worker_data->calibration_points_perside) {
        if (_cancel) {
            result = calibrate_return_cancelled;
            break;
        }

        for (size_t cur_mag=0; cur_mag<MAX_MAGS; cur_mag++) {
            if (!rgCompassAvailable[cur_mag]) {
                continue;
            }

            lastScaledImuMutex.lock();
            mavlink_scaled_imu_t copyLastScaledImu = rgLastScaledImu[cur_mag];
            lastScaledImuMutex.unlock();
            //Logger::Instance()->LogInfo(QString("copyLastScaledImu:xacc:%1,yacc:%2,zacc:%3,xgyro:%4,ygyro:%5,ygyro:%6,xmag:%7,ymag:%8,zmag:%9").arg(copyLastScaledImu.xacc).arg(copyLastScaledImu.yacc).arg(copyLastScaledImu.zacc).arg(copyLastScaledImu.xgyro).arg(copyLastScaledImu.ygyro).arg(copyLastScaledImu.zgyro).arg(copyLastScaledImu.xmag).arg(copyLastScaledImu.ymag).arg(copyLastScaledImu.zmag));

            worker_data->x[cur_mag][worker_data->calibration_counter_total[cur_mag]] = copyLastScaledImu.xmag;
            worker_data->y[cur_mag][worker_data->calibration_counter_total[cur_mag]] = copyLastScaledImu.ymag;
            worker_data->z[cur_mag][worker_data->calibration_counter_total[cur_mag]] = copyLastScaledImu.zmag;
            worker_data->calibration_counter_total[cur_mag]++;
        }

        calibration_counter_side++;
        // Progress indicator for side
        _emitVehicleTextMessage(QString("[cal] %1 side calibration: progress <%2>").arg(detect_orientation_str(orientation)).arg(progress_percentage(worker_data) +
        (unsigned)((100 / calibration_sides) * ((float)calibration_counter_side / (float)worker_data->calibration_points_perside))));

        QThread::usleep(loop_interval_usecs);  //单面时间/点数，即每个点的时间
    }

    if (result == calibrate_return_ok) {
        _emitVehicleTextMessage(QString("[cal] %1 side done, rotate to a different side").arg(detect_orientation_str(orientation)));

        worker_data->done_count++;
        _emitVehicleTextMessage(QString("[cal] progress <%1>").arg(progress_percentage(worker_data)));
    }

    return result;
}

CalWorkerThread::calibrate_return CalWorkerThread::calibrate_from_orientation(
        bool	side_data_collected[SIDE_COUNT],
        void*	worker_data)
{
    calibrate_return result = calibrate_return_ok;

    unsigned orientation_failures = 0;

    // Rotate through all requested orientations
    while (true)
    {
        if (_cancel)
        {
            result = calibrate_return_cancelled;
            break;
        }

        unsigned int side_complete_count = 0;

        // Update the number of completed sides
        for (unsigned i = 0; i < SIDE_COUNT; i++) {
            if (side_data_collected[i]) {
                side_complete_count++;
            }
        }

        if (side_complete_count == SIDE_COUNT) {
            // We have completed all sides, move on
            break;
        }

        /* inform user which orientations are still needed */
        char pendingStr[256];
        pendingStr[0] = 0;

        for (unsigned int cur_orientation=0; cur_orientation<SIDE_COUNT; cur_orientation++) {
            if (!side_data_collected[cur_orientation]) {
                strcat(pendingStr, " ");
                strcat(pendingStr, detect_orientation_str((enum detect_orientation_return)cur_orientation));
            }
        }
        _emitVehicleTextMessage(QString("[cal] pending:%1").arg(pendingStr));

        _emitVehicleTextMessage(QStringLiteral("[cal] hold vehicle still on a pending side"));
        enum detect_orientation_return orient = detect_orientation();


        if (orient == DETECT_ORIENTATION_ERROR) {
            orientation_failures++;
            _emitVehicleTextMessage(QStringLiteral("[cal] detected motion, hold still..."));
            continue;
        }
         //Logger::Instance()->LogInfo(QString("*********Oriention is %1***************").arg(detect_orientation_str(orient)));

        /* inform user about already handled side */
        if (side_data_collected[orient]) {
            orientation_failures++;
            _emitVehicleTextMessage(QString("%1 side already completed").arg(detect_orientation_str(orient)));
            _emitVehicleTextMessage(QStringLiteral("rotate to a pending side"));
            continue;
        }

        _emitVehicleTextMessage(QString("[cal] %1 orientation detected").arg(detect_orientation_str(orient)));
        orientation_failures = 0;

        // Call worker routine
        result = mag_calibration_worker(orient, worker_data);
        if (result != calibrate_return_ok ) {
            break;
        }

        _emitVehicleTextMessage(QString("[cal] %1 side done, rotate to a different side").arg(detect_orientation_str(orient)));

        // Note that this side is complete
        side_data_collected[orient] = true;
         QThread::usleep(200000);
    }

    return result;
}

enum CalWorkerThread::detect_orientation_return CalWorkerThread::detect_orientation(void)
{
    bool    stillDetected = false;
    quint64 stillDetectTime;

    int16_t lastX = 0;
    int16_t lastY = 0;
    int16_t lastZ = 0;

    while (true)
    {
        lastScaledImuMutex.lock();
        mavlink_raw_imu_t   copyLastRawImu = lastRawImu;
//        mavlink_raw_imu_t   copyLastRawImu;
//        memccpy(&copyLastRawImu,&lastRawImu,0,sizeof(mavlink_raw_imu_t));
        lastScaledImuMutex.unlock();

        int16_t xDelta = abs(lastX - copyLastRawImu.xacc);
        int16_t yDelta = abs(lastY - copyLastRawImu.yacc);
        int16_t zDelta = abs(lastZ - copyLastRawImu.zacc);
        //Logger::Instance()->LogInfo(QString("accept>>>>xacc:%1 && yacc:%2 && zacc:%3").arg(copyLastRawImu.xacc).arg(copyLastRawImu.yacc).arg(copyLastRawImu.zacc));
        lastX = copyLastRawImu.xacc;
        lastY = copyLastRawImu.yacc;
        lastZ = copyLastRawImu.zacc;

        if (xDelta < 200 && yDelta < 200 && zDelta < 200)
        {
             //Logger::Instance()->LogInfo(QString(">>>>xDelta:%1 && yDelta:%2 && zDelta:%3").arg(xDelta).arg(yDelta).arg(zDelta));
            if (stillDetected)
            {
                if (QGC::groundTimeMilliseconds() - stillDetectTime > 800)
                {
                    break;
                }
            } else
            {
                stillDetectTime = QGC::groundTimeMilliseconds();
                stillDetected = true;
            }
        } else {
            stillDetected = false;
        }

        if (_cancel) {
            break;
        }

        // FIXME: No timeout for never detect still

         QThread::usleep(1000);
    }

    qDebug()<<"check one Last="<<lastX<<"-"<<lastY<<"-"<<lastZ;

    static const uint16_t rawImuOneG = 3000;          //700;
    static const uint16_t rawImuNoGThreshold = 1100;  //900  300

    if (lastX > rawImuOneG && abs(lastY) < rawImuNoGThreshold && abs(lastZ) < rawImuNoGThreshold) {
        return DETECT_ORIENTATION_TAIL_DOWN;        // [ g, 0, 0 ]
    }

    if (lastX < -rawImuOneG && abs(lastY) < rawImuNoGThreshold && abs(lastZ) < rawImuNoGThreshold) {
        return DETECT_ORIENTATION_NOSE_DOWN;        // [ -g, 0, 0 ]
    }

    if (lastY > rawImuOneG && abs(lastX) < rawImuNoGThreshold && abs(lastZ) < rawImuNoGThreshold) {
        return DETECT_ORIENTATION_LEFT;        // [ 0, g, 0 ]
    }

    if (lastY < -rawImuOneG && abs(lastX) < rawImuNoGThreshold && abs(lastZ) < rawImuNoGThreshold) {
        return DETECT_ORIENTATION_RIGHT;        // [ 0, -g, 0 ]
    }

    if (lastZ > rawImuOneG && abs(lastX) < rawImuNoGThreshold && abs(lastY) < rawImuNoGThreshold) {
        return DETECT_ORIENTATION_UPSIDE_DOWN;        // [ 0, 0, g ]
    }

    if (lastZ < -rawImuOneG && abs(lastX) < rawImuNoGThreshold && abs(lastY) < rawImuNoGThreshold) {
        return DETECT_ORIENTATION_RIGHTSIDE_UP;        // [ 0, 0, -g ]
    }

    _emitVehicleTextMessage(QStringLiteral("[cal] ERROR: invalid orientation"));

    return DETECT_ORIENTATION_ERROR;	// Can't detect orientation
}

const char* CalWorkerThread::detect_orientation_str(enum detect_orientation_return orientation)
{
    static const char* rgOrientationStrs[] = {
        "back",		// tail down
        "front",	// nose down
        "left",
        "right",
        "up",		// upside-down
        "down",		// right-side up
        "error"
    };

    return rgOrientationStrs[orientation];
}

void CalWorkerThread::_ParamSet(const char *parId, float val)
{
    ParameterController::Instance()->set_parameter_noanswer((char*)parId,val,MAV_PARAM_TYPE_REAL32);
}
void CalWorkerThread::_ParamSet(const char *parId, int val)
{
    qDebug()<<"INT Parm:"<<parId;
    float fVal;
    memcpy(&fVal,&val,sizeof(float));
    ParameterController::Instance()->set_parameter_noanswer((char*)parId,fVal,MAV_PARAM_TYPE_INT32);
}


int CalWorkerThread::sphere_fit_least_squares(const float x[], const float y[], const float z[],
                                              unsigned int size, unsigned int max_iterations, float delta, float *sphere_x, float *sphere_y, float *sphere_z,
                                              float *sphere_radius)
{

    float x_sumplain = 0.0f;
    float x_sumsq = 0.0f;
    float x_sumcube = 0.0f;

    float y_sumplain = 0.0f;
    float y_sumsq = 0.0f;
    float y_sumcube = 0.0f;

    float z_sumplain = 0.0f;
    float z_sumsq = 0.0f;
    float z_sumcube = 0.0f;

    float xy_sum = 0.0f;
    float xz_sum = 0.0f;
    float yz_sum = 0.0f;

    float x2y_sum = 0.0f;
    float x2z_sum = 0.0f;
    float y2x_sum = 0.0f;
    float y2z_sum = 0.0f;
    float z2x_sum = 0.0f;
    float z2y_sum = 0.0f;

    for (unsigned int i = 0; i < size; i++) {

        float x2 = x[i] * x[i];
        float y2 = y[i] * y[i];
        float z2 = z[i] * z[i];

        x_sumplain += x[i];
        x_sumsq += x2;
        x_sumcube += x2 * x[i];

        y_sumplain += y[i];
        y_sumsq += y2;
        y_sumcube += y2 * y[i];

        z_sumplain += z[i];
        z_sumsq += z2;
        z_sumcube += z2 * z[i];

        xy_sum += x[i] * y[i];
        xz_sum += x[i] * z[i];
        yz_sum += y[i] * z[i];

        x2y_sum += x2 * y[i];
        x2z_sum += x2 * z[i];

        y2x_sum += y2 * x[i];
        y2z_sum += y2 * z[i];

        z2x_sum += z2 * x[i];
        z2y_sum += z2 * y[i];
    }

    //
    //Least Squares Fit a sphere A,B,C with radius squared Rsq to 3D data
    //
    //    P is a structure that has been computed with the data earlier.
    //    P.npoints is the number of elements; the length of X,Y,Z are identical.
    //    P's members are logically named.
    //
    //    X[n] is the x component of point n
    //    Y[n] is the y component of point n
    //    Z[n] is the z component of point n
    //
    //    A is the x coordiante of the sphere
    //    B is the y coordiante of the sphere
    //    C is the z coordiante of the sphere
    //    Rsq is the radius squared of the sphere.
    //
    //This method should converge; maybe 5-100 iterations or more.
    //
    float x_sum = x_sumplain / size;        //sum( X[n] )
    float x_sum2 = x_sumsq / size;    //sum( X[n]^2 )
    float x_sum3 = x_sumcube / size;    //sum( X[n]^3 )
    float y_sum = y_sumplain / size;        //sum( Y[n] )
    float y_sum2 = y_sumsq / size;    //sum( Y[n]^2 )
    float y_sum3 = y_sumcube / size;    //sum( Y[n]^3 )
    float z_sum = z_sumplain / size;        //sum( Z[n] )
    float z_sum2 = z_sumsq / size;    //sum( Z[n]^2 )
    float z_sum3 = z_sumcube / size;    //sum( Z[n]^3 )

    float XY = xy_sum / size;        //sum( X[n] * Y[n] )
    float XZ = xz_sum / size;        //sum( X[n] * Z[n] )
    float YZ = yz_sum / size;        //sum( Y[n] * Z[n] )
    float X2Y = x2y_sum / size;    //sum( X[n]^2 * Y[n] )
    float X2Z = x2z_sum / size;    //sum( X[n]^2 * Z[n] )
    float Y2X = y2x_sum / size;    //sum( Y[n]^2 * X[n] )
    float Y2Z = y2z_sum / size;    //sum( Y[n]^2 * Z[n] )
    float Z2X = z2x_sum / size;    //sum( Z[n]^2 * X[n] )
    float Z2Y = z2y_sum / size;    //sum( Z[n]^2 * Y[n] )

    //Reduction of multiplications
    float F0 = x_sum2 + y_sum2 + z_sum2;
    float F1 =  0.5f * F0;
    float F2 = -8.0f * (x_sum3 + Y2X + Z2X);
    float F3 = -8.0f * (X2Y + y_sum3 + Z2Y);
    float F4 = -8.0f * (X2Z + Y2Z + z_sum3);

    //Set initial conditions:
    float A = x_sum;
    float B = y_sum;
    float C = z_sum;

    //First iteration computation:
    float A2 = A * A;
    float B2 = B * B;
    float C2 = C * C;
    float QS = A2 + B2 + C2;
    float QB = -2.0f * (A * x_sum + B * y_sum + C * z_sum);

    //Set initial conditions:
    float Rsq = F0 + QB + QS;

    //First iteration computation:
    float Q0 = 0.5f * (QS - Rsq);
    float Q1 = F1 + Q0;
    float Q2 = 8.0f * (QS - Rsq + QB + F0);
    float aA, aB, aC, nA, nB, nC, dA, dB, dC;

    //Iterate N times, ignore stop condition.
    unsigned int n = 0;

#define FLT_EPSILON 1.1920929e-07F  /* 1E-5 */

    while (n < max_iterations) {
        n++;

        //Compute denominator:
        aA = Q2 + 16.0f * (A2 - 2.0f * A * x_sum + x_sum2);
        aB = Q2 + 16.0f * (B2 - 2.0f * B * y_sum + y_sum2);
        aC = Q2 + 16.0f * (C2 - 2.0f * C * z_sum + z_sum2);
        aA = (fabsf(aA) < FLT_EPSILON) ? 1.0f : aA;
        aB = (fabsf(aB) < FLT_EPSILON) ? 1.0f : aB;
        aC = (fabsf(aC) < FLT_EPSILON) ? 1.0f : aC;

        //Compute next iteration
        nA = A - ((F2 + 16.0f * (B * XY + C * XZ + x_sum * (-A2 - Q0) + A * (x_sum2 + Q1 - C * z_sum - B * y_sum))) / aA);
        nB = B - ((F3 + 16.0f * (A * XY + C * YZ + y_sum * (-B2 - Q0) + B * (y_sum2 + Q1 - A * x_sum - C * z_sum))) / aB);
        nC = C - ((F4 + 16.0f * (A * XZ + B * YZ + z_sum * (-C2 - Q0) + C * (z_sum2 + Q1 - A * x_sum - B * y_sum))) / aC);

        //Check for stop condition
        dA = (nA - A);
        dB = (nB - B);
        dC = (nC - C);

        if ((dA * dA + dB * dB + dC * dC) <= delta) { break; }

        //Compute next iteration's values
        A = nA;
        B = nB;
        C = nC;
        A2 = A * A;
        B2 = B * B;
        C2 = C * C;
        QS = A2 + B2 + C2;
        QB = -2.0f * (A * x_sum + B * y_sum + C * z_sum);
        Rsq = F0 + QB + QS;
        Q0 = 0.5f * (QS - Rsq);
        Q1 = F1 + Q0;
        Q2 = 8.0f * (QS - Rsq + QB + F0);
    }

    *sphere_x = A;
    *sphere_y = B;
    *sphere_z = C;
    *sphere_radius = sqrtf(Rsq);

    return 0;
}

APMCompassCal::APMCompassCal(void):
    _calWorkerThread(NULL)
{

}

APMCompassCal::~APMCompassCal()
{
    if (_calWorkerThread) {
          _calWorkerThread->terminate();
          // deleteLater so it happens on correct thread
          _calWorkerThread->deleteLater();
      }
}

void APMCompassCal::startCalibration(bool bMag1, bool bMag2, bool bMag0)
{
    _setSensorTransmissionSpeed(true /* fast */);
    connect(&FrmMainController::Instance()->__vehicle->mavLinkMessageInterface,SIGNAL(calibrate_raw_imu_recved(mavlink_raw_imu_t)),this, SLOT(_handleMavlinkRawImu(mavlink_raw_imu_t)));
    connect(&FrmMainController::Instance()->__vehicle->mavLinkMessageInterface,SIGNAL(calibrate_scale_imu_recved(mavlink_scaled_imu_t)),this, SLOT(_handleMavlinkScaledImu(mavlink_scaled_imu_t)));
    connect(&FrmMainController::Instance()->__vehicle->mavLinkMessageInterface,SIGNAL(calibrate_scale_imu2_recved(mavlink_scaled_imu2_t)),this, SLOT(_handleMavlinkScaledImu2(mavlink_scaled_imu2_t)));
    connect(&FrmMainController::Instance()->__vehicle->mavLinkMessageInterface,SIGNAL(calibrate_scale_imu3_recved(mavlink_scaled_imu3_t)),this, SLOT(_handleMavlinkScaledImu3(mavlink_scaled_imu3_t)));

    // Simulate a start message
    _emitVehicleTextMessage("[cal] calibration started: mag");

    qDebug()<<"Mag0="<<bMag0<<"Mag1="<<bMag1<<"Mag2="<<bMag2;
    _calWorkerThread = new CalWorkerThread(this);
    _calWorkerThread->rgCompassAvailable[0]=bMag0;
    _calWorkerThread->rgCompassAvailable[1]=bMag1;
    _calWorkerThread->rgCompassAvailable[2]=bMag2;
    connect(_calWorkerThread, SIGNAL(vehicleTextMessage(int,int,int,QString)), this, SIGNAL(vehicleTextMessage(int,int,int,QString)));
    connect(_calWorkerThread, SIGNAL(sendCompensationValue_signal(float,float,float)), this, SIGNAL(sendCompensationValue_signal(float,float,float)));
    // Clear the offset parameters so we get raw data
    float calVal[MAG_PARA_NUM]={1,0,0,1,0,1,0,0,0,1,0};  //开始校准是各个参数的默认值
    for (int i=0; i<MAX_MAGS; i++)
    {
        for (int j=0; j<MAG_PARA_NUM; j++)
        {
            if(_calWorkerThread->rgCompassAvailable[i])
            {
                _calWorkerThread->_ParamSet(CalWorkerThread::rgCompassParams[i][j],calVal[j]);
                QThread::msleep(200);
            }
        }
    }

    _calWorkerThread->_ParamSet("DL_MSG_GRP3_MS",100);
    _calWorkerThread->start();
}

void APMCompassCal::cancelCalibration(void)
{
    _calWorkerThread->_ParamSet("DL_MSG_GRP3_MS",1000);
    _stopCalibration();  

    // Simulate a cancelled message
    _emitVehicleTextMessage("[cal] calibration cancelled");
}

void APMCompassCal::_handleMavlinkRawImu(mavlink_raw_imu_t message)
{
    _calWorkerThread->lastScaledImuMutex.lock();
//      mavlink_msg_raw_imu_decode(&message, &_calWorkerThread->lastRawImu);
//      memcpy(&_calWorkerThread->lastRawImu,&message,sizeof(mavlink_raw_imu_t));
        ((mavlink_raw_imu_t*)(&_calWorkerThread->lastRawImu))->time_usec=message.time_usec;
        ((mavlink_raw_imu_t*)(&_calWorkerThread->lastRawImu))->xacc=message.xacc;
        ((mavlink_raw_imu_t*)(&_calWorkerThread->lastRawImu))->yacc=message.yacc;
        ((mavlink_raw_imu_t*)(&_calWorkerThread->lastRawImu))->zacc=message.zacc;
        ((mavlink_raw_imu_t*)(&_calWorkerThread->lastRawImu))->xgyro=message.xgyro;
        ((mavlink_raw_imu_t*)(&_calWorkerThread->lastRawImu))->ygyro=message.ygyro;
        ((mavlink_raw_imu_t*)(&_calWorkerThread->lastRawImu))->zgyro=message.zgyro;
        ((mavlink_raw_imu_t*)(&_calWorkerThread->lastRawImu))->xmag=message.xmag;
        ((mavlink_raw_imu_t*)(&_calWorkerThread->lastRawImu))->ymag=message.ymag;
        ((mavlink_raw_imu_t*)(&_calWorkerThread->lastRawImu))->zmag=message.zmag;
//     _calWorkerThread->lastRawImu=message;

//    _calWorkerThread->rgLastScaledImu[0].xacc = _calWorkerThread->lastRawImu.xacc;
//    _calWorkerThread->rgLastScaledImu[0].yacc = _calWorkerThread->lastRawImu.yacc;
//    _calWorkerThread->rgLastScaledImu[0].zacc = _calWorkerThread->lastRawImu.zacc;
//    _calWorkerThread->rgLastScaledImu[0].xgyro = _calWorkerThread->lastRawImu.xgyro;
//    _calWorkerThread->rgLastScaledImu[0].ygyro = _calWorkerThread->lastRawImu.ygyro;
//    _calWorkerThread->rgLastScaledImu[0].zgyro = _calWorkerThread->lastRawImu.zgyro;
//    _calWorkerThread->rgLastScaledImu[0].xmag = _calWorkerThread->lastRawImu.xmag;
//    _calWorkerThread->rgLastScaledImu[0].ymag = _calWorkerThread->lastRawImu.ymag;
//    _calWorkerThread->rgLastScaledImu[0].zmag = _calWorkerThread->lastRawImu.zmag;
    _calWorkerThread->lastScaledImuMutex.unlock();
     //Logger::Instance()->LogInfo(QString("Raw IMU recved:-->xacc:%1-->yacc:%2-->zacc:%3").arg(_calWorkerThread->lastRawImu.xacc).arg(_calWorkerThread->lastRawImu.yacc).arg(_calWorkerThread->lastRawImu.zacc));
     //Logger::Instance()->LogInfo(QString("recv:rgLastScaledImu[0]:xacc:%1,yacc:%2,zacc:%3,xgyro:%4,ygyro:%5,ygyro:%6,xmag:%7,ymag:%8,zmag:%9").arg(message.xacc).arg(message.yacc).arg(message.zacc).arg(message.xgyro).arg(message.ygyro).arg(message.zgyro).arg(message.xmag).arg(message.ymag).arg(message.zmag));
}

void APMCompassCal::_handleMavlinkScaledImu(mavlink_scaled_imu_t message)
{
    //qDebug() << "apm: scale re";
    _calWorkerThread->lastScaledImuMutex.lock();
    _calWorkerThread->rgLastScaledImu[0].xacc = message.xacc;
    _calWorkerThread->rgLastScaledImu[0].yacc = message.yacc;
    _calWorkerThread->rgLastScaledImu[0].zacc = message.zacc;
    _calWorkerThread->rgLastScaledImu[0].xgyro = message.xgyro;
    _calWorkerThread->rgLastScaledImu[0].ygyro = message.ygyro;
    _calWorkerThread->rgLastScaledImu[0].zgyro = message.zgyro;
    _calWorkerThread->rgLastScaledImu[0].xmag = message.xmag;
    _calWorkerThread->rgLastScaledImu[0].ymag = message.ymag;
    _calWorkerThread->rgLastScaledImu[0].zmag = message.zmag;

    _calWorkerThread->lastScaledImuMutex.unlock();
    //Logger::Instance()->LogInfo(QString("recv:rgLastScaledImu[1]:xacc:%1,yacc:%2,zacc:%3,xgyro:%4,ygyro:%5,ygyro:%6,xmag:%7,ymag:%8,zmag:%9").arg(message.xacc).arg(message.yacc).arg(message.zacc).arg(message.xgyro).arg(message.ygyro).arg(message.zgyro).arg(message.xmag).arg(message.ymag).arg(message.zmag));
    //Logger::Instance()->LogInfo(QString("recv:-->%1-->%2").arg(QDateTime::currentDateTime().toString("HHmmss")).arg("message Scaled2 IMU;"));
}

void APMCompassCal::_handleMavlinkScaledImu2(mavlink_scaled_imu2_t message)
{
    _calWorkerThread->lastScaledImuMutex.lock();
    ((mavlink_scaled_imu2_t*)&_calWorkerThread->rgLastScaledImu[1])->time_boot_ms=message.time_boot_ms;
    ((mavlink_scaled_imu2_t*)&_calWorkerThread->rgLastScaledImu[1])->xacc=message.xacc;
    ((mavlink_scaled_imu2_t*)&_calWorkerThread->rgLastScaledImu[1])->xgyro=message.xgyro;
    ((mavlink_scaled_imu2_t*)&_calWorkerThread->rgLastScaledImu[1])->xmag=message.xmag;
    ((mavlink_scaled_imu2_t*)&_calWorkerThread->rgLastScaledImu[1])->yacc=message.yacc;
    ((mavlink_scaled_imu2_t*)&_calWorkerThread->rgLastScaledImu[1])->ygyro=message.ygyro;
    ((mavlink_scaled_imu2_t*)&_calWorkerThread->rgLastScaledImu[1])->ymag=message.ymag;
    ((mavlink_scaled_imu2_t*)&_calWorkerThread->rgLastScaledImu[1])->zacc=message.zacc;
    ((mavlink_scaled_imu2_t*)&_calWorkerThread->rgLastScaledImu[1])->zgyro=message.zgyro;
    ((mavlink_scaled_imu2_t*)&_calWorkerThread->rgLastScaledImu[1])->zmag=message.zmag;

    _calWorkerThread->lastScaledImuMutex.unlock();
    //Logger::Instance()->LogInfo(QString("recv:rgLastScaledImu[1]:xacc:%1,yacc:%2,zacc:%3,xgyro:%4,ygyro:%5,ygyro:%6,xmag:%7,ymag:%8,zmag:%9").arg(message.xacc).arg(message.yacc).arg(message.zacc).arg(message.xgyro).arg(message.ygyro).arg(message.zgyro).arg(message.xmag).arg(message.ymag).arg(message.zmag));
    //Logger::Instance()->LogInfo(QString("recv:-->%1-->%2").arg(QDateTime::currentDateTime().toString("HHmmss")).arg("message Scaled2 IMU;"));
}

void APMCompassCal::_handleMavlinkScaledImu3(mavlink_scaled_imu3_t message)
{
    _calWorkerThread->lastScaledImuMutex.lock();
    ((mavlink_scaled_imu3_t*)&_calWorkerThread->rgLastScaledImu[2])->time_boot_ms=message.time_boot_ms;
    ((mavlink_scaled_imu3_t*)&_calWorkerThread->rgLastScaledImu[2])->xacc=message.xacc;
    ((mavlink_scaled_imu3_t*)&_calWorkerThread->rgLastScaledImu[2])->xgyro=message.xgyro;
    ((mavlink_scaled_imu3_t*)&_calWorkerThread->rgLastScaledImu[2])->xmag=message.xmag;
    ((mavlink_scaled_imu3_t*)&_calWorkerThread->rgLastScaledImu[2])->yacc=message.yacc;
    ((mavlink_scaled_imu3_t*)&_calWorkerThread->rgLastScaledImu[2])->ygyro=message.ygyro;
    ((mavlink_scaled_imu3_t*)&_calWorkerThread->rgLastScaledImu[2])->ymag=message.ymag;
    ((mavlink_scaled_imu3_t*)&_calWorkerThread->rgLastScaledImu[2])->zacc=message.zacc;
    ((mavlink_scaled_imu3_t*)&_calWorkerThread->rgLastScaledImu[2])->zgyro=message.zgyro;
    ((mavlink_scaled_imu3_t*)&_calWorkerThread->rgLastScaledImu[2])->zmag=message.zmag;

    _calWorkerThread->lastScaledImuMutex.unlock();
    //Logger::Instance()->LogInfo(QString("recv:-->%1-->%2").arg(QDateTime::currentDateTime().toString("HHmmss")).arg("message Scaled3 IMU;"));
}

void APMCompassCal::_setSensorTransmissionSpeed(bool fast)
{
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.requestDataStreamRawSensors(1,fast ? 10 : 2);
}

void APMCompassCal::_stopCalibration(void)
{
    _calWorkerThread->cancel();
    disconnect(&FrmMainController::Instance()->__vehicle->mavLinkMessageInterface,SIGNAL(calibrate_raw_imu_recved(mavlink_raw_imu_t)),this, SLOT(_handleMavlinkRawImu(mavlink_raw_imu_t)));
    disconnect(&FrmMainController::Instance()->__vehicle->mavLinkMessageInterface,SIGNAL(calibrate_scale_imu2_recved(mavlink_scaled_imu2_t)),this, SLOT(_handleMavlinkScaledImu2(mavlink_scaled_imu2_t)));
    disconnect(&FrmMainController::Instance()->__vehicle->mavLinkMessageInterface,SIGNAL(calibrate_scale_imu3_recved(mavlink_scaled_imu3_t)),this, SLOT(_handleMavlinkScaledImu3(mavlink_scaled_imu3_t)));
    _setSensorTransmissionSpeed(false /* fast */);
}

void APMCompassCal::_emitVehicleTextMessage(const QString& message)
{
    //qDebug() << "cal: " << message;
    emit vehicleTextMessage(_vehicleId, 0, MAV_SEVERITY_INFO, message);
}
void APMCompassCal::_queryParam(ParamData& param,char* argName)
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
void CalWorkerThread::_queryParam(ParamData& param,char* argName)
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
bool APMCompassCal::_ParamExist(char* argName)
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

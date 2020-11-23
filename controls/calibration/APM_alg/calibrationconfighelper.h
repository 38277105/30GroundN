#ifndef CALIBRATIONCONFIGHELPER_H
#define CALIBRATIONCONFIGHELPER_H

#include <QObject>
#include <QTableWidget>
#include "controls/calibration/AccelCalibrationConfig.h"
//#include "controls/calibration/CompassConfig.h"
#include "controls/calibration/RadioCalibrationConfig.h"

class CalibrationConfigHelper : public QObject
{
    Q_OBJECT
public:
    explicit CalibrationConfigHelper(QObject *parent = 0);

    void setupUi();

    QTabWidget* getWidget();

signals:

public slots:

private:
    QTabWidget *tabWidget;
    QWidget *tab_accelebrateCalibration;
    QWidget *tab_magnify;
    QWidget *tab_rc;

    AccelCalibrationConfig* m_accelCalibrationConfig;
//    CompassConfig* m_compassConfig;
    RadioCalibrationConfig* m_radioCalibrationConfig;
};

#endif // CALIBRATIONCONFIGHELPER_H

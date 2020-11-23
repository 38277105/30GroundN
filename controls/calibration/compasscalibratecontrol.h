#ifndef COMPASSCALIBRATECONTROL_H
#define COMPASSCALIBRATECONTROL_H

#include <QWidget>
#include "APMCompassCal.h"

namespace Ui {
class CompassCalibrateControl;
}

class CompassCalibrateControl : public QWidget
{
    Q_OBJECT

public:
    explicit CompassCalibrateControl(QWidget *parent = 0);
    ~CompassCalibrateControl();

    APMCompassCal m_compassCal;
signals:
    void __setdown();
    void __setUp();
    void __setLeft();
    void __setRight();
    void __setNoseDown();
    void __setTrailDown();


public slots:
    void initial_lblImage();
    void setdown();
    void setUp();
    void setLeft();
    void setRight();
    void setNoseDown();
    void setTrailDown();
private slots:
    void on_btn_calibration_clicked();
    void vehicleTextMessage(int vehicleId, int compId, int severity, QString text);

    void on_btn_Cancelcalibrate_clicked();
    void upDataCompensationValue(float x, float y, float z);
private:
    Ui::CompassCalibrateControl *ui;
    bool _magCalInProgress;

    bool _orientationCalDownSideDone;
    bool _orientationCalUpsideDownSideDone;
    bool _orientationCalLeftSideDone;
    bool _orientationCalRightSideDone;
    bool _orientationCalNoseDownSideDone;
    bool _orientationCalTailDownSideDone;

    bool _orientationCalDownSideInProgress;
    bool _orientationCalUpsideDownSideInProgress;
    bool _orientationCalLeftSideInProgress;
    bool _orientationCalRightSideInProgress;
    bool _orientationCalNoseDownSideInProgress;
    bool _orientationCalTailDownSideInProgress;

    bool _orientationCalDownSideRotate;
    bool _orientationCalUpsideDownSideRotate;
    bool _orientationCalLeftSideRotate;
    bool _orientationCalRightSideRotate;
    bool _orientationCalNoseDownSideRotate;
    bool _orientationCalTailDownSideRotate;

    QString m_offsetContent;

public:
    bool compassSetupNeeded(void) const;
    bool accelSetupNeeded(void) const;
};

#endif // COMPASSCALIBRATECONTROL_H

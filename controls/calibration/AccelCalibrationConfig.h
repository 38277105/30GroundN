#ifndef ACCELCALIBRATIONCONFIG_H
#define ACCELCALIBRATIONCONFIG_H

#include <QWidget>
#include <QTimer>
#include "ui_AccelCalibrationConfig.h"

class AccelCalibrationConfig : public QWidget
{
    Q_OBJECT
    
    static const int CALIBRATION_TIMEOUT_SEC = 40;
public:
    explicit AccelCalibrationConfig(QWidget *parent = 0);
    ~AccelCalibrationConfig();
protected:
    void hideEvent(QHideEvent *evt);
private slots:
    void makeConnection();
    void calibrateButtonClicked();
    void uasTextMessageReceived(int uasid, int componentid, int severity, QString text);
    void uasConnected();
    void uasDisconnected();
    void countdownTimerTick();

    void executeCommandAck(int num, bool success);

private:
    int m_accelAckCount;
    Ui::AccelCalibrationConfig ui;
    bool m_muted;
    bool m_isCalibrating;
    QTimer m_countdownTimer;
    int m_countdownCount;
};

#endif // ACCELCALIBRATIONCONFIG_H

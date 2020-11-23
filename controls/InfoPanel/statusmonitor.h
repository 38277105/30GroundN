#ifndef STATUSMONITOR_H
#define STATUSMONITOR_H

#include <QWidget>
#include <vehicles/vehiclestate.h>
#include <QLabel>
#include "poweralarm.h"
namespace Ui {
class StatusMonitor;
}

class StatusMonitor : public QWidget
{
    Q_OBJECT

public:
    explicit StatusMonitor(QWidget *parent = 0);
    ~StatusMonitor();

    void setStatus(VehicleState _state);

    void setFlightMode(QString strText);
    void setScale(float scale);
    void reshapeLabel();
private:
    void setMonitor();
    void tipMessageBox(const QString& title, const QString& content);
public slots:
    void TimeAddSlot();
private slots:
    void on_btn_set_bakVolt_coefficient_clicked();

private:
    Ui::StatusMonitor *ui;
    PowerAlarm   m_PA_mainVolt;
    PowerAlarm   m_PA_bakVolt;
    PowerAlarm   m_PA_mainCurrent;  //主电电流没有了，用油门代替
    PowerAlarm   m_PA_Temperature;
    PowerAlarm   m_PA_ESC_A;
    PowerAlarm   m_PA_ESC_B;
    PowerAlarm   m_PA_ESC_C;
    PowerAlarm   m_PA_ESC_D;
    PowerAlarm   m_PA_ESC_E;
    PowerAlarm   m_PA_ESC_F;
    PowerAlarm   m_PA_ESC_G;
    PowerAlarm   m_PA_ESC_H;
    PowerAlarm   m_PA_ESC_I;
    PowerAlarm   m_PA_ESC_J;
    PowerAlarm   m_PA_ESC_K;
    PowerAlarm   m_PA_ESC_L;
    //旋翼
    PowerAlarm   m_PA_RotorRemainTime;
    //电机
    PowerAlarm   m_PA_MotorRemainTime;
    //电调
    PowerAlarm   m_PA_EleDebugRemainTime;
    //电池
    PowerAlarm   m_PA_BatteryRemainTime;
    QTimer       m_timeAdd;
    int          m_tmCount;
    int         m_bakVolt_coefficient;
};

#endif // STATUSMONITOR_H

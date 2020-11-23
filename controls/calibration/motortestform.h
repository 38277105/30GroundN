#ifndef MOTORTESTFORM_H
#define MOTORTESTFORM_H

#include <QWidget>
#include "stdint.h"
#include <QPushButton>

namespace Ui {
class MotorTestForm;
}

class MotorTestForm : public QWidget
{
    Q_OBJECT

public:
    explicit MotorTestForm(QWidget *parent = 0);
    ~MotorTestForm();

    void CreateButtonByType(uint8_t uavType);
    void SendCommandByOrder(int motorIndex,float throttle,float timeout);

protected:
    void showEvent(QShowEvent* event);

    void hideEvent(QHideEvent* event);
private:
    Ui::MotorTestForm *ui;
      QPushButton* _motorTestButton;
      bool IsCreated;
      int  MaxMotor;
      uint8_t uavType;
public slots:
    void MotorTestClick();
};

#endif // MOTORTESTFORM_H

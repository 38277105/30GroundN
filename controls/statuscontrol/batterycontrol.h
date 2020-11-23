#ifndef BATTERYCONTROL_H
#define BATTERYCONTROL_H

#include <QWidget>

namespace Ui {
class BatteryControl;
}

class BatteryControl : public QWidget
{
    Q_OBJECT

public:
    explicit BatteryControl(QWidget *parent = 0);
    ~BatteryControl();

    void setValue(int left);

private:
    Ui::BatteryControl *ui;
    QList<QString> m_imageList;
};

#endif // BATTERYCONTROL_H

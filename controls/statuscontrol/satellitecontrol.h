#ifndef SATELLITECONTROL_H
#define SATELLITECONTROL_H

#include <QWidget>

namespace Ui {
class SatelliteControl;
}

class SatelliteControl : public QWidget
{
    Q_OBJECT

public:
    explicit SatelliteControl(QWidget *parent = 0);
    ~SatelliteControl();
    void setSatelliteNum(int num);
    void setSatelliteState(int fix_type);

private:
    Ui::SatelliteControl *ui;
};

#endif // SATELLITECONTROL_H

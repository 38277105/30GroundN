#ifndef CALIBRATEFORM_H
#define CALIBRATEFORM_H

#include <QWidget>

namespace Ui {
class CalibrateForm;
}

class AccelCalibrationConfig;
//class CompassConfig;
class CompassCalibrateControl;
class RadioCalibrationConfig;
class AccelCalibrationOfIndependent;
class MotorTestForm;
class ESCCaliForm;
class CalibrateForm : public QWidget
{
    Q_OBJECT

public:
    explicit CalibrateForm(QWidget *parent = 0);
    ~CalibrateForm();
    void initialContent();

private:
    Ui::CalibrateForm *ui;

    AccelCalibrationConfig* m_accelCalibrationConfig;
//    CompassConfig* m_compassConfig;
    CompassCalibrateControl* m_compassControl;

    RadioCalibrationConfig* m_radioCalibrationConfig;
    AccelCalibrationOfIndependent* m_accelCalibrationOfIndependent;
    MotorTestForm* m_motorTestForm;
    ESCCaliForm *m_esc_cali_from;
};

#endif // CALIBRATEFORM_H

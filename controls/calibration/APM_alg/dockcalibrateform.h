#ifndef DOCKCALIBRATEFORM_H
#define DOCKCALIBRATEFORM_H

#include <QWidget>

namespace Ui {
class DockCalibrateForm;
}

class AccelCalibrationConfig;
//class CompassConfig;
class RadioCalibrationConfig;

class DockCalibrateForm : public QWidget
{
    Q_OBJECT

public:
    explicit DockCalibrateForm(QWidget *parent = 0);
    ~DockCalibrateForm();

private slots:
    void on_compassButton_clicked();

    void on_accelCalibrateButton_clicked();

    void on_radioCalibrateButton_clicked();

private:
    Ui::DockCalibrateForm *ui;

    AccelCalibrationConfig* m_accelCalibrationConfig;
    //CompassConfig* m_compassConfig;
    RadioCalibrationConfig* m_radioCalibrationConfig;
};

#endif // DOCKCALIBRATEFORM_H

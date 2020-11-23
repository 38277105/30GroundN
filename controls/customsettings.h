#ifndef CUSTOMSETTINGS_H
#define CUSTOMSETTINGS_H
//添加设置项列表，包含设置值变更标识。提交时候检查标志，如果标志变了会提交到服务器。
#include <QWidget>
#include <settingcontrols/settingmanager.h>
#include <QDoubleSpinBox>
#include <QSlider>

namespace Ui {
class CustomSettings;
}

class CustomSettings : public QWidget
{
    Q_OBJECT

public:
    explicit CustomSettings(QWidget *parent = 0);
    ~CustomSettings();
public slots:
    void valueComing();
    void valueSendToPlane();
    void _queryParam(ParamData& param,char* argName);

private slots:
    void on_flightmode1_editingFinished();

    void on_flightmode2_editingFinished();

    void on_flightmode3_editingFinished();

    void on_flightmode4_editingFinished();

    void on_flightmode5_editingFinished();

    void on_flightmode6_editingFinished();

    void on_slider_n_hspeed_valueChanged(int value);

    void on_slider_n_distance_valueChanged(int value);

    void on_slider_n_climb_valueChanged(int value);

    void on_slider_n_downspeed_valueChanged(int value);

    void on_slider_n_hoverspeed_valueChanged(int value);

    void on_slider_r_returnheight_valueChanged(int value);

    void on_slider_r_lasthoverheight_valueChanged(int value);

    void on_slider_r_lasthovertime_valueChanged(int value);

    void on_slider_r_lastdownspeed_valueChanged(int value);

    void on_slider_handler_maxspeed_valueChanged(int value);

    void on_slider_handler_sense_editingFinished();

    void on_slider_handler_safebutton_editingFinished();

    void on_slider_stable_pitch_angle_valueChanged(int value);

    void on_slider_stable_roll_angle_valueChanged(int value);

    void on_slider_stable_yaw_angle_valueChanged(int value);

    void on_slider_stable_stablectrl_valueChanged(int value);


    void on_slider_pitch_speedctrl_P_valueChanged(int value);

    void on_slider_pitch_speedctrl_I_valueChanged(int value);

    void on_slider_pitch_speedctrl_D_valueChanged(int value);

    void on_slider_roll_speedctrl_P_valueChanged(int value);

    void on_slider_roll_speedctrl_I_valueChanged(int value);

    void on_slider_roll_speedctrl_D_valueChanged(int value);

    void on_slider_yaw_speedctrl_P_valueChanged(int value);

    void on_slider_yaw_speedctrl_I_valueChanged(int value);

    void on_slider_yaw_speedctrl_D_valueChanged(int value);

    void on_slider_stable_speedctrl_P_valueChanged(int value);

    void on_slider_stable_speedctrl_I_valueChanged(int value);

    void on_slider_stable_speedctrl_D_valueChanged(int value);

    void on_throttle_controlacc_P_valueChanged(int value);

    void on_throttle_controlacc_I_valueChanged(int value);

    void on_throttle_controlacc_D_valueChanged(int value);

    void on_throttle_controlspeed_P_valueChanged(int value);

    void on_btn_Save_clicked();

    void on_btn_Loadargs_clicked();

private:
    Ui::CustomSettings *ui;
    QMap<QString,ParamData> m_params;
    void shareMethod_DictFillor(QString key,QDoubleSpinBox* boxcontrol);
    void shareMethod_DictFillor_slider(QString key,QSlider* boxcontrol);
    void setcontrol(QString key,QDoubleSpinBox* boxcontrol);
    void setcontrol_slider(QString key,QSlider* boxcontrol);
};

#endif // CUSTOMSETTINGS_H

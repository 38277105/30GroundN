#ifndef CMDWINDOW_H
#define CMDWINDOW_H

#include <QWidget>

#include "vehicles/vehiclemanager.h"
#include "connections/linkmanager.h"
#include "vehicles/vehicle.h"
#include "connections/mavlinkmessageinterface.h"
namespace Ui {
class CmdWindow;
}

 enum MAV_MODE_MY_FLAG
{
           ///<summary> 0b00000001 Reserved for future use. | </summary>
       CUSTOM_MODE_ENABLED=1,
       ///<summary> 0b00000010 system has a test mode enabled. This flag is intended for temporary system tests and should not be used for stable implementations. | </summary>
       TEST_ENABLED=2,
       ///<summary> 0b00000100 autonomous mode enabled, system finds its own goal positions. Guided flag can be set or not, depends on the actual implementation. | </summary>
       AUTO_ENABLED=4,
       ///<summary> 0b00001000 guided mode enabled, system flies MISSIONs / mission items. | </summary>
       GUIDED_ENABLED=8,
       ///<summary> 0b00010000 system stabilizes electronically its attitude (and optionally position). It needs however further control inputs to move around. | </summary>
       STABILIZE_ENABLED=16,
       ///<summary> 0b00100000 hardware in the loop simulation. All motors / actuators are blocked, but internal software is full operational. | </summary>
       HIL_ENABLED=32,
       ///<summary> 0b01000000 remote control input is enabled. | </summary>
       MANUAL_INPUT_ENABLED=64,
       ///<summary> 0b10000000 MAV safety set to armed. Motors are enabled / running / can start. Ready to fly. | </summary>
       SAFETY_ARMED=128,
       ///<summary>  | </summary>
       ENUM_END=129,

};



class CmdWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CmdWindow(QWidget *parent = 0);
    ~CmdWindow();
    void InitialControl();
public slots:
//    void on_btn_getParam_clicked(bool noUsed);

//    void on_btn_getWP_clicked(bool noUsed);

//    void on_btn_RTL_clicked(bool noUsed);

//    void on_btn_sendParam_clicked(bool noUsed);

//    void on_btn_sendWP_clicked(bool noUsed);

//    void on_mode_currentIndexChanged(int index);

//    void on_cmd_currentIndexChanged(int index);

private slots:
    void on_btn_cmd_send_clicked();

    void onMissionList(std::vector<mavlink_mission_item_int_t*> items);

    void on_pushClearStream_clicked();

    void on_pushButtonTmp_clicked();

    void onParamValue(mavlink_param_value_t paramValue);

    void on_pushButton_clicked();

    void on_btn_getWP_clicked();

    void on_btn_getParam_clicked();

    void on_btn_RTL_clicked();

    void on_btn_sendWP_clicked();

    void on_btn_sendParam_clicked();

    void on_btn_ModeSet_clicked();

private:
    Ui::CmdWindow *ui;
   // void LoadEvents();
    LinkManager* lm;
    VehicleManager* vm;
    CommunicationInterface* ci;
    Vehicle* vehicle;
};




#endif // CMDWINDOW_H

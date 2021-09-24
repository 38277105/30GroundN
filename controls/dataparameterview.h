#ifndef DATAPARAMETERVIEW_H
#define DATAPARAMETERVIEW_H

#include <QWidget>
#include "vehicles/vehiclestate.h"
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QSpinBox>
#include <QSlider>
#include <QSerialPortInfo>
#include "InfoPanel/infoview.h"
#include "controllers/frmmaincontroller.h"
#include "InfoPanel/critActInfo.h"

namespace Ui {
class DataParameterView;
}

 class DataParameterView : public QWidget
{
    Q_OBJECT

public:
    explicit DataParameterView(QWidget *parent = 0);
    ~DataParameterView();
     Ui::DataParameterView *ui;

     void reSetupUi(QWidget *DataParameterView);

     void UpdateStatus(uint32_t sys_sts,uint16_t gps_sts,uint16_t rtk_sts,uint16_t psi_sts,bool bjs);

     void GetFixType(qint16 status, QString &strFixType);

public slots:
     void update(VehicleState& data);

     void valueChanged(int value);
     void textChanged(QString str);

     void OneKeyTakeOFF(float alt);
     void OneKeyLand();
     void ChangeAlt(float alt);
     void ChangeYAW(float yaw);
     //-----------------------------
     void on_btn_takeoffOnekey_clicked();

     void on_takeoff_after_unlocked();

     void on_btn_land_clicked();

     void on_btn_changeAlt_clicked();

     void on_btn_changeYaw_clicked();

     void on_btn_follow_clicked();

     void on_btn_stick_clicked();

     void on_btn_electric_clicked();

     //列出串口
     void PortsFoundNotify(QList<QSerialPortInfo> portlist);

     void on_btn_connect_clicked();

     QString GetLabHtmlStr(const QString& pig,const QString& name);

public:
     QLabel *label_tips;
     QLabel *label_CompassState;
     QLabel *label_Accelerometer;
     QLabel *label_RemoteController;
     QLabel *label_Digital;
     QLabel *label_GPS;
     QLabel *label_RTK;
     QLabel *label_HeadingRTK;
     QLabel *label_RecData;
     QLabel *label_CalPosition;

     QPushButton *btn_takeoffOnekey;
     QLineEdit *le_height;
     QPushButton *btn_changeYaw;
     QLineEdit *le_yaw;
     QPushButton *btn_changeAlt;
     QLineEdit *le_alt;

     QPushButton *btn_land;
     QPushButton *btn_follow;
     //QPushButton *btn_stick;
     //QPushButton *btn_electirc;//电源按钮
     //QPushButton *btn_electirc_close;//电源按钮

     float m_current_alt;
     QTimer* m_timer;
     float m_takeoff_alt;

     bool IsConnected;

     QString _currentport;
     int     _currentBaud;
     critActInfo m_critActInfo;
 private slots:
     void on_btn_exit_clicked();
     void on_cbx_ports_currentTextChanged(const QString &arg1);
     void on_btn_info_clicked();



     void on_btn_electric_close_clicked();

 signals:
     void signal_setTabEnabled(bool bIsTabEnabled);
 };

#endif // DATAPARAMETERVIEW_H

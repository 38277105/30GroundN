#ifndef PARAMETERCONTROLLER_H
#define PARAMETERCONTROLLER_H

#include <QObject>
#include <QMap>
#include "controls/commoncontrol/croundprogressbar.h"
#include "connections/mavlinkmessageinterface.h"
#include "vehicles/vehicle.h"
#include "vehicles/vehiclestate.h"
#include <connections/communicationinterface.h>
#include <QTimer>
#include "connections/datastruct.h"
#include "controls/parametertableview.h"
#include <frmmain.h>

class LineChartMainWindow;

class ParameterController:public QObject
{
    Q_OBJECT
public:
    ParameterController(Vehicle* t_vehicle);

    ~ParameterController();

    static  ParameterController* Instance();

signals:
   void OnParamRecv(QList<ParamData>*);

   void OnModeDisplay(int flightmode);

   void OnParameterChanged(int uas,int component,QString parameterName,QVariant value);

public slots:

    void request_parameterList();

    void request_flightMode();

    void set_parameter(char* param_id,float param_value,MAV_PARAM_TYPE para_type);

    void set_parameter_noanswer(char* param_id,float param_value,MAV_PARAM_TYPE para_type);

    void send_save_cmd();

    void onParamValue(mavlink_param_value_t paramValue);

    void onParamTimeOut();
public :
    void set_callback(ParameterTableView* view);

    void SetControl(frmMain* m_control);

    CRoundProgressBar* m_progressBar;


private:
    //-----参数设置-------
    int param_index;
    int ParamMaxCount;
    bool m_IsSet;
    bool m_IsGetFilgthMode;

    Vehicle* __vehicle;

    QTimer*  m_timer;

    QTimer*  m_param_timeout_timer;

    frmMain* m_frmMain;

public:
    QMap<QString,ParamData> m_params;
    bool IsExternalUsed;

    LineChartMainWindow* mainWin;
};

#endif // PARAMETERCONTROLLER_H

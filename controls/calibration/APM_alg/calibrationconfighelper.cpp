#include "calibrationconfighelper.h"

CalibrationConfigHelper::CalibrationConfigHelper(QObject *parent) : QObject(parent)
{
    setupUi();
}

void CalibrationConfigHelper::setupUi()
{
    tabWidget=new QTabWidget();
    tab_accelebrateCalibration=new QWidget();
    tab_magnify=new QWidget();
    tab_rc=new QWidget();

    tabWidget->addTab(tab_accelebrateCalibration,"加速度计校准");
    tabWidget->addTab(tab_magnify,"罗盘校准");
    tabWidget->addTab(tab_rc,"遥控器校准");

    m_accelCalibrationConfig=new AccelCalibrationConfig();
    QHBoxLayout* box=new QHBoxLayout();
    box->addWidget(m_accelCalibrationConfig);
    tab_accelebrateCalibration->setLayout(box);

//    m_compassConfig=new CompassConfig();
//    QHBoxLayout* box1=new QHBoxLayout();
//    box1->addWidget(m_compassConfig);
//    tab_magnify->setLayout(box1);

    m_radioCalibrationConfig=new RadioCalibrationConfig();
    QHBoxLayout* box2=new QHBoxLayout();
    box2->addWidget(m_radioCalibrationConfig);
    tab_rc->setLayout(box2);

}


QTabWidget* CalibrationConfigHelper::getWidget()
{
   return tabWidget;
}

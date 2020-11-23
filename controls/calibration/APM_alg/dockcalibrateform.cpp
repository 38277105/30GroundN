#include "dockcalibrateform.h"
#include "ui_dockcalibrateform.h"
#include "CompassConfig.h"
#include "AccelCalibrationConfig.h"
#include "RadioCalibrationConfig.h"

DockCalibrateForm::DockCalibrateForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DockCalibrateForm)
{
    ui->setupUi(this);

    m_accelCalibrationConfig=NULL;
   // m_compassConfig=NULL;
    m_radioCalibrationConfig=NULL;


    //m_compassConfig=new CompassConfig();


    m_accelCalibrationConfig=new AccelCalibrationConfig();


    m_radioCalibrationConfig=new RadioCalibrationConfig();
}

DockCalibrateForm::~DockCalibrateForm()
{
    delete ui;
}

void DockCalibrateForm::on_compassButton_clicked()
{

    ui->stackedWidget->setCurrentWidget(m_compassConfig);
}

void DockCalibrateForm::on_accelCalibrateButton_clicked()
{

    ui->stackedWidget->setCurrentWidget(m_accelCalibrationConfig);
}

void DockCalibrateForm::on_radioCalibrateButton_clicked()
{

    ui->stackedWidget->setCurrentWidget(m_radioCalibrationConfig);
}

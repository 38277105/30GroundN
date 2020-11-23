#include "calibrateform.h"
#include "ui_calibrateform.h"

#include "controls/calibration/AccelCalibrationConfig.h"
//#include "controls/calibration/CompassConfig.h"
#include "controls/calibration/compasscalibratecontrol.h"
#include "controls/calibration/RadioCalibrationConfig.h"
#include "controls/calibration/accelcalibrationofindependent.h"
#include "controls/calibration/motortestform.h"
#include "controls/calibration/esccaliform.h"
CalibrateForm::CalibrateForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalibrateForm)
{
    ui->setupUi(this);
    m_accelCalibrationConfig=NULL;
//    m_compassConfig=NULL;
    m_radioCalibrationConfig=NULL;
    m_compassControl=NULL;
    m_accelCalibrationOfIndependent = NULL;
    m_motorTestForm=NULL;
    m_esc_cali_from=NULL;

    initialContent();
}

CalibrateForm::~CalibrateForm()
{
    delete ui;
    if(m_accelCalibrationConfig!=NULL)
    {
        delete m_accelCalibrationConfig;
        m_accelCalibrationConfig=NULL;
    }
    if(NULL!=m_radioCalibrationConfig)
    {
        delete m_radioCalibrationConfig;
        m_radioCalibrationConfig=NULL;
    }
    if(NULL!=m_compassControl)
    {
        delete m_compassControl;
        m_compassControl=NULL;
    }
    if(NULL!=m_accelCalibrationOfIndependent)
    {
        delete m_accelCalibrationOfIndependent;
        m_accelCalibrationOfIndependent=NULL;
    }
    if(NULL!=m_motorTestForm)
    {
        delete m_motorTestForm;
        m_motorTestForm=NULL;
    }
}

void CalibrateForm::initialContent()
{
#ifdef ZYFlightMode_ENABLED
    if(m_accelCalibrationOfIndependent==NULL)
        m_accelCalibrationOfIndependent=new AccelCalibrationOfIndependent();
    QHBoxLayout* box=new QHBoxLayout();
    box->setDirection(QHBoxLayout::LeftToRight);
    box->addWidget(m_accelCalibrationOfIndependent);
    this->ui->tab_accelebrateCalibration->setLayout(box);
#else
      if(m_accelCalibrationConfig==NULL)
      m_accelCalibrationConfig=new AccelCalibrationConfig();
      QHBoxLayout* box=new QHBoxLayout();
      box->setDirection(QHBoxLayout::LeftToRight);
      box->addWidget(m_accelCalibrationConfig);
      this->ui->tab_accelebrateCalibration->setLayout(box);

#endif



//  if(m_compassConfig==NULL)
//  m_compassConfig=new CompassConfig();
//  QHBoxLayout* box1=new QHBoxLayout();
//  box1->setDirection(QHBoxLayout::LeftToRight);
//  box1->addWidget(m_compassConfig);
//  this->ui->tab_magnify->setLayout(box1);
  if(m_compassControl==NULL)
      m_compassControl=new CompassCalibrateControl();
    QHBoxLayout* box1=new QHBoxLayout();
    box1->setDirection(QHBoxLayout::LeftToRight);
    box1->addWidget(m_compassControl);
    this->ui->tab_magnify->setLayout(box1);


  if(m_radioCalibrationConfig==NULL)
  m_radioCalibrationConfig=new RadioCalibrationConfig();
  QHBoxLayout* box2=new QHBoxLayout();
  box2->setDirection(QHBoxLayout::LeftToRight);
  box2->addWidget(m_radioCalibrationConfig);
  QSpacerItem* item=new QSpacerItem(1000,50);
  box2->addItem(item);
  this->ui->tab_rc->setLayout(box2);

  if(m_motorTestForm==NULL)
  {
      m_motorTestForm=new MotorTestForm();
      QHBoxLayout* box3=new QHBoxLayout();
      box3->setDirection(QHBoxLayout::LeftToRight);
      box3->addWidget(m_motorTestForm);
      this->ui->tab_motorTest->setLayout(box3);
  }

  if(NULL == m_esc_cali_from)
  {
      m_esc_cali_from = new ESCCaliForm();
      QHBoxLayout* box4=new QHBoxLayout();
      box4->setDirection(QHBoxLayout::LeftToRight);
      box4->addWidget(m_esc_cali_from);
      this->ui->tab_esc_cali_en->setLayout(box4);
  }
}

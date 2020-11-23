#include "motortestform.h"
#include "ui_motortestform.h"
#include "controllers/frmmaincontroller.h"
#include "settingcontrols/settingmanager.h"
#include "../../Utils/zygroundglobalconfig.h"


MotorTestForm::MotorTestForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MotorTestForm)
{
    ui->setupUi(this);

    IsCreated=false;
    MaxMotor=0;
    // 微调框
    ui->spinBox->setMinimum(0);  // 最小值
    ui->spinBox->setMaximum(100);  // 最大值
    ui->spinBox->setSingleStep(1);  // 步长

    // 滑动条
    ui->horizontalSlider->setMinimum(0);  // 最小值
    ui->horizontalSlider->setMaximum(100);  // 最大值
    ui->horizontalSlider->setSingleStep(1);  // 步长

    // 连接信号槽（相互改变）
    connect(ui->spinBox, SIGNAL(valueChanged(int)), ui->horizontalSlider, SLOT(setValue(int)));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), ui->spinBox, SLOT(setValue(int)));

    ui->spinBox->setValue(0);
    uavType =14;
}

MotorTestForm::~MotorTestForm()
{
    delete ui;
}

void MotorTestForm::CreateButtonByType(uint8_t uavType)
{

  qDebug()<<"*************************************************** uavType:>>"<<uavType;

  if(uavType==2)
  {

    connect(this->ui->pushButton, SIGNAL(clicked()), this, SLOT(MotorTestClick()));
    connect(this->ui->pushButton_2, SIGNAL(clicked()), this, SLOT(MotorTestClick()));
    connect(this->ui->pushButton_3, SIGNAL(clicked()), this, SLOT(MotorTestClick()));
    connect(this->ui->pushButton_4, SIGNAL(clicked()), this, SLOT(MotorTestClick()));
    this->ui->pushButton_5->setVisible(false);
    this->ui->pushButton_6->setVisible(false);
    this->ui->pushButton_7->setVisible(false);
    this->ui->pushButton_8->setVisible(false);
     MaxMotor=4;
  }
  else if(uavType==13)
  {
      connect(this->ui->pushButton, SIGNAL(clicked()), this, SLOT(MotorTestClick()));
      connect(this->ui->pushButton_2, SIGNAL(clicked()), this, SLOT(MotorTestClick()));
      connect(this->ui->pushButton_3, SIGNAL(clicked()), this, SLOT(MotorTestClick()));
      connect(this->ui->pushButton_4, SIGNAL(clicked()), this, SLOT(MotorTestClick()));
      connect(this->ui->pushButton_5, SIGNAL(clicked()), this, SLOT(MotorTestClick()));
      connect(this->ui->pushButton_6, SIGNAL(clicked()), this, SLOT(MotorTestClick()));

      this->ui->pushButton_7->setVisible(false);
      this->ui->pushButton_8->setVisible(false);
       MaxMotor=6;
  }
  else if(uavType==14)
  {
      connect(this->ui->pushButton, SIGNAL(clicked()), this, SLOT(MotorTestClick()));
      connect(this->ui->pushButton_2, SIGNAL(clicked()), this, SLOT(MotorTestClick()));
      connect(this->ui->pushButton_3, SIGNAL(clicked()), this, SLOT(MotorTestClick()));
      connect(this->ui->pushButton_4, SIGNAL(clicked()), this, SLOT(MotorTestClick()));
      connect(this->ui->pushButton_5, SIGNAL(clicked()), this, SLOT(MotorTestClick()));
      connect(this->ui->pushButton_6, SIGNAL(clicked()), this, SLOT(MotorTestClick()));
      connect(this->ui->pushButton_7, SIGNAL(clicked()), this, SLOT(MotorTestClick()));
      connect(this->ui->pushButton_8, SIGNAL(clicked()), this, SLOT(MotorTestClick()));
       MaxMotor=8;

  }
  connect(this->ui->pushButton_9,SIGNAL(clicked(bool)),this,SLOT(MotorTestClick()));
 this->update();

}
void MotorTestForm::SendCommandByOrder(int motorIndex,float throttle,float timeout)
{
    int nThrottle = throttle*10+1000;
    QThread::msleep(20);
    qDebug()<<"int motorIndex,float throttle,float timeout"<<motorIndex<<throttle<<timeout;
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.doCommand(MAV_CMD_DO_SET_SERVO, (float)motorIndex,
                    (float)nThrottle, 0, 0, 0, 0,0);
    qDebug()<<"<0524>-----------motorIndex:"<<motorIndex<<"|"<<(float)throttle<<"-->timeOut|"<<timeout;
    QThread::msleep(20);
}

void MotorTestForm::MotorTestClick()
{
    QPushButton* btn=(QPushButton*)sender();
    qDebug()<<"btnName:"<<btn->objectName();
    int throttle = ui->spinBox->value();
    if(btn->text().contains("A"))
    {
        SendCommandByOrder(1,throttle,2);
    }
    else if(btn->text().contains("B"))
    {
        SendCommandByOrder(2,throttle,2);
    }
    else if(btn->text().contains("C"))
    {
        SendCommandByOrder(3,throttle,2);
    }
    else if(btn->text().contains("D"))
    {
        SendCommandByOrder(4,throttle,2);
    }
    else if(btn->text().contains("E"))
    {
        SendCommandByOrder(5,throttle,2);
    }
    else if(btn->text().contains("F"))
    {
        SendCommandByOrder(6,throttle,2);
    }
    else if(btn->text().contains("G"))
    {
        SendCommandByOrder(7,throttle,2);
    }
    else if(btn->text().contains("H"))
    {
        SendCommandByOrder(8,throttle,2);
    }
    else if(btn->text().compare(QString::fromLocal8Bit("所有电机"))==0)
    {

        int i=1;
        while(i<=MaxMotor)
        {
            for(int j=0;j<2;j++)
            {
              SendCommandByOrder(i,throttle,2);
            }
             i++;
        }

    }
}


void MotorTestForm::showEvent(QShowEvent* event)
{
    qDebug()<<"serial port has open:>>"<<(int)ZYGroundGlobalConfig::m_SerialIsOpen;
    if(ZYGroundGlobalConfig::m_SerialIsOpen&&!IsCreated)
    {
        //2:QUAD(4axis)
        //13:HEXAROTOR(6axis)
        //14:OCTOROTOR(4axis8jiang)
        //FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.getUAVType(uavType);

        CreateButtonByType(uavType);
        IsCreated=true;
    }
}

void MotorTestForm::hideEvent(QHideEvent* event)
{

}

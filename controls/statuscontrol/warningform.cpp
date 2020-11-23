#include "warningform.h"
#include "ui_warningform.h"

WarningForm::WarningForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WarningForm)
{
    ui->setupUi(this);
    ui->RdBtn_RemoteController->setStyleSheet("QRadioButton::indicator {  border-radius: 8px;background-color: green;}");
    ui->RdBtn_GPS->setStyleSheet("QRadioButton::indicator {  border-radius: 8px;background-color: green;}");
    ui->RdBtn_RTK->setStyleSheet("QRadioButton::indicator {  border-radius: 8px;background-color: green;}");
    ui->RdBtn_PsiRTK->setStyleSheet("QRadioButton::indicator {  border-radius: 8px;background-color: green;}");
    //UpdateStatus(0x00FF00FF,0x0311,0x0109,0x0213);
}

WarningForm::~WarningForm()
{
    delete ui;
}

QString WarningForm::getMsg(uint16_t nSts)
{
    uint8_t height,low;
    height=nSts/256;
    low=nSts%256;
    QString str;
    switch (height)
    {
    case 0:
        str = "NoLink ";
        break;
    case 1:
        str = "NoPos";
        break;
    case 2:
        str = "2DLock";
        break;
    case 3:
        str = "3DLock";
        break;
    case 4:
        str = "DGPS";
        break;
    case 5:
        str = "Float";
        break;
    case 6:
        str = "Fixed";
        break;
    default:
        str = "UnKnown";
        break;
    }
    str+= " "+QString::number(low);
    return str;
}

void WarningForm::UpdateStatus(uint32_t sys_sts,uint16_t gps_sts,uint16_t rtk_sts,uint16_t psirtk_sts)
{
    ui->RdBtn_GPS->setText("GPS "+getMsg(gps_sts));
    ui->RdBtn_RTK->setText("RTK "+getMsg(rtk_sts));
    ui->RdBtn_PsiRTK->setText("航向RTK "+getMsg(psirtk_sts));

    //RTK5GPS6航向RTK13遥控器17
    if(sys_sts&0x00010000)
    {
        ui->RdBtn_RemoteController->setText("遥控器已连接");
        ui->RdBtn_RemoteController->setStyleSheet("QRadioButton::indicator {  border-radius: 8px;background-color: green;}");
    }
    else
    {
        ui->RdBtn_RemoteController->setText("遥控器未连接");
        ui->RdBtn_RemoteController->setStyleSheet("QRadioButton::indicator {  border-radius: 8px;background-color: red;}");
    }
    if(sys_sts&0x00001000)
    {
        ui->RdBtn_PsiRTK->setStyleSheet("QRadioButton::indicator {  border-radius: 8px;background-color: green;}");
    }
    else
    {
        ui->RdBtn_PsiRTK->setStyleSheet("QRadioButton::indicator {  border-radius: 8px;background-color: red;}");
    }
    if(sys_sts&0x00000010)
    {
        ui->RdBtn_RTK->setStyleSheet("QRadioButton::indicator {  border-radius: 8px;background-color: green;}");
    }
    else
    {
        ui->RdBtn_RTK->setStyleSheet("QRadioButton::indicator {  border-radius: 8px;background-color: red;}");
    }
    if(sys_sts&0x00000020)
    {
        ui->RdBtn_GPS->setStyleSheet("QRadioButton::indicator {  border-radius: 8px;background-color: green;}");
    }
    else
    {
        ui->RdBtn_GPS->setStyleSheet("QRadioButton::indicator {  border-radius: 8px;background-color: red;}");
    }
}


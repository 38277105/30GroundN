#include "esccaliform.h"
#include "ui_esccaliform.h"
#include "../../Utils/zygroundglobalconfig.h"
#include "../../controls/myhelper.h"
#include "../../controllers/frmmaincontroller.h"


ESCCaliForm::ESCCaliForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ESCCaliForm)
{
    ui->setupUi(this);
    m_timer.setInterval(1000);
    m_timer.setSingleShot(false);
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(timer_tick()));
    g_bSetESC = false;
    m_setCount = 0;
    m_bIsCalibrating = false;
}

ESCCaliForm::~ESCCaliForm()
{
    delete ui;
}

void ESCCaliForm::on_btn_ESCCali_clicked()
{
    if(false == m_bIsCalibrating)
    {
        int i = 1;
        float f;
        memcpy(&f, (void*)&i, sizeof(int));
        FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(f,(char *)"ESC_CALI_EN",MAV_PARAM_TYPE_INT32);
        m_timer.start();
        ui->btn_ESCCali->setEnabled(false);
        g_bSetESC = false;
        m_setCount = 0;
        ui->btn_ESCCali->setText(QStringLiteral("校准完成"));
    }
    else
    {
        ui->btn_ESCCali->setText(QStringLiteral("开始校准"));
    }
    m_bIsCalibrating = !m_bIsCalibrating;
}

void ESCCaliForm::timer_tick()
{
   if(2 == m_setCount)
   {
       m_timer.stop();
       myHelper::ShowMessageBoxInfo("设置参数失败");
       ui->btn_ESCCali->setEnabled(true);
       ui->btn_ESCCali->setText(QStringLiteral("开始校准"));
       return;
   }
   if(!g_bSetESC)
   {
       m_setCount++;
       int i = 1;
       float f;
       memcpy(&f, (void*)&i, sizeof(int));
       FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.paramSet(f,(char *)"ESC_CALI_EN",MAV_PARAM_TYPE_INT32);
   }
   else
   {
       m_timer.stop();
       myHelper::ShowMessageBoxInfo("设置参数成功，请将飞控断电");
       ui->btn_ESCCali->setEnabled(true);
   }
}

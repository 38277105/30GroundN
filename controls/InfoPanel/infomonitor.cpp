#include "infomonitor.h"
#include "ui_infomonitor.h"
#include "../Utils/zygroundglobalconfig.h"

InfoMonitor::InfoMonitor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InfoMonitor)
{
    ui->setupUi(this);
    int ftSize=12;
    if(ZYGroundGlobalConfig::RESOLUTION_1920)
        ftSize=15;
    QString valStyle=QString("font: %1pt \"黑体\"; color: rgb(233, 233, 233);\
                              background-color: rgb(125, 125, 125);").arg(ftSize);
    QLabel* pLab;
    QImageLabel* pImgLab;
    for(int i=0;i<MAX_NUM;++i)
    {
         pLab = findChild<QLabel *>("labVal"+QString::number(i+1));
         pLab->setStyleSheet(valStyle);
         pLab->setAlignment(Qt::AlignCenter);

         pImgLab = findChild<QImageLabel*>("labDesc"+QString::number(i+1));
         pImgLab->setImage(":/image/control/control_01.png");
         pImgLab->setFontSize(ftSize);

         pLab = findChild<QLabel*>("labDan"+QString::number(i+1));
         if(pLab)
             pLab->setStyleSheet(valStyle);
    }
    setStatus(0,0x340045,3,2,5);
}

InfoMonitor::~InfoMonitor()
{
    delete ui;
}

void InfoMonitor::setStatus(uint32_t sys_info1, uint32_t sys_info2, uint16_t vdop, uint16_t hdop, int32_t h)
{
    UnExtendinfo info;
    info.info=sys_info1;
    //水平定位
    if(info.hGPS)
        ui->labVal1->setText("GPS");
    else if(info.hVison)
        ui->labVal1->setText(QString::fromLocal8Bit("视觉"));
    else
        ui->labVal1->setText(QString::fromLocal8Bit("无定位"));
    //垂直定位
    if(info.vAcc)
        ui->labVal2->setText(QString::fromLocal8Bit("气压计"));
    else if(info.vGPS)
        ui->labVal2->setText("GPS");
    else if(info.vVison)
        ui->labVal2->setText(QString::fromLocal8Bit("视觉"));
    else
        ui->labVal2->setText(QString::fromLocal8Bit("无定位"));
    //定向
    if(info.yawRTK)
        ui->labVal3->setText("RTK");
    else if(info.yawMag)
        ui->labVal3->setText(QString::fromLocal8Bit("磁"));
    else
        ui->labVal3->setText(QString::fromLocal8Bit("无定向"));
    //RTK角度
    ui->labVal4->setText(QString::number(info.angleRTK));
    //磁1和磁2角度
    uint16_t magAngle1=(uint16_t)(sys_info2&0xFFFF);
    ui->labVal5->setText(QString::number(magAngle1));
    uint16_t magAngle2=(uint16_t)((sys_info2>>16)&0xFFFF);
    ui->labVal6->setText(QString::number(magAngle2));

    ui->labVal7->setText(QString::number(h)); //GPS高度
    ui->labVal8->setText(QString::number(vdop));  //VDOP值
    ui->labVal9->setText(QString::number(hdop));  //HDOP值
}

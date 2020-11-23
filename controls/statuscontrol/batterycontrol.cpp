#include "batterycontrol.h"
#include "ui_batterycontrol.h"

BatteryControl::BatteryControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BatteryControl)
{
    ui->setupUi(this);

    m_imageList.append(QStringLiteral(":/image/signals/battery_0.png"));
    m_imageList.append(QStringLiteral(":/image/signals/battery_1.png"));
    m_imageList.append(QStringLiteral(":/image/signals/battery_2.png"));
    m_imageList.append(QStringLiteral(":/image/signals/battery_3.png"));
    m_imageList.append(QStringLiteral(":/image/signals/battery_4.png"));
}

BatteryControl::~BatteryControl()
{
    delete ui;
}
void BatteryControl::setValue(int left)
{
    if(left<0)
        return;
    ui->lbl_desc->setText(QString::number(left));
    left=(int)left/25;
    QPixmap pixmap(m_imageList.at(left));
    ui->lbl_image->setPixmap(pixmap);

}

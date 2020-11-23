#include "signalcontrol.h"
#include "ui_signalcontrol.h"

SignalControl::SignalControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignalControl)
{
    ui->setupUi(this);
    m_imageList.append(QStringLiteral(":/image/signals/signal_0.png"));
    m_imageList.append(QStringLiteral(":/image/signals/signal_1.png"));
    m_imageList.append(QStringLiteral(":/image/signals/signal_2.png"));
    m_imageList.append(QStringLiteral(":/image/signals/signal_3.png"));
    m_imageList.append(QStringLiteral(":/image/signals/signal_4.png"));
    m_imageList.append(QStringLiteral(":/image/signals/signal_5.png"));
}

SignalControl::~SignalControl()
{
    delete ui;
}
void SignalControl::setSignalLevel(int level)
{

//    QBrush brush= QBrush(QImage().scaled(ui->lbl_signal->width(),ui->lbl_signal->height()));
//    palette.setBrush(ui->lbl_signal->backgroundRole(),brush);
//    ui->lbl_signal->setPalette(palette);

    if(level>5)
        level=5;
    if(level<0)
        level=0;
    level=level;
    QPixmap pixmap(m_imageList.at(level));
    ui->lbl_signal->setPixmap(pixmap);

   switch(level)
    {
        case 5:
        {
            ui->lbl_descript->setText(QString::fromLocal8Bit("无"));
            break;
        }
       case 4:
       {
           ui->lbl_descript->setText(QString::fromLocal8Bit("较弱"));
           break;
       }
       case 3:
       {
           ui->lbl_descript->setText(QString::fromLocal8Bit("弱"));
           break;
       }
       case 2:
       {
           ui->lbl_descript->setText(QString::fromLocal8Bit("一般"));
           break;
       }
       case 1:
       {
           ui->lbl_descript->setText(QString::fromLocal8Bit("强"));
           break;
       }
       case 0:
       {
           ui->lbl_descript->setText(QString::fromLocal8Bit("较强"));
           break;
       }
        default:
            break;
    }
}

void SignalControl::setMode(QString mode)
{
    QPixmap pixmap(QStringLiteral(":/image/signals/airplane_mode.png"));
    ui->lbl_signal->setPixmap(pixmap);
    ui->lbl_descript->setText(mode);
}

void SignalControl::setVolt(float volt)
{
    QPixmap pixmap(QStringLiteral(":/image/signals/Voltage.png"));
    ui->lbl_signal->setPixmap(pixmap);
    ui->lbl_descript->setText(QString::asprintf("%01f",volt));
}


void SignalControl::setLockState(bool IsLocked)
{
   if(IsLocked)
   {
    QPixmap pixmap(QStringLiteral(":/image/signals/locked.png"));
    ui->lbl_signal->setPixmap(pixmap);
    ui->lbl_descript->setText(QString::fromLocal8Bit("锁定"));
   }
   else
   {
       QPixmap pixmap(QStringLiteral(":/image/signals/unlocked.png"));
       ui->lbl_signal->setPixmap(pixmap);
       ui->lbl_descript->setText(QString::fromLocal8Bit("解锁"));
   }
}

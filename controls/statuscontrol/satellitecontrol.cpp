#include "satellitecontrol.h"
#include "ui_satellitecontrol.h"

SatelliteControl::SatelliteControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SatelliteControl)
{
    ui->setupUi(this);
}

SatelliteControl::~SatelliteControl()
{
    delete ui;
}
void SatelliteControl::setSatelliteNum(int num)
{
    this->setMinimumWidth(60);

//    QPalette palette;
//    QBrush brush= QBrush(QImage(QStringLiteral(":/image/signals/satellite_0.png")).scaled( ui->lbl_image->width(),ui->lbl_image->height()));
//    palette.setBrush(ui->lbl_image->backgroundRole(),brush);
//    ui->lbl_image->setPalette(palette);
    QPixmap pixmap(":/image/signals/satellite_0.png");
    ui->lbl_image->setPixmap(pixmap);

   ui->lbl_desc->setText(QString::number(num));
}
void SatelliteControl::setSatelliteState(int fix_type)
{
     this->setMinimumWidth(114);
   switch(fix_type)
   {
   case 0:
   {
       QPixmap pixmap(":/image/signals/nofixed.png");
       ui->lbl_image->setPixmap(pixmap);
       ui->lbl_desc->setText(QString::fromLocal8Bit("no fix"));
   }
   break;
   case 1:
   {
       QPixmap pixmap(":/image/signals/nofixed.png");
       ui->lbl_image->setPixmap(pixmap);
       ui->lbl_desc->setText(QString::fromLocal8Bit("no fix"));
   }
   break;
   case 2:
   {
       QPixmap pixmap(":/image/signals/fixed.png");
       ui->lbl_image->setPixmap(pixmap);
       ui->lbl_desc->setText(QString::fromLocal8Bit("2D fixed"));
   }
   break;
   case 3:
   {
       QPixmap pixmap(":/image/signals/fixed.png");
       ui->lbl_image->setPixmap(pixmap);
       ui->lbl_desc->setText(QString::fromLocal8Bit("3D fixed"));
   }
        break;
   case 4:
   {
       QPixmap pixmap(":/image/signals/fixed.png");
       ui->lbl_image->setPixmap(pixmap);
       ui->lbl_desc->setText(QString::fromLocal8Bit("RTK fixed"));
   }
        break;
   case 5:
   {
       QPixmap pixmap(":/image/signals/nofixed.png");
       ui->lbl_image->setPixmap(pixmap);
       ui->lbl_desc->setText(QString::fromLocal8Bit("RTK float"));
   }
   break;
   default:
   {
    QPixmap pixmap(":/image/signals/nofixed.png");
    ui->lbl_image->setPixmap(pixmap);
    ui->lbl_desc->setText(QString::fromLocal8Bit("--"));
     this->setMinimumWidth(60);
   }
       break;
   }

}

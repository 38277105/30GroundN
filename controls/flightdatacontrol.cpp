#include "flightdatacontrol.h"
#include "svgview.h"
#include <QLabel>
#include "controls/iconhelper.h"
#include <QGridLayout>
#include "picturebox.h"
flightdatacontrol::flightdatacontrol(QWidget *parent) : QWidget(parent)
{
    this->LoadConfig();
    //this->LoadLayout();
}

void flightdatacontrol::LoadConfig()
{
    QGridLayout* layout=new QGridLayout(this);
    int fontindex=0xf001;
    for(int i=0;i<600;i++)
    {
        QLabel* lbl_test=new QLabel(this);
        lbl_test->move(i*30,(i%10)*30);
        IconHelper::Instance()->SetIcon(lbl_test,QChar(fontindex),20);
        layout->addWidget(lbl_test,(i/30),(i%30));
        fontindex++;
    }
    this->setLayout(layout);
}

void flightdatacontrol::LoadLayout()
{
    //stateholder
    QImage _image(":/img_apm/flightdata.png");
    PictureBox *box=new PictureBox();
    m_resourcelst.push_back(box);//垃圾回收
    box->setMode(PictureBox::AUTO_ZOOM);
    box->setImage(_image);
    QBrush _brush(Qt::transparent);
    box->setBackground(_brush);
    box->setSelected();
    box->setFixedWidth(400);
    box->setFixedHeight(300);
    //this->addWidget(box);
}

void flightdatacontrol::LoadEvents()
{

}

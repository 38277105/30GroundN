#include "infoview.h"

InfoView::InfoView(QWidget *parent) : QWidget(parent)
{
    st_X = 0;
    st_Y = 0;

    m_flowLayout=new FlowLayout();

    m_lontitudeItem=new InfoViewItem(QString::fromLocal8Bit("经度:"));
    m_latitudeItem=new InfoViewItem(QString::fromLocal8Bit("纬度:"));
    m_altitudeItem=new InfoViewItem(QString::fromLocal8Bit("海拔:"));
    m_x=new InfoViewItem(QString::fromLocal8Bit("X(东):"));
    m_y=new InfoViewItem(QString::fromLocal8Bit("Y(北):"));
    m_z=new InfoViewItem(QString::fromLocal8Bit("Z(天):"));
    m_rollItem=new InfoViewItem(QString::fromLocal8Bit("滚转:"));
    m_yawItem=new InfoViewItem(QString::fromLocal8Bit("偏航:"));
    m_pitchItem=new InfoViewItem(QString::fromLocal8Bit("俯仰:"));
    m_satelliteCountItem=new InfoViewItem(QString::fromLocal8Bit("星数:"));
    m_groundspeedItem=new InfoViewItem(QString::fromLocal8Bit("速度"));
    m_climbspeedItem=new InfoViewItem(QString::fromLocal8Bit("升速"));


    m_lontitudeItem->SetValue("118.613921");
    m_latitudeItem->SetValue("31.943511");
    m_altitudeItem->SetValue("0.021311");
    m_x->SetValue("0.000");
    m_y->SetValue("1.000");
    m_z->SetValue("1.245");
    m_rollItem->SetValue("1.02");
    m_yawItem->SetValue("1.34");
    m_pitchItem->SetValue("1.13");
    m_satelliteCountItem->SetValue("0");
    m_groundspeedItem->SetValue("1.943");
    m_climbspeedItem->SetValue("1.156");



    m_flowLayout->addWidget(m_lontitudeItem);
    m_flowLayout->addWidget(m_latitudeItem);
    m_flowLayout->addWidget(m_altitudeItem);

    m_flowLayout->addWidget(m_x);
    m_flowLayout->addWidget(m_y);
    m_flowLayout->addWidget(m_z);

    m_flowLayout->addWidget(m_rollItem);
    m_flowLayout->addWidget(m_yawItem);
    m_flowLayout->addWidget(m_pitchItem);
    m_flowLayout->addWidget(m_satelliteCountItem);

    m_flowLayout->addWidget(m_groundspeedItem);
    m_flowLayout->addWidget(m_climbspeedItem);

    this->setLayout(m_flowLayout);
}

void InfoView::resetXY(){
    st_X = m_lastX;
    st_Y = m_lastY;
}

void InfoView::update(VehicleState& state)
{
    QString s_temp;
    m_lontitudeItem->SetValue(s_temp.sprintf("%.06f",state.lontitude));
    m_latitudeItem->SetValue(s_temp.sprintf("%.06f",state.latitude));
    m_altitudeItem->SetValue(s_temp.sprintf("%.03f",state.height));
    m_lastX = state.X;
    m_lastY = state.Y;
    m_x->SetValue(s_temp.sprintf("%.03f",state.X));
    m_y->SetValue(s_temp.sprintf("%.03f",state.Y));
    m_z->SetValue(s_temp.sprintf("%.03f",state.Z));
    m_rollItem->SetValue(s_temp.sprintf("%.02f",state.roll));
    m_rollItem->SetRange(-15,15);   //gyq add
    m_yawItem->SetValue(s_temp.sprintf("%.02f",state.yaw));
    m_pitchItem->SetValue(s_temp.sprintf("%.02f",state.pitch));
    m_pitchItem->SetRange(-15,15);  //gyq add
    m_satelliteCountItem->SetValue(s_temp.sprintf("%d",state.starNum));
    m_groundspeedItem->SetValue(s_temp.sprintf("%.03f",state.groundspeed));
    if(state.climb>10.0f||state.climb<-10.0f)
        return;
    m_climbspeedItem->SetValue(s_temp.sprintf("%.03f",state.climb));
}

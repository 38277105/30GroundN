#include "spdwidget.h"

//设置表盘的中心
#define CenterPoint_X  80
#define CenterPoint_Y  80

//设置指针的中心，注意这里是负的，因为一会我们要平移坐标系
#define NeedleCenter_X     -10   //
#define NeedleCenter_Y     -50

//指针图片的宽度和高度
#define NeedlePic_W         13
#define NeedlePic_H         57

//指针可以活动的最小最大值
#define Needle_MinAngle    -140
#define Needle_MaxAngle     140



//初始化
SPDWidget::SPDWidget(QWidget *parent) :
    QWidget(parent)
{
    pix_indicator.load(":/image/indicator.png");
    pix_instrument.load(":/image/instrument.png");

    d_Pointer_angle_now = Needle_MinAngle;   //start angle

    connect(this,SIGNAL(signal_throttleChanged()),this,SLOT(update()));


}

//画图
void SPDWidget::paintEvent(QPaintEvent  *event)
{
    Q_UNUSED(event);
    //每次调用这个函数，改变角度值

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    painter.drawPixmap(0,0,160*m_scale,160*m_scale,pix_instrument);

    //-----------------------------------------
    painter.save();
    //设置新的圆心，其实也是旋转中心
    painter.translate(CenterPoint_X*m_scale, CenterPoint_Y*m_scale);
    //旋转一个角度
    painter.rotate(d_Pointer_angle_now);
    //画指针，注意是在指针中心的反方向开始画
    painter.drawPixmap(NeedleCenter_X*m_scale, NeedleCenter_Y*m_scale,
                       NeedlePic_W*m_scale, NeedlePic_H*m_scale,
                       pix_indicator);
    //使原点复原
    painter.restore();
}

//测试函数，在最小与最大范围之间摆动
void SPDWidget::set_angle(qint32 throttle_now)
{
   if(throttle_now > 120)
   {
       throttle_now = 120;
   }
   else if(throttle_now < 0)
   {
       throttle_now = 0;
   }
   d_Pointer_angle_now = (throttle_now - 60)*140/60;

   emit signal_throttleChanged();

}


void SPDWidget::set_scale(float scale)
{
    m_scale = scale;
    this->resize(160*m_scale, 110*m_scale);
}

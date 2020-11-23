#include "qwt_time_scale_draw.h"

qwt_time_scale_draw::qwt_time_scale_draw(const QDateTime &base) : m_scaleBaseTime(base)
{

}

QwtText qwt_time_scale_draw::label(double v) const
{
    QDateTime upTime = m_scaleBaseTime.addMSecs((int )v);
    return upTime.toString("yyyy-MM-dd\nhh:mm:ss");
}
qwt_time_scale_draw::~qwt_time_scale_draw()
{

}

#include "qwt_time_plot.h"

qwt_time_plot::qwt_time_plot(const QwtText &title) : QwtPlot(title)
{
    QDateTime m_baseTime = QDateTime::currentDateTime();
    setAxisScaleDraw(QwtPlot::xBottom,new qwt_time_scale_draw(m_baseTime));
}


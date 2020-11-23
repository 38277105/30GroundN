#ifndef QWT_TIME_PLOT_H
#define QWT_TIME_PLOT_H

#include <QObject>
#include "qwt_plot.h"
#include "qwt_time_scale_draw.h"

class qwt_time_plot : public QwtPlot
{
public:
    qwt_time_plot(const QwtText &title);

signals:

public slots:
};

#endif // QWT_TIME_PLOT_H

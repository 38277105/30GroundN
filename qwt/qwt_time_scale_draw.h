#ifndef QWT_TIME_SCALE_DRQW_H
#define QWT_TIME_SCALE_DRQW_H

#include <QObject>
#include "qwt_scale_draw.h"
#include <QDateTime>

class qwt_time_scale_draw : public QwtScaleDraw
{
public:
     qwt_time_scale_draw(const QDateTime &base);
     virtual ~qwt_time_scale_draw();

      virtual QwtText label(double v) const ;

signals:

public slots:
private:
     QDateTime m_scaleBaseTime;
};

#endif // QWT_TIME_SCALE_DRQW_H

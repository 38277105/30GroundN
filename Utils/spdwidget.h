#ifndef SPDWIDGET_H
#define SPDWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QLabel>

class SPDWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SPDWidget(QWidget *parent = 0);

    QPixmap pix_indicator;
    QPixmap pix_instrument;
    qint32 d_Pointer_angle_now;
    float m_scale;

protected:
    void paintEvent(QPaintEvent  *event);

signals:

    void signal_throttleChanged();

public slots:
    void set_angle(qint32 throttle_now);
    void set_scale(float scale);
};

#endif // SPDWIDGET_H

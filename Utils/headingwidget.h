#ifndef HEADINGWIDGET_H
#define HEADINGWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QLabel>

class HeadingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HeadingWidget(QWidget *parent = 0);

    QPixmap pix_indicator;
    QPixmap pix_compass;
    qint32 d_Pointer_angle_now;
    float m_scale;

protected:
    void paintEvent(QPaintEvent  *event);

signals:

    void signal_headingChanged();

public slots:
    void set_angle(qint32 throttle_now);

    void set_scale(float scale);
};

#endif // SPDWIDGET_H

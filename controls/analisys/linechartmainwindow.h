#ifndef LINECHARTMAINWINDOW_H
#define LINECHARTMAINWINDOW_H

#include <QMainWindow>

#include "MAVLinkDecoder.h"
#include <QPointer>

class Linecharts;

class LineChartMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit LineChartMainWindow(QWidget *parent = 0);

signals:
    void valueChanged(const int uasId, const QString& name, const QString& unit, const QVariant& value, const quint64 msec);
public slots:



private:
    Linecharts* m_linecharts;
    QPointer<MAVLinkDecoder> m_decoder;
};

#endif // LINECHARTMAINWINDOW_H

#ifndef FLIGHTDATACONTROL_H
#define FLIGHTDATACONTROL_H

#include <QWidget>

class flightdatacontrol : public QWidget
{
    Q_OBJECT
public:
    explicit flightdatacontrol(QWidget *parent = 0);

private:
    QList<QObject*> m_resourcelst;

    void LoadConfig();

    void LoadLayout();

    void LoadEvents();

signals:

public slots:

};

#endif // FLIGHTDATACONTROL_H

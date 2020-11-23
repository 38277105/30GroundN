#ifndef SERIALPORTFINDER_H
#define SERIALPORTFINDER_H

#include <QObject>
#include <QTimer>
#include <QSerialPortInfo>
#include <QMap>
#include <QThread>

class SerialPortFinder : public QThread
{
    Q_OBJECT
public:
    explicit SerialPortFinder(QObject *parent = 0);
    ~SerialPortFinder();
signals:
   void SerialPortChangedNotify(QList<QSerialPortInfo> portlist);
public slots:
    void portQuery();
protected:
    void run();
private:
    QTimer* m_QueryTimer;
    QMap<QString,QSerialPortInfo> m_ports;
    int m_portCount;


};

#endif // SERIALPORTFINDER_H

#ifndef SERIALLINK_H
#define SERIALLINK_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>
#include <QFile>
#include "communicationinterface.h"
//#include "qmutex.h"
#include "./Utils/myserialport.h"

class UDPLink;
class LinkInterface;

typedef void (*messagerecvFun)(mavlink_message_t message);

class SerialLink :public CommunicationInterface
{
    Q_OBJECT
public:
    SerialLink();
    ~SerialLink();

    bool    linkOpen(QString portName,int BaudRate);
    bool    linkClose();
    void    SetCallBack(messagerecvFun fun);
    void    run();
    void    loadConfig();

    void    queryPorts(QList<QString>& devList);

    int readMessage(char* buffer, int length, mavlink_message_t &message);
    int writeMessage(const mavlink_message_t &message);

//signals:
//   void onMavLinkMessage(mavlink_message_t message);

public slots:
    //void onReadyRead();
    void onReadyRead(QByteArray buffer);
    void onBytesWritten(qint64 bytes);
    void UDP_onMavLinkMessage(mavlink_message_t message);
private:
    //int     __readBytes(char* buffer, const int length);
    int     __writeBytes(const char* buffer, const int length);

private:
    //QMutex  __dataMutex;       // Mutex for reading data from _port
    //QSerialPort __serialport;
    MySerialPort __serialport;
    QString     __portName;
    UDPLink* _udpLink;
    bool    isUDP;
    messagerecvFun m_serial_msgrecvFun;
};



#endif // SERIALLINK_H

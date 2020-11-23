#ifndef MYUDPLINK_H
#define MYUDPLINK_H

#include <QObject>
#include <QTimer>
#include <QFile>
#include <QUdpSocket>
#include "communicationinterface.h"


class LinkInterface;


class UDPLink :public CommunicationInterface
{
    Q_OBJECT
public:
    UDPLink();
    ~UDPLink();

    bool    linkOpen(QString portName,int BaudRate);
    bool    linkClose();

    int readMessage(char* buffer, int length, mavlink_message_t &message);
    int writeMessage(const mavlink_message_t &message);

public slots:
    //void onReadyRead();
    void onReadyRead(QByteArray buffer);
    void onBytesWritten(qint64 bytes);
private:
    //int     __readBytes(char* buffer, const int length);
    int     __writeBytes(const char* buffer, const int length);

private:
    QString     __portName;
    QUdpSocket* _socket;
};
#endif // MYUDPLINK_H

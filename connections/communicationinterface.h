#ifndef COMMUNICATIONINTERFACE_H
#define COMMUNICATIONINTERFACE_H

#include <QThread>

//#include <common/mavlink.h>
#include "connections/CommonStru.h"

class CommunicationInterface : public QObject
{
    Q_OBJECT
public:
    CommunicationInterface();
    ~CommunicationInterface();

public:


   virtual bool linkClose() = 0;
   virtual bool linkOpen(QString portName,int BaudRate) = 0;

   virtual void loadConfig() = 0;

   virtual int readMessage(char* buffer, int length, mavlink_message_t &message) = 0;
   virtual int writeMessage(const mavlink_message_t &message) = 0;

signals:
   void onMavLinkMessage(mavlink_message_t message);

};

#endif // COMMUNICATIONINTERFACE_H

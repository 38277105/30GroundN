#ifndef LINKMANAGER_H
#define LINKMANAGER_H

#include <QObject>
#include <QList>
#include "communicationinterface.h"
#include "seriallink.h"

typedef enum{
    SerialLinkType      = 0x1100,
    TCPLinkType         = 0x1101,
    BluetoothLinkType   = 0x1102
}LINKTYPE;

class LinkManager : public QObject
{
    Q_OBJECT
public:

static LinkManager* getInstance();
     ~LinkManager();
private:
     LinkManager();

public:
    CommunicationInterface* addLink(LINKTYPE linkType);

    void removeLink(CommunicationInterface* connection);

private:
    QList<CommunicationInterface*> __connections;

};

#endif // LINKMANAGER_H

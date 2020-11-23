#include "linkmanager.h"
#include <QDebug>


LinkManager::LinkManager()
{
}

LinkManager::~LinkManager()
{
}

LinkManager* LinkManager::getInstance(){
    static LinkManager lm;
    return &lm;
}

CommunicationInterface* LinkManager::addLink(LINKTYPE linkType){
    CommunicationInterface* ci = NULL;
    switch(linkType){
    case SerialLinkType:
        ci = new SerialLink();
        break;
    case TCPLinkType:
        break;
    case BluetoothLinkType:
        break;
    default:
        break;
    }

    __connections.append(ci);
    return ci;
}

void LinkManager::removeLink(CommunicationInterface* connection)
{
    for(QList<CommunicationInterface*>::iterator it = __connections.begin() ; it != __connections.end() ; ++it){
        if( (*it) == connection ){
            __connections.erase(it);
        }
    }
}


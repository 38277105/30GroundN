#include "serialportfinder.h"
#include "zygroundglobalconfig.h"
#include <QDebug>


SerialPortFinder::SerialPortFinder(QObject *parent) : QThread(parent)
{
    m_portCount=0;


}
SerialPortFinder::~SerialPortFinder()
{

}
void SerialPortFinder::portQuery()
{
	if (ZYGroundGlobalConfig::m_SerialIsOpen)  //串口已经打开就不要检测了
	{ 
		return;
	}
		

    m_ports.clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        if(info.isBusy())         //串口已经被占用了
            continue;
        m_ports[info.portName()]=info;
    }
    if(m_portCount!=m_ports.size())
    {
        m_portCount=m_ports.size();
        emit SerialPortChangedNotify(m_ports.values());
    }
}

void SerialPortFinder::run()
{
    m_QueryTimer=new QTimer();
    m_QueryTimer->setInterval(3000);
    m_QueryTimer->setSingleShot(false);
    connect(m_QueryTimer,SIGNAL(timeout()),this,SLOT(portQuery()));
    m_QueryTimer->start();
    this->exec();
}

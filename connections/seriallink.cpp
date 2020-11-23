#include "seriallink.h"
#include <QSerialPortInfo>
#include <QDebug>
#include "controllers/frmmaincontroller.h"
#include "ui_frmmain.h"
#include <QDateTime>
#include "../Utils/zygroundglobalconfig.h"
#include "UDP/UDPLink.h"

SerialLink::SerialLink()
{
    isUDP=false;
    _udpLink=NULL;
}
SerialLink::~SerialLink()
{
}

void SerialLink::SetCallBack(messagerecvFun fun)
{
    m_serial_msgrecvFun=fun;
}

bool SerialLink::linkOpen(QString portName,int BaudRate)
{    
    if(portName.compare("UDP")==0)
    {
        isUDP=true;
        UDPConfiguration* udpConfig = new UDPConfiguration("Default UDP Link");
        udpConfig->setLocalPort(QGC_UDP_LOCAL_PORT);
        udpConfig->setDynamic(true);
        _udpLink=new UDPLink(udpConfig);
        //_udpLink->SetCallBack(UDP_onMavLinkMessage);
        connect(_udpLink,SIGNAL(onMavLinkMessage(mavlink_message_t)),this,SLOT(UDP_onMavLinkMessage(mavlink_message_t)));
        //_udpLink->start(QThread::LowPriority);
        _udpLink->start(QThread::NormalPriority);
        return true;
    }
    else
    {
        if(_udpLink!=NULL)
            disconnect(_udpLink,SIGNAL(onMavLinkMessage(mavlink_message_t)),this,SLOT(UDP_onMavLinkMessage(mavlink_message_t)));

         isUDP=false;
//        __serialport.setPortName(portName);
//        __serialport.setBaudRate(BaudRate);
         bool ret = false;

        ret = __serialport.setCOM(portName,BaudRate);
         __serialport.clear();

        //loadConfig();
//       bool ret = false;
//        if( !( ret =__serialport.open(QIODevice::ReadWrite) )){
//            printf("open serial error : %s\n", __serialport.errorString());
//            QString ss = __serialport.errorString();
//            return false;
//        }else{
//            printf("the serial open ok : %s", __portName);
//        }
		 
//        connect(&__serialport, SIGNAL(readyRead()), this, SLOT(onReadyRead()), Qt::DirectConnection);
//        connect(&__serialport, SIGNAL(bytesWritten(qint64)), this, SLOT(onBytesWritten(qint64)),Qt::DirectConnection);

         connect(&__serialport, SIGNAL(sigDataReady(QByteArray)), this, SLOT(onReadyRead(QByteArray)), Qt::DirectConnection);

         return ret;
     }
}

bool SerialLink::linkClose()
{
    if(isUDP)
    {
        //_udpLink->terminate();
        _udpLink->_running = false;
        return true;
    }
    if(__serialport.isOpen() ){
        __serialport.close();
    }

    return true;
}

void SerialLink::run()
{
}

//int SerialLink::__readBytes(char* buffer, const int length){
//    //__dataMutex.lock();
//    int sz = __serialport.read(buffer, length);
//    //__dataMutex.unlock();
//    return sz;
//}

int SerialLink::__writeBytes(const char* buffer, const int length){
    // Lock
    //__dataMutex.lock();
    int sz = __serialport.write(buffer,length);
    // Unlock
   // __dataMutex.unlock();


    return sz;
}

void SerialLink::loadConfig()
{
//    //reading from xml

//    __serialport.setDataBits(QSerialPort::Data8);
//    __serialport.setStopBits(QSerialPort::OneStop);
//    __serialport.setParity(QSerialPort::NoParity);
//    __serialport.setFlowControl(QSerialPort::NoFlowControl);
//    __serialport.clearError();
//    __serialport.clear();
}

void SerialLink::queryPorts(QList<QString>& devlist)
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        //qDebug() << "Name : " << info.portName();
        //qDebug() << "Description : " << info.description();
       // qDebug() << "Manufacturer: " << info.manufacturer();
       // qDebug() << "Serial Number: " << info.serialNumber();
       // qDebug() << "System Location: " << info.systemLocation();
        devlist.append(info.portName());
    }
}

void SerialLink::onReadyRead(QByteArray buffer){
//    char buffer[1024];
//    memset( buffer,0,1024*sizeof(char) );
//    qint64 numBytes = __serialport.bytesAvailable();
//    if(numBytes>0)
//    {
//        int length = __readBytes(buffer,numBytes);

//        if(length>0)
//        {
            if(g_bWriteLog)
            {
               __writelog(true,buffer.data(),buffer.length());
            }
            mavlink_message_t message;
            //readMessage(buffer, length, message );
            readMessage(buffer.data(),buffer.length(),message);
//        }
//    }
}

int SerialLink::readMessage(char* buffer, int length, mavlink_message_t &message){

    mavlink_status_t status;
    uint8_t          msgReceived = false;
    if(length<=0)
        return msgReceived;
    for(int i = 0 ; i < length ; ++i){
        msgReceived = mavlink_parse_char(MAVLINK_COMM_1, (uint8_t)buffer[i], &message, &status);
        if(msgReceived){
            emit onMavLinkMessage(message);
        }
    }
    return msgReceived;
}

void SerialLink::UDP_onMavLinkMessage(mavlink_message_t message)
{
     emit onMavLinkMessage(message);//udp
}

int SerialLink::writeMessage(const mavlink_message_t &message){
    char buffer[300];
    //char* buffer=new char[300];
    memset(buffer,0,300);
    // Translate message to buffer
    unsigned int length = mavlink_msg_to_send_buffer((uint8_t*)buffer, &message);
    // Write buffer to serial port, locks port while writing
    int bytesWritten;
    if(isUDP)
    {

       _udpLink->writeBytes(buffer, length);
       bytesWritten = length;
    }
    else
    {
       bytesWritten = __writeBytes(buffer, length);
    }

    if(g_bWriteLog)
    {
      __writelog(false,buffer,bytesWritten);
    }
    //delete[] buffer;
    return bytesWritten;
}

void SerialLink::onBytesWritten(qint64 bytes){
    int sz = bytes;
    //qDebug("============write ok\n");
}





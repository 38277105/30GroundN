#include "zygroundglobalconfig.h"
#include <QFile>
#include <QDomDocument>
#include <QTextStream>
#include <QTextCodec>
#include <QDataStream>
#include <QString>
#include <QDebug>

bool    ZYGroundGlobalConfig::m_SerialIsOpen=false;
bool    ZYGroundGlobalConfig::setUavAsCenter=true;
bool    ZYGroundGlobalConfig::RESOLUTION_1920=false;

bool    ZYGroundGlobalConfig::m_bAlarmedEnabled=true;
bool    ZYGroundGlobalConfig::m_bRequestParamEnabled=true;
bool    ZYGroundGlobalConfig::m_bUsedNet206=false;
int     ZYGroundGlobalConfig::m_nNet206Port=10011;
QString ZYGroundGlobalConfig::m_sNet206IP="127.0.0.1";
QString ZYGroundGlobalConfig::m_sIconName="";
QString ZYGroundGlobalConfig::m_sStickPortName="COM2";
int     ZYGroundGlobalConfig::m_sStickBaudRate=115200;
bool    ZYGroundGlobalConfig::m_bUsedSlavePort=false;
QString ZYGroundGlobalConfig::m_sSlavePortName="COM3";
int     ZYGroundGlobalConfig::m_sSlaveBaudRate=115200;
QString ZYGroundGlobalConfig::m_sMainPortName="COM4";
int     ZYGroundGlobalConfig::m_sMainBaudRate=115200;
bool   g_bWriteLog=true;
bool   g_bDevelopMode=true;
QTimer*   g_FlushFileTimer;
QFile    g_fSend;
QFile    g_fRecv;
QDataStream g_sendDatastream;
QDataStream g_recvDataStream;
bool  g_bSendFlushed;
bool  g_bRecvFlushed;
QMutex g_mutex;
QString g_strTitle;
bool g_bSetESC;
//bool g_bcritActInfo;


void ZYGroundGlobalConfig::LoadConfig()
{
    QFile file("./data/zy_config.xml");
    if(!file.open(QFile::ReadOnly))
        return;

    QTextStream vReadStream(&file);
    QTextCodec *vCodec = QTextCodec::codecForName("GBK");
    vReadStream.setCodec(vCodec);
    QString vXmlDataStr = vReadStream.readAll();
    file.close();


    QDomDocument document;
    if(!document.setContent(vXmlDataStr.toLocal8Bit()))
        return;
    if(document.isNull())
        return;

    QDomElement root = document.documentElement();
    QDomElement tmpElem = root.firstChildElement("RequestParam");
    if(tmpElem.text()=="Enabled")
        m_bRequestParamEnabled=true;
    else
        m_bRequestParamEnabled=false;

    tmpElem = root.firstChildElement("SysAlarm");
    if(tmpElem.text()=="Enabled")
        m_bAlarmedEnabled=true;
    else
        m_bAlarmedEnabled=false;

    tmpElem = root.firstChildElement("Net206");
    if(tmpElem.isElement())
    {
        m_bUsedNet206=true;
        m_sNet206IP=tmpElem.attribute("IP",m_sNet206IP);
        m_nNet206Port=tmpElem.attribute("Port",QString::number(m_nNet206Port)).toInt();
        qDebug()<<"206 Net Enabled IP="<<m_sNet206IP<<" Port="<<m_nNet206Port;
    }
    else
        m_bUsedNet206=false;

    tmpElem = root.firstChildElement("WriteLog");
    if(tmpElem.text()=="Enabled")
        g_bWriteLog=true;
    else
        g_bWriteLog=false;

    tmpElem = root.firstChildElement("DevelopMode");
    if(tmpElem.text()=="Enabled")
        g_bDevelopMode=true;
    else
        g_bDevelopMode=false;

    tmpElem = root.firstChildElement("Title");
    g_strTitle = tmpElem.text();
    tmpElem = root.firstChildElement("Icon");
    m_sIconName = tmpElem.text();

    tmpElem = root.firstChildElement("JoyStick");
    if(tmpElem.isElement())
    {
        m_sStickPortName=tmpElem.attribute("Name","");
        m_sStickBaudRate=tmpElem.attribute("BaudRate",QString::number(m_sStickBaudRate)).toInt();
        qDebug()<<"JoyStick Enabled"<<m_sStickPortName;
    }

    tmpElem = root.firstChildElement("MainPort");
    if(tmpElem.isElement())
    {
        m_sMainPortName=tmpElem.attribute("Name",m_sMainPortName);
        m_sMainBaudRate=tmpElem.attribute("BaudRate",QString::number(m_sMainBaudRate)).toInt();
        qDebug()<<"MainPort Init"<<m_sMainPortName<<m_sMainBaudRate;
    }

    tmpElem = root.firstChildElement("SlavePort");
    if(tmpElem.isElement())
    {
        m_bUsedSlavePort=true;
        m_sSlavePortName=tmpElem.attribute("Name",m_sSlavePortName);
        m_sSlaveBaudRate=tmpElem.attribute("BaudRate",QString::number(m_sSlaveBaudRate)).toInt();
        qDebug()<<"SlavePort Enabled"<<m_sSlavePortName;
    }
    else
        m_bUsedSlavePort=false;
}


ZYGroundGlobalConfig::ZYGroundGlobalConfig(QObject *parent) : QObject(parent)
{

}


void __writelog(bool isReceived,char* buffer,qint32 length)
{
    if(isReceived)
    {
        if(g_fRecv.isOpen())
        {
            g_recvDataStream.writeRawData(buffer,length);
            if(g_bRecvFlushed)
            {
                g_fRecv.flush();
                g_bRecvFlushed=false;
            }
        }
        else
            qDebug()<<"Recv File Not Open";
    }
    else
    {
        if(g_fSend.isOpen())
        {
            g_sendDatastream.writeRawData(buffer,sizeof(buffer));
            if(g_bSendFlushed)
            {
                g_fSend.flush();
                g_bSendFlushed=false;
            }
        }
        else
            qDebug()<<"Send File Not Open";
    }
}


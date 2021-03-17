#include "zystick.h"
#include <QDir>
#include <QDomDocument>
#include <QTextCodec>
#include "../../vehicles/vehicle.h"
#include "../../controllers/frmmaincontroller.h"
#include <QDebug>
//#include "serialthread.h"

//CRC数据表
const quint8 aucCRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40
};
const quint8 aucCRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
    0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
    0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
    0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
    0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
    0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
    0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
    0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
    0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
    0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
    0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
    0x41, 0x81, 0x80, 0x40
};

ZYStick::ZYStick(QObject *parent) : QObject(parent),m_Ch5(1000,2000),m_Ch8(1000,2000),
    m_Thro(1000,2000),m_Yaw(1000,2000),m_Roll(1000,2000),m_Pitch(1000,2000)
{
    LoadParam();
    m_nNum=0;
    m_Ch5.cur=m_Ch5.max;
    m_Thro.cur=m_Thro.min;
    m_Ch8.cur=m_Ch8.max;
    m_bSending=false;
    m_bKeyRecv = false;

    connect(&m_tm,SIGNAL(timeout()),this,SLOT(onTmUpdate()));
}

ZYStick::~ZYStick()
{
    linkClose();
}

void ZYStick::LoadParam()
{
    QDir current;
    QString path=current.currentPath();
    QFile file("./data/joystick_config.xml");
    qDebug()<<path+"/data/joystick_config.xml"<<"------------------------";
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
    QDomElement settingItem = root.firstChildElement();
    QMap<QString,int> _AxisMaps;
    while(!settingItem.isNull())
    {
        _AxisMaps[settingItem.attribute("Key")]=settingItem.attribute("Value").toInt();
        settingItem=settingItem.nextSiblingElement();
    }
    if(_AxisMaps.contains("ROLL_MIN"))
    {
        m_Roll=CHItem(_AxisMaps["ROLL_MIN"],_AxisMaps["ROLL_MAX"]);
        qDebug()<<"Roll"<<m_Roll.range<<m_Roll.mid;
    }
    if(_AxisMaps.contains("PITCH_MIN"))
    {
        m_Pitch=CHItem(_AxisMaps["PITCH_MIN"],_AxisMaps["PITCH_MAX"]);
        qDebug()<<"Pitch"<<m_Pitch.range<<m_Pitch.mid;
    }
    if(_AxisMaps.contains("YAW_MIN"))
    {
        m_Yaw=CHItem(_AxisMaps["YAW_MIN"],_AxisMaps["YAW_MAX"]);
        qDebug()<<"Yaw"<<m_Yaw.range<<m_Yaw.mid;
    }
    if(_AxisMaps.contains("THRO_MIN"))
    {
        m_Thro=CHItem(_AxisMaps["THRO_MIN"],_AxisMaps["THRO_MAX"]);
        m_Thro.cur=m_Thro.min;
        qDebug()<<"Thro"<<m_Thro.range<<m_Thro.mid;
    }
    if(_AxisMaps.contains("MODE_MIN"))
    {
        m_Ch5=CHItem(_AxisMaps["MODE_MIN"],_AxisMaps["MODE_MAX"]);
        qDebug()<<"Mode"<<m_Ch5.range<<m_Ch5.mid;
        m_Ch8=m_Ch5;
    }
}

void ZYStick::setPort(QString portName, int BaudRate)
{
    __portName=portName;
    __portBaud=BaudRate;
}

bool ZYStick::linkOpen()
{
    m_nOKNum=0;
    m_nCRCNum=0;
    m_nNum=0;
    m_sPort = new SerialPort();
    m_sPort->init_port(__portName, __portBaud);
    connect(m_sPort,SIGNAL(receive_data(QByteArray)),this,SLOT(onReadyRead(QByteArray)),Qt::QueuedConnection);
    //m_Thread =new SerialThread(__portName, __portBaud);

    //connect(m_Thread,SIGNAL(senddata(QByteArray)),this,SLOT(onReadyRead(QByteArray)),Qt::QueuedConnection);

            //接收子线程传输数据的信号
    //m_Thread->setPriority(QThread :: HighestPriority);//开启子线程
    //m_Thread->run();
    m_tm.start(50);
    return true;
}

bool ZYStick::linkClose()
{
    m_bSending=false;
//    m_tm.stop();
    if(__serialport.isOpen())
        __serialport.close();
    return true;
}

bool ZYStick::isSending()
{
    return m_bSending;
}

void ZYStick::setSend(bool bSend)
{
    if(bSend && !__serialport.isOpen())
    {
        if(linkOpen())
            m_bSending=true;
        else
            m_bSending=false;
    }
    else
        m_bSending=bSend;
}
int ZYStick::__writeBytes(const char* buffer, const int length){
    // Lock
    //__dataMutex.lock();
    int sz = __serialport.write(buffer,length);
    // Unlock
   // __dataMutex.unlock();
    return sz;
}

void ZYStick::onReadyRead(QByteArray data)
{
    int nDropLen=0;
    __buf.append(data);
    StickPK* pHead=CheckHead(__buf,nDropLen);
    if(nDropLen>0)
        qDebug()<<m_nNum<<" Drop data="<<nDropLen;
    if(!pHead)
       return;
    ReadFrame(pHead);
    __buf.remove(0,SKPK_LEN);
}
void ZYStick::ReadFrame(StickPK *pHead)
{
 //   qDebug()<<"Stick Key:";
//    for(int i=0;i<16;i++)
 //       qDebug()<<pHead->nKeys[0]<<pHead->nKeys[1]<<pHead->nKeys[2]<<pHead->nKeys[3]<<pHead->nKeys[4]<<pHead->nKeys[7];

    m_Thro.cur=pHead->nKeys[2];
    m_Roll.cur=pHead->nKeys[0];
    m_Pitch.cur=pHead->nKeys[1];
    m_Yaw.cur=pHead->nKeys[3];
    m_Ch5.cur=pHead->nKeys[4];
    m_Ch8.cur=pHead->nKeys[7];
    m_bKeyRecv = true;
    m_debugValue = pHead->nKeys[2];
}

void ZYStick::onTmUpdate()
{
    if(!m_bSending)
        return;
    m_nNum++;
    if(m_nNum>=10)
    {
        m_nNum=0;
        //qDebug()<<"Recv OK="<<m_nOKNum<<"bad crc="<<m_nCRCNum;
        //qDebug()<<QTime::currentTime().msecsSinceStartOfDay()<<m_nOKNum<<m_Pitch.cur<<m_Roll.cur<<m_Thro.cur<<m_Yaw.cur<<m_Ch5.cur;
        if(m_Thro.cur==m_Thro.min)
        {
            if(m_Yaw.cur==m_Yaw.max)
                qDebug()<<QStringLiteral("解锁");
            else if(m_Yaw.cur==m_Yaw.min)
                qDebug()<<QStringLiteral("锁定");
        }
    }

    Vehicle* pVeh=FrmMainController::Instance()->__vehicle;
    if(!pVeh||!m_bKeyRecv)
        return;
    pVeh->mavLinkMessageInterface.sendRC_channels_override(m_Pitch.cur, m_Roll.cur, m_Thro.cur, m_Yaw.cur,m_Ch5.cur,m_Ch8.cur);
    m_bKeyRecv = false;
    //qDebug()<<"send to uav one pack";
}

//解析数据包包头
StickPK* ZYStick::CheckHead(QByteArray &dt,int& nDropLen)
{
    StickPK* pHead;
    nDropLen=0;
    while(dt.count()>=SKPK_LEN)
    {
        pHead=(StickPK*)(dt.data());
        if(pHead->nSTX!=SKPK_MASK) //对比标识
        {
            dt.remove(0,1);   //扔掉一个字节
            nDropLen++;
            continue;
        }
        else
        {
            if(!CheckSum(pHead))
            //if(!CheckCRC16(dt,pHead->nCrc))  //校验错误
            {
                dt.remove(0,1);
                nDropLen+=1;
                m_nCRCNum++;
                continue;
            }
            else
            {
                m_nOKNum++;
                return pHead;
            }
        }
    }
    return NULL;
}
//CRC16校验
bool ZYStick::CheckCRC16(QByteArray &dt,quint16 crc)
{
    quint8 ucCRCHi = 0xFF;
    quint8 ucCRCLo = 0xFF;
    quint8 tmpVal;
    int iIndex;
    for(int i=2;i<dt.count()-2;i++)
    {
        tmpVal=dt.at(i);
        iIndex = ucCRCLo ^ tmpVal;
        ucCRCLo = ( quint8 )( ucCRCHi ^ aucCRCHi[iIndex] );
        ucCRCHi = aucCRCLo[iIndex];
    }
    quint16 CrcReg=(quint16)(ucCRCHi << 8 | ucCRCLo);

    if(CrcReg==crc)
    {
        //qDebug()<<"CRC Check Success";
        return true;
    }
    else
    {
        //qDebug()<<"CRC Check Failed";
        return false;
    }

}

//CRC16校验
bool ZYStick::CheckSum(StickPK* pHead)
{
    //quint8 tmpData[] = {0x55, 00, 0x20, 0xA9 ,0xE5, 00, 0xDC, 05, 0xDC, 05,\
     //                    0xE8 ,03, 0xDC, 05, 0xDC, 05, 0xE8, 03, 0xE8, 03, 0xE8, 03, 0xDC, 05,\
    //                    0xDC, 05, 0xE8, 03, 0xE8 ,03, 0xE8 ,03, 0xE8 ,03, 0xE8, 03, 0xE8, 03, 0x77};
    //pHead =(StickPK*)tmpData;
    quint8* dt=(quint8*)pHead;
    int len=SKPK_LEN-1;
    //len= sizeof(tmpData)-1;
    quint8 sum=0;
    while (len > 0)
    {
        sum = sum + *dt++;
        len= len-1;
    }
    return sum == *dt;
}

SerialPort::SerialPort(QObject *parent) : QObject(parent)

{
    my_thread = new QThread();
    port = new QSerialPort();
}

SerialPort::~SerialPort()

{

    port->close();

    port->deleteLater();

    my_thread->quit();

    my_thread->wait();

    my_thread->deleteLater();

}



void SerialPort::init_port(QString portName,long portBaud)

{

    port->setPortName(portName);                   //串口名 windows下写作COM1

    port->setBaudRate(portBaud);                           //波特率

    port->setDataBits(QSerialPort::Data8);             //数据位

    port->setStopBits(QSerialPort::OneStop);           //停止位

    port->setParity(QSerialPort::NoParity);            //奇偶校验

    port->setFlowControl(QSerialPort::NoFlowControl);  //流控制

    if (port->open(QIODevice::ReadWrite))

    {

        qDebug() << "Port have been opened";

    }

    else

    {

        qDebug() << "open it failed";

    }

    connect(port, SIGNAL(readyRead()), this, SLOT(handle_data()), Qt::QueuedConnection); //Qt::DirectConnection



    this->moveToThread(my_thread);

    port->moveToThread(my_thread);

    my_thread->start();

}



void SerialPort::handle_data()

{

    QByteArray data = port->readAll();

    //qDebug() << QStringLiteral("data received(收到的数据):") << data;

    //qDebug() << "handing thread is:" << QThread::currentThreadId();

    emit receive_data(data);

}



void SerialPort::write_data()

{

    qDebug() << "write_id is:" << QThread::currentThreadId();

    port->write("data", 4);   //发送“data”字符

}

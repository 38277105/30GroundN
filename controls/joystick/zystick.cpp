#include "zystick.h"
#include <QDir>
#include <QDomDocument>
#include "../vehicles/vehicle.h"
#include "../../controllers/frmmaincontroller.h"
#include <QDebug>

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
    disconnect(&__serialport, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(&__serialport, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    LoadParam();
    m_nNum=0;
    m_bOpened=false;
    m_Ch5.cur=m_Ch5.max;
    m_Thro.cur=m_Thro.min;
    m_Ch8.cur=m_Ch8.max;
    m_fNuJu=-1;        //小于0不发送，0减小，100增大
    connect(&m_tm,SIGNAL(timeout()),this,SLOT(onTmUpdate()));
//    QByteArray tmp;
//    tmp.push_back(0xDD);
//    tmp.push_back(0x60);
//    for(int i=0;i<7;i++)
//        tmp.push_back((char)0);
//    tmp.push_back(0xEF);
//    for(int i=0;i<20;i++)
//        tmp.push_back((char)0);
//    tmp.push_back(0xA1);
//    tmp.push_back(0x7B);
//    StickPK* pHead=(StickPK*)tmp.data();
//    qDebug()<<pHead->nCrc;
//    CheckCRC16(tmp,pHead->nCrc);
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

bool ZYStick::linkOpen(QString portName, int BaudRate)
{
    if(__serialport.isOpen())
        __serialport.close();
    __serialport.setPortName(portName);
    if(__serialport.open(QIODevice::ReadWrite))
    {
        __serialport.setBaudRate(BaudRate);
        __serialport.setParity(QSerialPort::NoParity);
        __serialport.setDataBits(QSerialPort::Data8);
        __serialport.setStopBits(QSerialPort::OneStop);
        __serialport.setFlowControl(QSerialPort::NoFlowControl);
        __serialport.clearError();
        __serialport.clear();
        m_bOpened=true;
    }
    else
        return false;
    m_tm.start(50);
    return true;
}

bool ZYStick::linkClose()
{
    m_bOpened=false;
    m_tm.stop();
    if(__serialport.isOpen())
        __serialport.close();
    return true;
}

bool ZYStick::isOpened()
{
    return m_bOpened;
}
int ZYStick::__writeBytes(const char* buffer, const int length){
    // Lock
    //__dataMutex.lock();
    int sz = __serialport.write(buffer,length);
    // Unlock
   // __dataMutex.unlock();
    return sz;
}

void ZYStick::onReadyRead()
{
    if(!__serialport.isOpen())
        return;

    if(__serialport.bytesAvailable()<SKPK_LEN)
        return;
    int nDropLen=0;
    __buf.append(__serialport.read(SKPK_LEN));
    StickPK* pHead=CheckHead(__buf,nDropLen);
    if(nDropLen>0)
        qDebug()<<"Drop data="<<nDropLen;
    if(!pHead)
       return;
    ReadFrame(pHead);
    __buf.remove(0,SKPK_LEN);
}
void ZYStick::ReadFrame(StickPK *pHead)
{
    if(pHead->nKey5==0xC2)//右摇杆
    {
        m_Thro.cur=pHead->nRUpDown*m_Thro.range/240+m_Thro.mid;
        m_Roll.cur =pHead->nRLeftRight*m_Roll.range/240+m_Roll.mid;
    }
    if(pHead->nKey4==0xC1)//左摇杆
    {
        m_Pitch.cur=-pHead->nLUpDown*m_Pitch.range/240+m_Pitch.mid;
        m_Yaw.cur =pHead->nLLeftRight*m_Yaw.range/240+m_Yaw.mid;
    }
    if(pHead->nKey1==SK_BT_LUP || pHead->nKey2==SK_BT_LUP || pHead->nKey3==SK_BT_LUP)
        m_Ch5.cur=m_Ch5.min;  //增稳
    else if(pHead->nKey1==SK_BT_LDOWN || pHead->nKey2==SK_BT_LDOWN || pHead->nKey3==SK_BT_LDOWN)
        m_Ch5.cur=m_Ch5.max;  //定点
    else
        m_Ch5.cur=m_Ch5.mid;  //定高

    if(pHead->nKey1==SK_BT_RDOWN || pHead->nKey2==SK_BT_RDOWN || pHead->nKey3==SK_BT_RDOWN)
        m_Ch8.cur=m_Ch8.max;
    else
        m_Ch8.cur=m_Ch8.min;

//    if(pHead->nKey1==SK_THROTE_UP || pHead->nKey2==SK_THROTE_UP ||pHead->nKey3==SK_THROTE_UP)
//        m_fNuJu=100;                      //扭矩加
//    else if(pHead->nKey1==SK_THROTE_DOWN || pHead->nKey2==SK_THROTE_DOWN ||pHead->nKey3==SK_THROTE_UP)
//        m_fNuJu=0;                        //扭矩减
//    else
//        m_fNuJu=-1;                      //不发送
}

void ZYStick::onTmUpdate()
{
    m_nNum++;
    if(m_nNum>10)
    {
        m_nNum=0;
        qDebug()<<QStringLiteral("姿态")<<m_Pitch.cur<<m_Roll.cur<<m_Thro.cur<<m_Yaw.cur<<m_Ch5.cur;
        if(m_Thro.cur==m_Thro.min)
        {
            if(m_Yaw.cur==m_Yaw.max)
                qDebug()<<QStringLiteral("解锁");
            else if(m_Yaw.cur==m_Yaw.min)
                qDebug()<<QStringLiteral("锁定");
        }
    }

    Vehicle* pVeh=FrmMainController::Instance()->__vehicle;
    if(!pVeh)
        return;
    pVeh->mavLinkMessageInterface.sendRC_channels_override(m_Pitch.cur, m_Roll.cur, m_Thro.cur, m_Yaw.cur,m_Ch5.cur,m_Ch8.cur);
//    if(m_fNuJu!=-1)//发送扭矩控制
//    {
//        pVeh->mavLinkMessageInterface.doCommand((MAV_CMD)181,10,m_fNuJu,0,0,0,0,0);
//        qDebug()<<"Send NJ="<<m_fNuJu;
//    }
}

//解析数据包包头
StickPK* ZYStick::CheckHead(QByteArray &dt,int& nDropLen)
{
    StickPK* pHead;
    nDropLen=0;
    while(dt.count()>=SKPK_LEN)
    {
        pHead=(StickPK*)(dt.data());
        if(pHead->nMask!=SKPK_MASK) //对比标识
        {
            dt.remove(0,1);   //扔掉一个字节
            nDropLen++;
            continue;
        }
        else
        {
            if(!CheckCRC16(dt,pHead->nCrc))  //校验错误
            {
                dt.remove(0,2);
                nDropLen+=2;
                continue;
            }
            else
                return pHead;
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
        qDebug()<<"CRC Check Failed";
        return false;
    }

}

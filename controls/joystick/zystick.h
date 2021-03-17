#ifndef ZYSTICK_H
#define ZYSTICK_H

#include <QObject>
#include <QTimer>
#include <QSerialPort>
#include <QObject>
#include <QThread>
#include <QWaitCondition>
#include <QMutex>

enum
{
    SK_MENU_UP=0xA1,     //菜单上
    SK_MENU_DOWN,
    SK_MENU_LEFT,
    SK_MENU_RIGHT,       //菜单右
    SK_BT_LUP=0xA5,      //按键左上 拨杆左上
    SK_BT_RUP,          //按键右上 拨杆右上
    SK_BT_RDOWN,       //按键右后  拨杆右下
    SK_BT_LDOWN,       //按键左后  拨杆左下
    SK_THROTE_UP=0xB1,    //油门上
    SK_THROTE_DOWN,
    SK_STICK_LEFT=0xC1,   //左摇杆
    SK_STICK_RIGHT,
    SK_MENU_LEFT_D=0xD1,   //左按钮双击
    SK_MENU_RIGHT_D,
    SK_MENU_LAFTER_D,      //后左按钮双击
    SK_MENU_RAFTER_D
};
#pragma pack(push) //保存对齐状态
#pragma pack(1)//设定为1字节对齐
struct StickPK
{
    quint8  nSTX;
    quint8  reserve1;
    quint8  nDataLength;
    quint16  nSeq;
    quint8   nCmd;
    quint16  nKeys[16];
    quint8   nCheckSum;
};
#pragma pack(pop)//恢复对齐状态
#define SKPK_LEN   sizeof(StickPK)
#define SKPK_MASK  0x55
struct CHItem
{
    int min;
    int max;
    int mid;
    int range;
    int cur;
    CHItem(int l,int h)
    {
        min=l;
        max=h;
        range=h-l;
        mid=range/2+l;
        cur=mid;
    }
};


class SerialPort : public QObject

{

  Q_OBJECT

public:

  explicit SerialPort(QObject *parent = NULL);

  ~SerialPort();



  void init_port(QString portName,long portBaud);  //初始化串口
  //void startLoop();



public slots:

  void handle_data();  //处理接收到的数据

  void write_data();     //发送数据



signals:

  //接收数据

  void receive_data(QByteArray tmp);



private:

  QThread *my_thread;

  QSerialPort *port;

};

class ZYStick : public QObject
{
    Q_OBJECT
public:
    explicit ZYStick(QObject *parent = 0);
    ~ZYStick();
    void    setPort(QString portName,int BaudRate);
    bool    linkOpen();
    bool    linkClose();
    void    setSend(bool bSend);
    bool    isSending();
    float getValue(){return m_debugValue;};
public slots:
    void onReadyRead(QByteArray data);
    void onTmUpdate();
private:
    int      __writeBytes(const char* buffer, const int length);
    StickPK* CheckHead(QByteArray &dt,int& nDropLen);
    bool     CheckCRC16(QByteArray &dt,quint16 crc);
    bool     CheckSum(StickPK* pHead);
    void     ReadFrame(StickPK* pHead);
    void     LoadParam();
private:
    QSerialPort  __serialport;
    QString      __portName;
    int          __portBaud;
    QByteArray   __buf;
    float m_debugValue;
    SerialPort*  m_sPort;
    bool         m_bSending;
    int          m_nNum;
    int          m_nOKNum;
    int          m_nCRCNum;
    bool         m_bKeyRecv;
    CHItem       m_Thro;
    CHItem       m_Yaw;
    CHItem       m_Pitch;
    CHItem       m_Roll;
    CHItem       m_Ch5;
    CHItem       m_Ch8;
    QTimer       m_tm;
};

#endif // ZYSTICK_H

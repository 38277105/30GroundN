#ifndef ZYSTICK_H
#define ZYSTICK_H

#include <QObject>
#include <QTimer>
#include <QSerialPort>
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
struct StickPK
{
    quint16  nMask;
    quint8   nKey1;
    quint8   nKey2;
    quint8   nKey3;
    quint8   nKey4;
    quint8   nKey5;
    qint8    nLUpDown;
    qint8    nLLeftRight;
    quint8   nVolt;
    qint8    nRUpDown;
    qint8    nRLeftRight;
    qint32   nReserve1;
    qint32   nReserve2;
    qint16   nReserve3;
    quint8   nReserve4;
    quint8   nReserve5;
    quint8   nReserve6;
    quint8   nReserve7;
    quint8   nReserve8;
    quint8   nReserve9;
    quint16  nReserve10;
    quint16  nCrc;
};
#define SKPK_LEN   sizeof(StickPK)
#define SKPK_MASK  0x60DD
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
class ZYStick : public QObject
{
    Q_OBJECT
public:
    explicit ZYStick(QObject *parent = 0);
    ~ZYStick();
    bool    linkOpen(QString portName,int BaudRate);
    bool    linkClose();
    bool    isOpened();
public slots:
    void onReadyRead();
    void onTmUpdate();
private:
    int      __writeBytes(const char* buffer, const int length);
    StickPK* CheckHead(QByteArray &dt,int& nDropLen);
    bool     CheckCRC16(QByteArray &dt,quint16 crc);
    void     ReadFrame(StickPK* pHead);
    void     LoadParam();
private:
    QSerialPort  __serialport;
    QString      __portName;
    QByteArray   __buf;
    int          m_nNum;
    bool         m_bOpened;
    CHItem       m_Thro;
    CHItem       m_Yaw;
    CHItem       m_Pitch;
    CHItem       m_Roll;
    CHItem       m_Ch5;
    CHItem       m_Ch8;
    QTimer       m_tm;
    float        m_fNuJu; //小于0不发送保持现状，0减小，100增大
};

#endif // ZYSTICK_H

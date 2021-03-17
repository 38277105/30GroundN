#ifndef ZYGROUNDGLOBALCONFIG_H
#define ZYGROUNDGLOBALCONFIG_H

#include <QObject>
#include <QTimer>
#include <QFile>
#include <QMutex>

class ZYGroundGlobalConfig : public QObject
{
    Q_OBJECT
public:
    explicit ZYGroundGlobalConfig(QObject *parent = 0);
    static bool m_SerialIsOpen;

    static void LoadConfig();
    static void SubOutTime();
public:
    static double offset_lat;
    static double offset_lon;
    static bool   setUavAsCenter;
    static bool   RESOLUTION_1920;
    static bool    m_bRequestParamEnabled;
    static bool    m_bAlarmedEnabled;
    static bool    m_bUsedNet206;
    static int     m_nNet206Port;
    static QString m_sNet206IP;
    static QString m_sIconName;
    static bool    m_bUsedStick;
    static QString m_sStickPortName;
    static int     m_sStickBaudRate;
    static QString m_sMainPortName;
    static int     m_sMainBaudRate;
    static bool    m_bUsedSlavePort;
    static QString m_sSlavePortName;
    static int     m_sSlaveBaudRate;
    //旋翼剩余时间，电调剩余，电机剩余，电池剩余
   //旋翼剩余时长
    static int     m_RotorRemainTime;
   //电调剩余时长
    static int     m_EleDebugRemainTime;
   //电池剩余时长
    static int     m_BatteryRemainTime;
    //电机剩余时长
    static  int    m_MotorRemainTime;
    //偏航角
    static float m_YawAngle;
    //飞行时长
    static int m_flytime;

signals:

public slots:
};

extern bool   g_bWriteLog;
extern bool   g_bDevelopMode;

extern void  __writelog(bool isReceived,char* buffer,qint32 length);

extern  QTimer*   g_FlushFileTimer;

extern QFile    g_fSend;
extern QFile    g_fRecv;
extern QDataStream g_sendDatastream;
extern QDataStream g_recvDataStream;
extern bool  g_bSendFlushed;
extern bool  g_bRecvFlushed;
extern QMutex g_mutex;
extern QString g_strTitle;
extern bool g_bSetESC;
//extern bool g_bcritActInfo;
#endif // ZYGROUNDGLOBALCONFIG_H

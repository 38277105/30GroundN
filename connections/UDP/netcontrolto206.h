#ifndef NETCONTROLTO206_H
#define NETCONTROLTO206_H
#include <QThread>
#include <QUdpSocket>

struct NetPack206
{
    quint32 nMask;
    quint32 nCodeId;
    quint32 nLen;
    quint32 nFNum;
    quint32 nFCur;
    quint32 nFlag;
    qint32 nRoll;
    qint32 nPitch;
    qint32 nYaw;
    qint32 nRollRate;
    qint32 nPitchRate;
    qint32 nYawRate;
    quint32 nSpeed;
    qint32 nMP;
    quint32 nLon;
    quint32 nLat;
    quint32 nAlt;
    quint32 nChc;
    quint32 nEnd;
    NetPack206()
    {
        nMask=0x00007E7E;
        nCodeId=0x0109;
        nLen=76;
        nFNum=1;
        nFCur=1;
        nFlag=1;
        nMP=0;
        nEnd=0x00000A0D;
    }
    void SetCHC()
    {
        nChc=nMask+nCodeId+nLen+nFNum+nFCur+nFlag+nRoll+nPitch+nYaw+nRollRate+nPitchRate+nYawRate+
                nSpeed+nMP+nLon+nLat+nAlt;
    }
};
#define PACK_SIZE sizeof(NetPack206)

class MavLinkMessageInterface;
class NetControlTo206 : public QThread
{
    Q_OBJECT
public:
    explicit NetControlTo206(const QString& sIP,int nPort,MavLinkMessageInterface* pMavLink);
//    void start();
    void stop();
protected:
    void run();
private:
    bool          m_bExit;
    QString       m_sIP;
    qint16        m_nPort;
    QUdpSocket    m_nSocket;
    MavLinkMessageInterface*  m_pMavLink;
};

#endif // NETCONTROLTO206_H

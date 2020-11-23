#include "netcontrolto206.h"
#include <QtMath>
#include <QNetworkProxy>
#include "../mavlinkmessageinterface.h"
NetControlTo206::NetControlTo206(const QString &sIP, int nPort, MavLinkMessageInterface *pMavLink)
{
    m_bExit=false;
    m_pMavLink=pMavLink;
    m_sIP=sIP;;
    m_nPort=nPort;

    QHostAddress host = QHostAddress::AnyIPv4;
    m_nSocket.setProxy(QNetworkProxy::NoProxy);
    m_nSocket.bind(host, 6008, QAbstractSocket::ReuseAddressHint | QUdpSocket::ShareAddress);
    m_nSocket.setSocketOption(QAbstractSocket::SendBufferSizeSocketOption,  100 * 1024);
}

void NetControlTo206::stop()
{
    m_bExit=true;
    quit();
    if(!wait(500))
        exit();
    m_nSocket.close();
}

void NetControlTo206::run()
{
    if(m_pMavLink)
    {
        while(!m_bExit)
        {
            msleep(50);

            NetPack206 nData;
            float roll,pitch,yaw,rollrate,pitchrate,yawrate;
            float lon,lat,alt;
            float airSpeed,gSpeed,valt,climb;
            int16_t heading;
            uint16_t throttle;
            float tmpAP=18000/M_PI;
            m_pMavLink->getAttitude(roll,yaw,pitch,rollrate,yawrate,pitchrate);
            nData.nRoll=roll*tmpAP;
            nData.nPitch=pitch*tmpAP;
            nData.nYaw=yaw*tmpAP;
            nData.nRollRate=rollrate*tmpAP;
            nData.nPitchRate=pitchrate*tmpAP;
            nData.nYawRate=yawrate*tmpAP;
            m_pMavLink->getPosition(lat,lon,alt);
            m_pMavLink->getVfrHud(airSpeed,gSpeed,heading,throttle,valt,climb);
            nData.nSpeed=gSpeed*100;
            nData.nLon=lon*1.193;
            nData.nLat=lat*2.386;
            if(alt==0)
                nData.nAlt=0;
            else
                nData.nAlt=alt/100;
            nData.SetCHC();


            QHostAddress host(m_sIP);
//            #include <QtEndian>
//            uchar nBytes[PACK_SIZE];
//            qToBigEndian(nData,nBytes);
            int nLen=m_nSocket.writeDatagram((const char*)&nData, PACK_SIZE, host, m_nPort);
            //if(nLen!=PACK_SIZE)
                qDebug()<<"Wirte Dataram "<<m_sIP<<" port="<<m_nPort<<" sz="<<nLen;
        }
    }
}

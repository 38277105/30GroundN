#include "poweralarm.h"
#include "../controllers/soundthread.h"
#include <QDebug>
#include <QDateTime>
#include <QWidget>
#include "./Utils/zygroundglobalconfig.h"

PowerAlarm::PowerAlarm(QObject *parent) : QObject(parent)
{
    m_state = false;
    m_pSound = SoundThread::getInstance();
}

void PowerAlarm::setAlarmConfig(alarmConfig cfg, QImageLabel* pImageLabel)
{
    m_config = cfg;
    m_pImageLabel = pImageLabel;
}

void PowerAlarm::checkValue(const float &data)
{
    bool bState=false;
    if((m_config.minValue!=-1 && data<m_config.minValue) ||
           (m_config.maxValue != -1 && data >m_config.maxValue))
        bState=true;           //报警
    if(m_state == bState)
        return;

    QString strImagePath;
    qint32 fontsize;

    if(ZYGroundGlobalConfig::RESOLUTION_1920)
    {      
        fontsize = 15;
    }
    else
    {     

        fontsize = 12;
    }

    m_state=bState;
    if(bState)
    {
        if(m_config.isSound)
            m_pSound->start();
        strImagePath = ":/image/control/control_03.png";

    }
    else
    {
        if(m_config.isSound)
            m_pSound->stop();
        strImagePath = ":/image/control/control_01.png";
    }
    m_pImageLabel->setFontSize(fontsize);
    m_pImageLabel->setImage(strImagePath);

}




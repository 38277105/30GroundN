#include "poweralarm.h"
#include "../controllers/soundthread.h"
#include <QDebug>
#include <QDateTime>
#include <QWidget>
#include "./Utils/zygroundglobalconfig.h"

PowerAlarm::PowerAlarm(QObject *parent) : QObject(parent)
{
    m_state = Normal;
    m_pSound = SoundThread::getInstance();
}

void PowerAlarm::setAlarmConfig(alarmConfig cfg, QImageLabel* pImageLabel)
{
    m_config = cfg;
    m_pImageLabel = pImageLabel;
}

void PowerAlarm::checkValue(const float &data)
{
    if(-1 == m_config.minValue && -1 == m_config.maxValue)
        return;
    EState state=Normal;
    if((data<m_config.minValue-m_config.alarmOffset) ||
           (data >m_config.maxValue+m_config.alarmOffset))
        state = Alarm;           //报警
    else if(data<m_config.minValue-m_config.warnOffset || data>m_config.maxValue+m_config.warnOffset)
        state = Warn;
    if(m_state == state)
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

    m_state=state;
    QRgb color = qRgb(233,233,233);
    if(Alarm == state){
        if(m_config.isSound)
            m_pSound->start();
        strImagePath = ":/image/control/control_03.png";

    }
    else if(Warn == state){
        if(m_config.isSound)
            m_pSound->start();
        strImagePath = ":/image/control/control_41.png";
        color = qRgb(12,12,12);
    }
    else{
        if(m_config.isSound)
            m_pSound->stop();
        strImagePath = ":/image/control/control_01.png";
    }
    m_pImageLabel->setFontSize(fontsize);
    m_pImageLabel->setImage(strImagePath);
    m_pImageLabel->setTextColor(color);
}




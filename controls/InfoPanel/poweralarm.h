﻿#ifndef POWERALARM_H
#define POWERALARM_H

#include <QObject>
#include <QTimer>
#include "./Utils/qimagelabel.h"
class SoundThread;
typedef struct _alarmConfig
{
    float minValue;     //最小域值
    float maxValue;     //最大域值
    bool  isSound;      //是否开启声音警报
    _alarmConfig(){
       minValue = -1;
       maxValue = -1;
       isSound = false;
    }
    _alarmConfig& operator =(_alarmConfig& cfg)
    {
        this->minValue = cfg.minValue;
        this->maxValue = cfg.maxValue;
        this->isSound = cfg.isSound;
        return *this;
    }
} alarmConfig;
class PowerAlarm : public QObject
{
    Q_OBJECT
public:
    explicit PowerAlarm(QObject *parent = 0);
    void setAlarmConfig(alarmConfig cfg,QImageLabel* pImageLabel);
    void checkValue(const float& data);

private:
    SoundThread*  m_pSound;
    alarmConfig   m_config;
    QImageLabel*  m_pImageLabel;
    bool          m_state; //当前检测数据的状态，未警报为假，警报中为真
};

#endif // POWERALARM_H

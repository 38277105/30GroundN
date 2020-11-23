#ifndef VOICEHELPER_H
#define VOICEHELPER_H

#include <QObject>
#include "QGCAudioWorker.h"

class VoiceHelper : public QObject
{
    Q_OBJECT
public:

     ~VoiceHelper();
public:
     void speak(QString content);
     static  VoiceHelper* m_Instance();
     QGCAudioWorker* m_worker;
private:
     VoiceHelper();
};

#endif // VOICEHELPER_H

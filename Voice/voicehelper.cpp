#include "voicehelper.h"

VoiceHelper* m_VoiceHelperInstance=NULL;

VoiceHelper::VoiceHelper()
{
    m_worker=new QGCAudioWorker();
}
VoiceHelper* VoiceHelper::m_Instance()
{
    if(m_VoiceHelperInstance==NULL)
        m_VoiceHelperInstance=new VoiceHelper();
    return m_VoiceHelperInstance;

}

VoiceHelper::~VoiceHelper()
{
    delete m_worker;
}

void VoiceHelper::speak(QString content)
{
    m_worker->say(content);
}

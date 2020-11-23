#include "soundthread.h"
#include <QApplication>
#include <QDebug>
SoundThread* SoundThread::instance = NULL;
int SoundThread::playerNum = 0;
SoundThread::SoundThread(QObject* parent) : QThread(parent)
{

    m_sound=new QSound(":/voices/image/music/6709.wav",this);
    m_sound->setLoops(-1);
}

SoundThread* SoundThread::getInstance(){
    if(NULL == instance){
        instance = new SoundThread();
    }
    return instance;
}

void SoundThread::start(){
    if(playerNum==0){
        QThread::start();
    }
    playerNum++;
    qDebug() << "start playerNum: " << playerNum;
}

void SoundThread::run(void)
{
    m_sound->play();
}
 void SoundThread::stop()
 {
    playerNum--;
    qDebug() << "stop playerNum: " << playerNum;
    if(playerNum < 1){
       m_sound->stop();
    }

 }

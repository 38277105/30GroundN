#ifndef SOUNDTHREAD_H
#define SOUNDTHREAD_H

#include <QThread>
#include <QSound>
#include <QSoundEffect>

class SoundThread : public QThread
{
    Q_OBJECT
private:
    SoundThread(QObject* parent=0);
public:
    static SoundThread* getInstance();
    void start();
    void run(void);
    void stop();

signals:

public slots:
private:
   QSound* m_sound;
   static int playerNum;
   static SoundThread* instance;
};

#endif // SOUNDTHREAD_H

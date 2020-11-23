#ifndef RUNNINGLOG_H
#define RUNNINGLOG_H

#include <QObject>
#include <QFile>
#include "connections/CommonStru.h"
#define RUNNINGLOG_DIR "./data/runninglog/"
class runningLog : public QObject
{
    Q_OBJECT
public:
    explicit runningLog(QObject *parent = 0);
    void start();
    int parserunningLogFile();
    void flushfilebuffer();
signals:
    void runninglogMessage_siganl(mavlink_message_t message);
public slots:
    void messLogData(mavlink_message_t message);
private:
    void initDir();
    void printMessage(mavlink_message_t message);
private:
    FILE*     m_file;
    int       m_lines;
};

#endif // RUNNINGLOG_H

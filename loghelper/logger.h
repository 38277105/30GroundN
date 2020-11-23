#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
//#include <common/mavlink.h>
#include "connections/CommonStru.h"
class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = 0);
    ~Logger();
     void LogE(QString content);
     void LogN(QString content);
     void LogInfo(QString content);
     void LogC(QString content);
     void LogStruct(mavlink_message_t msg);

   static  Logger* Instance();

signals:

public slots:

private:
      FILE *m_fp;
      char fileName[128];
};

#endif // LOGGER_H

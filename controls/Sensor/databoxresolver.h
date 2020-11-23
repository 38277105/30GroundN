#ifndef DATABOXRESOLVER_H
#define DATABOXRESOLVER_H

#include <QObject>
#include <QtSerialPort/qserialport.h>
#include <QtSerialPort/qserialportinfo.h>
#include "SensorStruct.h"
#include  <QMutex>
#include <QThread>

class DataBoxResolver : public QThread
{
    Q_OBJECT
public:
    explicit DataBoxResolver(QObject *parent = 0);

signals:
        void datacoming(AIRSENSORSTR& airSensorStr, UAVDATASTR& uavDataStr);
        void signal_WriteBytes(char* buffer, size_t length);
public slots:
        void readBytes();
        void WriteBytes(char* buffer, size_t length);
public:
        void openserial(QString comName);
        void sendDateTime();
        int  parseAirDetectData2Str(char* buffer, size_t length,AIRSENSORSTR& airSensorStr, UAVDATASTR& uavDataStr);
        void queryports(QList<QString>& devlist);
        void SaveFile(QList<AIRSENSORSTR>& airSensorStr, QList<UAVDATASTR>& uavDataStr);
        void stop();
        void run();
        void setPortName(QString comName,int PortNum);

private:
       QSerialPort* _serialport;
       QMutex  _dataMutex;       // Mutex for reading data from _port
       bool  _readtag;
       char* _dirtyData;
       QString _portName;
       int _portNum;
       FILE *fp_uav;

       char data[128];
       int offset;
};

#endif // DATABOXRESOLVER_H

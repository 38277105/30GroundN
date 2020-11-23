#ifndef NMEAINTERFACE_H
#define NMEAINTERFACE_H

#include <QObject>
#include <QSerialPort>

#include <queue>
#include <algorithm>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <stdint.h>
using namespace std;

struct HEADINGA{
    string header;
    string solStat;
    string posType;
    string length;
    string heading;
    string pitch;
    string reserved1;
    string hdg;
    string ptch;
    string stn;
    string svs;
    string solnSvs;
    string obs;
    string multi;
    string reserved2;
    string extSolStat;
    string reserved3;
    string sigMask;
    string crc;
};

class nmeaInterface:public QObject
{
    Q_OBJECT
public:
    nmeaInterface();

public:
    //打开nmea传输
    bool openNmea(char* port);
    //关闭nmea传输
    bool closeNmea();

public slots:
    void onReadyRead();
private:
    int __readNmea(char* buffer, int length);
    int  __parseNmea(string buffer);
    bool __paraseHeadinga(string buffer, HEADINGA& headinga);
    bool __sendHeading2Vehicle(HEADINGA headinga);
private:
    QSerialPort __serialport;
    stringstream __streamBuffer;

signals:
    void paramSet(float paramValue, char* paramId, uint8_t paramType);
};

#endif // NMEAINTERFACE_H

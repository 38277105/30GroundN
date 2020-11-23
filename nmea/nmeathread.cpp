#include "nmeathread.h"

nmeaThread::nmeaThread()
{

}

void nmeaThread::run(){
    nmeaInterface nmea;
    nmea.openNmea((char*)"COM1");
    __vehicle->connectSlots(&nmea);
    exec();
}

void nmeaThread::switchVehicle(Vehicle* v){
    __vehicle = v;
}

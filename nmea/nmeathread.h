#ifndef NMEATHREAD_H
#define NMEATHREAD_H
#include <QThread>
#include "nmeainterface.h"
#include "vehicles/vehicle.h"
class nmeaThread : public QThread
{
public:
    nmeaThread();

    void switchVehicle(Vehicle* v);
protected:
    void run();
private:
    Vehicle* __vehicle;
};

#endif // NMEATHREAD_H

#ifndef VEHICLEMANAGER_H
#define VEHICLEMANAGER_H

#include <QObject>
#include "mavlinkparser.h"
#include "speciallinkparser.h"
//#include <common/mavlink.h>
#include "connections/CommonStru.h"
#include "connections/communicationinterface.h"
class Vehicle;

class VehicleManager : public QObject
{
    Q_OBJECT
public:
    static VehicleManager* getInstance();
    ~VehicleManager();
private:
    explicit VehicleManager(QObject *parent = 0);

public:
    Vehicle* addVehicle(QObject* handle);
    void     removeVehicle(Vehicle* vehicle);
    void     removeAllVehicles();
    QList<Vehicle*> getVehicleList();
    Vehicle* getActiveVehicle();
    int getMaxId();

public slots:
    void activeById(int vehichleId);



private:
      QList<Vehicle*> __Vehicles;
};

#endif // VEHICLEMANAGER_H

#include "vehiclemanager.h"
#include "vehicle.h"

VehicleManager::VehicleManager(QObject *parent) : QObject(parent)
{
    __Vehicles.clear();
}

VehicleManager::~VehicleManager()
{
}

VehicleManager* VehicleManager::getInstance(){
    static VehicleManager vm;
    return &vm;
}

void VehicleManager::activeById(int vehichleId)
{
    for(int i=0;i<__Vehicles.count();i++)
    {
        if(__Vehicles.at(i)->m_State.m_Id==vehichleId)
        {
            __Vehicles.at(i)->m_IsActive=true;
        }
        else
        {
             __Vehicles.at(i)->m_IsActive=false;
        }

    }
}

Vehicle* VehicleManager::addVehicle(QObject* handle){
    Vehicle* vehicle = new Vehicle(handle);

    __Vehicles.size();

    for(int i=0;i<__Vehicles.size();i++)
    {
         __Vehicles.at(i)->m_IsActive=false;
    }
    vehicle->m_IsActive=true;             /******激活当前无人机对象*******/

    vehicle->m_State.m_Id=getMaxId()+1;
    __Vehicles.append(vehicle);
    return vehicle;
}

void VehicleManager::removeVehicle(Vehicle* vehicle){
    for(QList<Vehicle*>::iterator it = __Vehicles.begin() ; it != __Vehicles.end() ; ++it){
        if( (*it) == vehicle ){
            __Vehicles.erase(it);
            if( NULL != (*it) ){
                delete (*it);
                (*it) = NULL;
            }
            return;
        }
    }
}

void VehicleManager::removeAllVehicles(){

}

QList<Vehicle*> VehicleManager::getVehicleList(){
    return __Vehicles;
}

Vehicle* VehicleManager::getActiveVehicle()
{
    for(int i=0;i<__Vehicles.count();i++)
    {
        if(__Vehicles.at(i)->m_IsActive==true)
        {
           return __Vehicles.at(i);
        }

    }
}

int VehicleManager::getMaxId()
{
   int tempId=0;
   for(int i=0;i<__Vehicles.count();i++)
   {
       int t_id=__Vehicles.at(i)->m_State.m_Id;
       if(tempId<t_id)
           tempId=t_id;
   }
   return tempId;
}

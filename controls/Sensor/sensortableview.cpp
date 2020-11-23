#include "sensortableview.h"
#include "QStandardItemModel"
#include "QHeaderView"
#include "sensordatamodel.h"

SensorTableView::SensorTableView(QWidget *parent) : QTableView(parent)
{
    initLayout();
    setData();
}
void SensorTableView::initLayout()
{

    _sensorModel=new SensorDataModel(this);

    this->setModel(_sensorModel);

    for(int i=0;i<12;i++)
    {

    this->setColumnWidth(i,88);

    }
    this->setColumnWidth(13,150);
    this->setColumnWidth(14,120);
}
void SensorTableView::setData()
{
//    QList<UAVDdataStruct> m_datalist;
//    QList<AirSensorStruct> m_Airdatalist;
//    for(int i=0;i<10;i++)
//    {
//        UAVDdataStruct tempdata;
//        tempdata.year=2016;
//        tempdata.mon=5;
//        tempdata.day=4;
//        tempdata.hour=10;
//        tempdata.min=53;
//        tempdata.sec=24;
//        tempdata.latitude=35.4;
//        tempdata.longitude=125;
//        tempdata.hight=1001;
//        m_datalist.append(tempdata);

//        AirSensorStruct airobj;
//        airobj.CO=1;
//        airobj.humidity=1;
//        airobj.NO2=1;
//        airobj.O3=1;
//        airobj.pm1_0=1;
//        airobj.pm2_5=1;
//        airobj.pm10=1;
//        airobj.SO2=1;
//        airobj.temperature=1;
//        m_Airdatalist.append(airobj);
//    }
//    _sensorModel->setData(m_datalist,m_Airdatalist);
}


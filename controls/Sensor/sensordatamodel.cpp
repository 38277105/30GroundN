#include "sensordatamodel.h"

SensorDataModel::SensorDataModel(QObject *parent) : QAbstractTableModel(parent)
{

}

void SensorDataModel::setData(const QList<UAVDdataStruct> &datalist,const QList<AirSensorStruct> &sensorlist)
{
    _dataArray = datalist;
    _airdataArray=sensorlist;
    //reset();
}
int SensorDataModel::rowCount(const QModelIndex &parent) const{
 return _dataArray.count();
}
int SensorDataModel::columnCount(const QModelIndex &parent) const{
 return 14;
}
QVariant SensorDataModel::data(const QModelIndex &index, int role) const{

    if (!index.isValid())
                   return QVariant();

           if (role == Qt::TextAlignmentRole) {
                   return int(Qt::AlignRight | Qt::AlignVCenter);
           } else if (role == Qt::DisplayRole) {
                   UAVDdataStruct dataobj = _dataArray[index.row()];
                   AirSensorStruct airobj=_airdataArray[index.row()];
                   switch (index.column()) {
                   case 0:
                   {
                       QString date_str="";
                        return date_str.sprintf("%d/%02d/%02d",dataobj.year,dataobj.mon,dataobj.day);
                       break;
                   }
                   case 1:
                   {
                       QString time_str="";
                       return time_str.sprintf("%02d:%02d:%02d",dataobj.hour,dataobj.min, dataobj.sec);
                       break;
                   }
                   case 2:
                        return dataobj.longitude;
                       break;
                   case 3:
                        return dataobj.latitude;
                       break;
                   case 4:
                        return dataobj.hight;
                       break;

                        //sensor
                    case 5:
                       return airobj.O3;
                       break;
                   case 6:
                       return airobj.SO2;
                       break;
                   case 7:
                       return airobj.CO;
                       break;
                   case 8:
                       return airobj.NO2;
                       break;
                   case 9:
                   return airobj.pm1_0;
                       break;
                   case 10:
                   return airobj.pm2_5;
                       break;
                   case 11:
                   return airobj.pm10;
                       break;
                   case 12:
                   return airobj.temperature;
                       break;
                   case 13:
                   return airobj.humidity;
                       break;
                   default:
                        return QVariant();
                       break;
                   }

           }
           return QVariant();

}
QVariant SensorDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role==Qt::DisplayRole)
       {
           if(orientation==Qt::Horizontal) // 水平表头
           {
               if(14 > section)

                   switch (section) {
                   case 0:
                        return "Date";
                       break;
                   case 1:
                        return "Time";
                       break;
                   case 2:
                        return "Lontitude";
                       break;
                   case 3:
                        return "Latitude";
                       break;
                   case 4:
                        return "Height";
                       break;
                   case 5:
                      return "O3";
                      break;
                  case 6:
                      return "SO2";
                      break;
                  case 7:
                      return "CO";
                      break;
                  case 8:
                      return "NO2";
                      break;
                  case 9:
                  return "PM1.0";
                      break;
                  case 10:
                  return "PM2.5";
                      break;
                  case 11:
                   return "PM10";
                      break;
                  case 12:
                  return "Temperature";
                      break;
                  case 13:
                  return "Humidity";
                      break;
                   default:
                        return QVariant();
                       break;
                   }
               else
                   return QVariant();
           }
           else
           {
            //竖直列头
          return QVariant();
           }
       }

       return QVariant();
}

void SensorDataModel::refresh()
{
    beginResetModel();
    endResetModel();
   // emit updateCount(this->rowCount(QModelIndex()));
}

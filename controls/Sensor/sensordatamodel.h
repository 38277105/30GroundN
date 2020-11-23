#ifndef SENSORDATAMODEL_H
#define SENSORDATAMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include "SensorStruct.h"

class SensorDataModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SensorDataModel(QObject *parent = 0);
    void setData(const QList<UAVDdataStruct> &datalist,const QList<AirSensorStruct> &sensorlist);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
signals:

public slots:
public:
    QString DataAt(int offset) const;
    QList<UAVDdataStruct> _dataArray;
    QList<AirSensorStruct> _airdataArray;
    void refresh();
};

#endif // SENSORDATAMODEL_H

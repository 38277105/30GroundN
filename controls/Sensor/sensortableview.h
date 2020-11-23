#ifndef SENSORTABLEVIEW_H
#define SENSORTABLEVIEW_H

#include <QObject>
#include <QTableView>

class QStandardItemModel;
class SensorDataModel;

class SensorTableView : public QTableView
{
    Q_OBJECT
public:
    explicit SensorTableView(QWidget *parent = 0);
    void initLayout();
    void setData();
signals:

public slots:
public:
     QStandardItemModel* _model;
     SensorDataModel* _sensorModel;
};

#endif // SENSORTABLEVIEW_H

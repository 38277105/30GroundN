#ifndef SHOWLOGTABLE_H
#define SHOWLOGTABLE_H

#include <QStandardItemModel>
#include <QTableView>
#include "vehicles/vehiclestate.h"
class showlogtable : public QTableView
{
    Q_OBJECT

public:
    explicit showlogtable(QTableView* parent = 0);
    ~showlogtable();
    int setTableData(QList<LogData> datalist);
private:
    void initTable();
private:
    QStandardItemModel* m_dataModel;
};

#endif // SHOWLOGTABLE_H

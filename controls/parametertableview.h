#ifndef PARAMETERTABLEVIEW_H
#define PARAMETERTABLEVIEW_H

#include <QObject>
#include <QTableView>
#include "connections/datastruct.h"
#include "connections/communicationinterface.h"
#include <QDesktopWidget>

class QStandardItemModel;
class ParameterModel;

class ParameterTableView : public QTableView
{
    Q_OBJECT
public:
    explicit ParameterTableView(QWidget *parent = 0);
    void init();
    void setList(QList<ParamData>* datalist);
    void getList(QList<ParamData>& datalist);

signals:
    void set_parameter(char* param_id,float param_value,MAV_PARAM_TYPE para_type);
public slots:
   void sync_args_msgcomes(QList<ParamData>* datalist);
   void btn_set_click(const QModelIndex& index);
public:
     QStandardItemModel* dataModel;
     QList<ParamData>* m_datalist;
};

#endif // SENSORTABLEVIEW_H

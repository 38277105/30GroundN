#include "parametertableview.h"
#include "QStandardItemModel"
#include "QHeaderView"
#include "inputdelegate.h"
#include "buttondelegate.h"
#include <QMessageBox>
#include <connections/communicationinterface.h>
#include <settingcontrols/settingmanager.h>
#include <QDebug>

ParameterTableView::ParameterTableView(QWidget *parent) : QTableView(parent)
{
    init();
    m_datalist=NULL;
}

void ParameterTableView::init()
{

    dataModel=new QStandardItemModel(this);

    dataModel = new QStandardItemModel();

    dataModel->setColumnCount(5);

    dataModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("参数名"));

    dataModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("参数值"));

    dataModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("类型"));

    dataModel->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("设置"));

    dataModel->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("描述"));




    this->setColumnWidth(0,300);

    this->setColumnWidth(1,150);

    this->setColumnWidth(2,150);

    this->setColumnWidth(3,100);

    this->setColumnWidth(4,this->width()-750);


    this->setModel(dataModel);

    InputDelegate* editDelegate=new InputDelegate();

    this->setItemDelegateForColumn(1,editDelegate);

    ButtonDelegate* buttonDelegate=new ButtonDelegate();

    this->setItemDelegateForColumn(3,buttonDelegate);

    connect(buttonDelegate,SIGNAL(processMsg(const QModelIndex&)),this,SLOT(btn_set_click(const QModelIndex&)));

}

void ParameterTableView::setList(QList<ParamData>* datalist)
{
    if(!datalist)
        return;
    m_datalist=datalist;

    dataModel->clear();

    dataModel->setColumnCount(5);

    dataModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("参数名"));

    dataModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("参数值"));

    dataModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("类型"));

    dataModel->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("设置"));

    dataModel->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("描述"));




    this->setColumnWidth(0,300);

    this->setColumnWidth(1,150);

    this->setColumnWidth(2,150);

    this->setColumnWidth(3,100);

    this->setColumnWidth(4,this->width()-750);


    for(int i = 0; i < datalist->count(); i++)
     {
         dataModel->setItem(i,0,new QStandardItem(datalist->at(i).ZhName));
            //设置字符位置
         dataModel->item(i,0)->setTextAlignment(Qt::AlignLeft);

         dataModel->setItem(i,1,new QStandardItem(datalist->at(i).value));

        QString tempType=SettingManager::Instance()->m_types[datalist->at(i).type];
         dataModel->setItem(i,2,new QStandardItem(tempType));

         dataModel->setItem(i,3,new QStandardItem(QString::fromLocal8Bit("设置")));

         dataModel->setItem(i,4,new QStandardItem(datalist->at(i).desc));


     }
    this->reset();
}

void ParameterTableView::getList(QList<ParamData>& templst)
{
        int r=  dataModel->rowCount();
        int c= dataModel->columnCount();
        for(int i=0;i<r;i++)
        {
            QString argName=  dataModel->item(i,0)->text();
            QString argValue=  dataModel->item(i,1)->text();
            QString argType= m_datalist->at(i).type;
            QString descStr=  dataModel->item(i,3)->text();

            ParamData t_data;
            t_data.key=argName.toUtf8().data();
            t_data.value=argValue.toUtf8().data();
            t_data.type=atoi(argType.toUtf8().data());
            t_data.desc=descStr.toUtf8().data();

            templst.append(t_data);
        }

}

void ParameterTableView::sync_args_msgcomes(QList<ParamData>* datalist)
{

   SettingManager::Instance()->SaveSetting(datalist,"Data/parameters.xml");

   QList<ParamData>* displayParams=SettingManager::Instance()->LoadPartSettings("Data/DisplayList.xml","Data/parameters.xml");

   this->setList(displayParams);
}

void ParameterTableView::btn_set_click(const QModelIndex& index)
{
    int row=index.row();
   // QString argZhName=dataModel->item(row,0)->text();
    QString argName= m_datalist->at(row).key;
    QString argValue= dataModel->item(row,1)->text();
    QString argType=  dataModel->item(row,2)->text();


      QMap<int,QString>::iterator _it=  SettingManager::Instance()->m_types.begin();
      int key=-1;
    for(;_it!=SettingManager::Instance()->m_types.end();_it++)
    {
         if(_it.value()==argType)
             key=_it.key();
    }
    if(key==-1)
    {
        QMessageBox msg(QMessageBox::Warning,QString::fromLocal8Bit("警告"),QString::fromLocal8Bit("参数类型转换失败，请检查参数类型！"));
        msg.show();
        QThread::sleep(2);
        return;
    }
    //QString descStr=  dataModel->item(row,2)->text();
    //QString argType=  m_datalist->at(row).type;
    if(m_datalist->at(row).type == MAV_PARAM_TYPE_REAL32){
        emit set_parameter(argName.toLocal8Bit().data(),argValue.toFloat(),(MAV_PARAM_TYPE)key);
    }else if(m_datalist->at(row).type == MAV_PARAM_TYPE_INT32){
        int i = argValue.toInt();
        float f;
        memcpy(&f, (void*)&i, sizeof(int));
        emit set_parameter(argName.toLocal8Bit().data(),f,(MAV_PARAM_TYPE)key);
    }
}

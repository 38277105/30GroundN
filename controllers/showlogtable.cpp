#include "showlogtable.h"
//#include "ui_showlogtable.h"

showlogtable::showlogtable(QTableView * parent):QTableView (parent)
{
    initTable();
}

showlogtable::~showlogtable()
{
}

int showlogtable::setTableData(QList<LogData> datalist){
    m_dataModel->clear();

    m_dataModel->setColumnCount(6);
    m_dataModel->setHeaderData(0,Qt::Horizontal,"index");
    m_dataModel->setHeaderData(1,Qt::Horizontal,"size");
    m_dataModel->setHeaderData(2,Qt::Horizontal,"type");
    m_dataModel->setHeaderData(3,Qt::Horizontal,"count");
    m_dataModel->setHeaderData(4,Qt::Horizontal,"time");
    m_dataModel->setHeaderData(5,Qt::Horizontal,"msg");

    for(int i = 0; i < datalist.count(); i++){
         m_dataModel->setItem(i,0,new QStandardItem(QString::number(datalist.at(i).index)));
         m_dataModel->setItem(i,1,new QStandardItem(QString::number(datalist.at(i).size)));
         m_dataModel->setItem(i,2,new QStandardItem(QString::number(datalist.at(i).type)));
         m_dataModel->setItem(i,3,new QStandardItem(QString::number(datalist.at(i).count)));
         m_dataModel->setItem(i,4,new QStandardItem(QString::number(datalist.at(i).time_stamp)));
         m_dataModel->setItem(i,5,new QStandardItem(QString::fromLocal8Bit((char*)datalist.at(i).msg, datalist.at(i).count)));
     }
    this->setModel(m_dataModel);
    this->resizeColumnsToContents();
    return true;
}

void showlogtable::initTable(){
    this->setFixedSize(500, 400);
    m_dataModel = new QStandardItemModel();
    m_dataModel->setColumnCount(6);
    m_dataModel->setHeaderData(0,Qt::Horizontal,"index");
    m_dataModel->setHeaderData(1,Qt::Horizontal,"size");
    m_dataModel->setHeaderData(2,Qt::Horizontal,"type");
    m_dataModel->setHeaderData(3,Qt::Horizontal,"count");
    m_dataModel->setHeaderData(4,Qt::Horizontal,"time");
    m_dataModel->setHeaderData(5,Qt::Horizontal,"msg");

    this->setModel(m_dataModel);
}

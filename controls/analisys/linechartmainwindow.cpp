#include "linechartmainwindow.h"
#include "linechart/Linecharts.h"
#include <QThread>
#include <vehicles/vehiclemanager.h>

LineChartMainWindow::LineChartMainWindow(QWidget *parent) : QMainWindow(parent)
{

  Vehicle*  m_vehicle=  VehicleManager::getInstance()->getActiveVehicle();
    m_decoder = new MAVLinkDecoder(m_vehicle,this);
    //connect(m_decoder.data(), &MAVLinkDecoder::valueChanged, this, &LineChartMainWindow::valueChanged);
    m_linecharts=new Linecharts("Argrument Value Analisys",NULL,m_decoder,this);
    m_decoder->start(QThread::LowPriority);

    this->setCentralWidget(m_linecharts);
    this->resize(1360,600);
    this->setWindowTitle(QString::fromLocal8Bit("Êý¾Ý·ÖÎö"));
    this->setMaximumSize(1360,600);
    this->setMinimumSize(1360,600);
}


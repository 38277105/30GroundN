#include "waypointeditor.h"
#include "ui_waypointeditor.h"

WayPointEditor::WayPointEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WayPointEditor)
{
    ui->setupUi(this);
    //setWindowFlags(Qt::CustomizeWindowHint| Qt::WindowTitleHint );
}

WayPointEditor::~WayPointEditor()
{
    delete ui;
}

void WayPointEditor::SetNumber(const double& lng, const double& lat, const double& alt){
    m_longitude = lng;
    m_latitude = lat;
    m_altitude = alt;
    ui->m_doubleSpinBox_longitude->setValue(m_longitude);
    ui->m_doubleSpinBox_latitude->setValue(m_latitude);
    ui->m_doubleSpinBox_altitude->setValue(m_altitude);
}

void WayPointEditor::on_m_btn_ok_clicked()
{
    if(ui->m_doubleSpinBox_longitude->value() == m_longitude &&  ui->m_doubleSpinBox_latitude->value() == m_latitude && ui->m_doubleSpinBox_altitude->value() == m_altitude){
        this->close();
    }
    m_longitude = ui->m_doubleSpinBox_longitude->value();
    m_latitude = ui->m_doubleSpinBox_latitude->value();
    m_altitude = ui->m_doubleSpinBox_altitude->value();
    emit transferNums(m_longitude, m_latitude, m_altitude);
    this->close();
}

void WayPointEditor::on_m_btn_cancel_clicked()
{
    this->close();
}


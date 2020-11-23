#include "autoplan.h"
#include "ui_autoplan.h"
#include <QMessageBox>
#include <QDebug>
AutoPlan::AutoPlan() :
    ui(new Ui::AutoPlan)
{
    ui->setupUi(this);
    autoPlan_initTablePointsHead();
    setWindowFlags(Qt::WindowMinimizeButtonHint);
    connect(ui->m_btn_add, SIGNAL(clicked(bool)), this, SLOT(autoPlan_addOnePoint_slots(bool)));
    connect(ui->m_btn_delete, SIGNAL(clicked(bool)), this, SLOT(autoPlan_deleteOnePoint_slots(bool)));
    connect(ui->m_btn_ok, SIGNAL(clicked(bool)), this, SLOT(autoPlan_okSlo_slots(bool)));
    connect(ui->m_btn_cancel, SIGNAL(clicked(bool)), this, SLOT(autoPlan_cancel_slots(bool)));
    connect(ui->m_tableWidget_points, SIGNAL(cellChanged(int,int)), this, SLOT(autoPlan_resizeColumn_slots(int,int)));
    connect(ui->m_btn_commit, SIGNAL(clicked(bool)), this, SLOT(autoPlan_commit_slots(bool)));
    connect(ui->m_lineEdit_horizonalDistance, SIGNAL(editingFinished()), this, SLOT(autoPlan_InfoEditFinished_slots()));
    connect(ui->m_lineEdit_verticalDistance, SIGNAL(editingFinished()), this, SLOT(autoPlan_InfoEditFinished_slots()));
    connect(ui->m_lineEdit_angle, SIGNAL(editingFinished()), this, SLOT(autoPlan_InfoEditFinished_slots()));
    connect(ui->m_checkBox_isEnableClick, SIGNAL(clicked(const bool&)), this, SLOT(autoPlan_isEnableClick_slots(const bool&)));
    m_horizonalDistance = "0";
    m_verticalDistance = "0";
    m_angle = "0";
    //lineEdit输入框输入参数的范围
    //距离:10m~1000m
    //疏密:10m~1000m
    //航向角:-180度~180度
    {
        ui->m_lineEdit_horizonalDistance->setValidator(new QIntValidator(0, 1000));
        ui->m_lineEdit_verticalDistance->setValidator(new QIntValidator(0, 1000));
        ui->m_lineEdit_angle->setValidator(new QIntValidator(-180, 180));
    }
    m_isCommited = false;
    QMessageBox::warning(NULL,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("至多支持一个凹角多边形"),QMessageBox::Yes);
}

AutoPlan::~AutoPlan()
{
    delete ui;
}

void AutoPlan::autoPlan_initTablePointsHead(){
    ui->m_tableWidget_points->setColumnCount(3);
    QStringList nameList;
    nameList << QString::fromLocal8Bit("经度") << QString::fromLocal8Bit("纬度") << QString::fromLocal8Bit("高度");
    ui->m_tableWidget_points->setHorizontalHeaderLabels(nameList);
    ui->m_tableWidget_points->resizeColumnsToContents();
}

bool AutoPlan::autoPlan_isNum(const APInfo& info){

    return true;
}

void AutoPlan::autoPlan_addOnePoint_slots(bool flag){
    ui->m_tableWidget_points->insertRow(ui->m_tableWidget_points->rowCount());
    ui->m_tableWidget_points->setItem(ui->m_tableWidget_points->rowCount()-1, 0, new QTableWidgetItem("0"));
    ui->m_tableWidget_points->setItem(ui->m_tableWidget_points->rowCount()-1, 1, new QTableWidgetItem("0"));
    ui->m_tableWidget_points->setItem(ui->m_tableWidget_points->rowCount()-1, 2, new QTableWidgetItem("0"));
}

void AutoPlan::autoPlan_deleteOnePoint_slots(bool flag){
    if(ui->m_tableWidget_points->currentRow() < 0){
        return;
    }
    emit autoPlan_eraseOnePoint(ui->m_tableWidget_points->currentRow());
    ui->m_tableWidget_points->removeRow(ui->m_tableWidget_points->currentRow());
}

void AutoPlan::autoPlan_okSlo_slots(bool flag){
    if(!m_isCommited)
        return;
    ui->m_checkBox_isEnableClick->setChecked(false);
    int len = ui->m_tableWidget_points->rowCount();
    for(int i = 0; i < len; i++){
        ui->m_tableWidget_points->removeRow(0);
    }
    emit autoPlan_finish_signal();
    emit autoPlan_enableButtons();
    emit autoPlan_isEnableClicke_signal(false);
    ui->m_btn_add->setEnabled(true);
    ui->m_btn_delete->setEnabled(true);
    this->close();
}

void AutoPlan::autoPlan_cancel_slots(bool flag){
    ui->m_checkBox_isEnableClick->setChecked(false);
    int len = ui->m_tableWidget_points->rowCount();
    if(len == 0){
        emit autoPlan_enableButtons();
        this->close();
        return;
    }
    for(int i = 0; i < len; i++){
        ui->m_tableWidget_points->removeRow(0);
    }
    emit autoPlan_clearBoundary_signal(true);
    emit autoPlan_isEnableClicke_signal(false);
    emit autoPlan_enableButtons();
    ui->m_btn_add->setEnabled(true);
    ui->m_btn_delete->setEnabled(true);
    this->close();
}

void AutoPlan::autoPlan_resizeColumn_slots(int, int){
    ui->m_tableWidget_points->resizeColumnsToContents();
}

void AutoPlan::autoPlan_commit_slots(bool){
    if(ui->m_tableWidget_points->rowCount() <3){
        QMessageBox::warning(NULL,QString::fromUtf8("提示"),QString::fromUtf8("边界点数目不能少于三"),QMessageBox::Yes);
        return;
    }
    bool flag = false;
    QList<MapPointData> datas;
    for(int i=0; i < ui->m_tableWidget_points->rowCount(); i++){
        MapPointData tmp;
        tmp.lng = ui->m_tableWidget_points->item(i,0)->text().toDouble();
        tmp.lat = ui->m_tableWidget_points->item(i, 1)->text().toDouble();
        tmp.alt = ui->m_tableWidget_points->item(i, 2)->text().toDouble();
        if(tmp.lng == 0 || tmp.lat == 0 || tmp.alt == 0){
            flag = true;
            break;
        }
        datas.push_back(tmp);
    }
    if(flag){
        QMessageBox::warning(NULL,QString::fromUtf8("提示"),QString::fromUtf8("请正确输入经纬高信息"),QMessageBox::Yes);
        return;
    }
    emit autoPlan_commitBoundaryData_signal(datas);
    emit ui->m_lineEdit_angle->editingFinished();
    m_isCommited = true;
    ui->m_btn_add->setEnabled(false);
    ui->m_btn_delete->setEnabled(false);
}

void AutoPlan::autoPlan_InfoEditFinished_slots(){
    APInfo  tmpInfo;
    tmpInfo.strHD = ui->m_lineEdit_horizonalDistance->text();
    tmpInfo.strVD = ui->m_lineEdit_verticalDistance->text();
    tmpInfo.strAngle = ui->m_lineEdit_angle->text();
    if(tmpInfo.strHD.toInt() < 10){
       tmpInfo.strHD = "10";
       ui->m_lineEdit_horizonalDistance->setText("10");
    }
    if(tmpInfo.strVD.toInt() < 10){
       tmpInfo.strVD = "10";
       ui->m_lineEdit_verticalDistance->setText("10");
    }
    emit autoPlan_updateInfoNums_siganl(tmpInfo);
}

void AutoPlan::autoPlan_isEnableClick_slots(const bool& isChecked){
    emit autoPlan_isEnableClicke_signal(isChecked);
}

void AutoPlan::autoPlan_mouseAddOnePoint_slots(const MapPointData& data){
    ui->m_tableWidget_points->insertRow(ui->m_tableWidget_points->rowCount());
    ui->m_tableWidget_points->setItem(ui->m_tableWidget_points->rowCount()-1, 0, new QTableWidgetItem(QString::number(data.lng, 'f', 7)));
    ui->m_tableWidget_points->setItem(ui->m_tableWidget_points->rowCount()-1, 1, new QTableWidgetItem(QString::number(data.lat, 'f', 7)));
    ui->m_tableWidget_points->setItem(ui->m_tableWidget_points->rowCount()-1, 2, new QTableWidgetItem(QString::number(data.alt, 'f', 7)));
}

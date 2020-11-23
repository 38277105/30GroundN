#include "testform.h"
#include "ui_testform.h"
#include <QMessageBox>
#include "../controllers/frmmaincontroller.h"

TestForm::TestForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestForm)
{
    ui->setupUi(this);
    connect(FrmMainController::Instance(),SIGNAL(showCmdInfo(int,QString)),this,SLOT(on_showCmdInfo(int,QString)));
}

TestForm::~TestForm()
{
    delete ui;
}

void TestForm::on_pbSend_clicked()
{
    int cmd=ui->txtId->text().toInt();
    if(cmd<=0)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("命令id值不能为0"));
        return;
    }
    Vehicle* pVeh= FrmMainController::Instance()->__vehicle;
    if(!pVeh)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("飞机不存在"));
        return;
    }
    float p1=ui->txtPar1->text().toFloat();
    float p2=ui->txtPar2->text().toFloat();
    float p3=ui->txtPar3->text().toFloat();
    float p4=ui->txtPar4->text().toFloat();
    float p5=ui->txtPar5->text().toFloat();
    float p6=ui->txtPar6->text().toFloat();
    float p7=ui->txtPar7->text().toFloat();
    pVeh->mavLinkMessageInterface.doCommand((MAV_CMD)cmd,p1,p2,p3,p4,p5,p6,p7);
}

void TestForm::on_showCmdInfo(int cmd, QString str)
{
    int nCmd=ui->txtId->text().toInt();
    if(cmd==nCmd)
        ui->txtShow->append(str);
}

void TestForm::on_pbClear_clicked()
{
    ui->txtShow->clear();
}

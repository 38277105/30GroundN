#include "firmwareform.h"
#include "ui_firmwareform.h"
#include <QSerialPortInfo>
#include <QFileDialog>
#include "firmware/FirmwareUpgradeController.h"

FirmWareForm::FirmWareForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FirmWareForm)
{
    ui->setupUi(this);
    m_pFirm=new FirmwareUpgradeController();
    connect(m_pFirm,SIGNAL(infoMsg(QString)),this,SLOT(AppendTxt(QString)));
    connect(m_pFirm,SIGNAL(updateProgress(float)),this,SLOT(SetProgress(float)));
    ui->proBar->setRange(0,100);
    ui->proBar->setValue(0);
    FindPort();
    connect(&m_tmPort,SIGNAL(timeout()),this,SLOT(FindPort()));
    m_tmPort.start(2000);
}

FirmWareForm::~FirmWareForm()
{
    delete ui;
}

void FirmWareForm::AppendTxt(QString str)
{
    ui->txtShow->append(str);
}

void FirmWareForm::SetProgress(float val)
{
    ui->proBar->setValue(val*100);
}

void FirmWareForm::FindPort()
{
    QList<QSerialPortInfo> lst=QSerialPortInfo::availablePorts();
    if(lst.count()!=ui->cbBoxPort->count())
    {
        ui->cbBoxPort->clear();
        foreach (QSerialPortInfo info, lst)
        {
            ui->cbBoxPort->insertItem(0,info.portName());
        }
        ui->cbBoxPort->setCurrentIndex(0);
    }
}

void FirmWareForm::on_pbSelFile_clicked()
{
    m_fmFile = QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("固件选择"),".",
                                                    QString::fromLocal8Bit("固件文件(*bin)"));
    ui->txtShow->append(m_fmFile);
    ui->pbFlashing->setEnabled(true);
}

void FirmWareForm::on_pbFlashing_clicked()
{
    if(m_pFirm)
    {
        m_pFirm->flashFirmware(m_fmFile,ui->cbBoxPort->currentText(),
                               ui->cbBoxBuad->currentText(),false);
    }
}

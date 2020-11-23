#include "magcalform.h"
#include "ui_magcalform.h"
#include <QCoreApplication>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include "libeigen/ellipsoidmag.h"

#define MIN_DATA_NUM 300
MagCalForm::MagCalForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MagCalForm)
{
    ui->setupUi(this);

    m_sBasePath=QCoreApplication::applicationDirPath()+"/data/";
    QDir dr(m_sBasePath);
    if(!dr.exists())
        dr.mkdir(m_sBasePath);

    m_nCurMagId=1;
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::red);
    ui->curveSrc->addLine(LX,"MagX",pen);
    ui->curveCut->addLine(LX,"MagX",pen);
    pen.setColor(Qt::blue);
    ui->curveSrc->addLine(LY,"MagY",pen);
    ui->curveCut->addLine(LY,"MagY",pen);
    pen.setColor(Qt::magenta);
    ui->curveSrc->addLine(LZ,"MagZ",pen);
    ui->curveCut->addLine(LZ,"MagZ",pen);
    for(int i=0;i<MAX_MAG;i++)
    {
        m_magDataSrc[i].nCount=0;
        m_magDataCut[i].nCount=0;
    }
    connect(&cc,SIGNAL(sendMagData(int,const float*,const float*,const float*,int)),
            this,SLOT(RecvMagData(int,const float*,const float*,const float*,int)),Qt::DirectConnection);
    cc.ReadData(2);
    cc.ReadData(1);
}

MagCalForm::~MagCalForm()
{
    delete ui;
}

void MagCalForm::ShowSrcMagData()
{
    ui->curveSrc->addPts(LX,m_magDataSrc[m_nCurMagId].keys,m_magDataSrc[m_nCurMagId].valsX);
    ui->curveSrc->addPts(LY,m_magDataSrc[m_nCurMagId].keys,m_magDataSrc[m_nCurMagId].valsY);
    ui->curveSrc->addPts(LZ,m_magDataSrc[m_nCurMagId].keys,m_magDataSrc[m_nCurMagId].valsZ);

    float low,uper;
    ui->curveSrc->yRange(low,uper);
    ui->txtMin->setText(QString::number(low,'f',4));
    ui->txtMax->setText(QString::number(uper,'f',4));
}
void MagCalForm::on_pbCut_clicked()
{
    if(m_magDataSrc[m_nCurMagId].nCount<MIN_DATA_NUM)
    {
        QMessageBox::warning(NULL,QString::fromLocal8Bit("警告"),
                 QString::fromLocal8Bit("外磁的数据不能少于300条"));
        return;
    }
    //ui->curveCut->clearData();
    m_magDataCut[m_nCurMagId].Clear();
    double minVal=ui->txtMin->text().toDouble();
    double maxVal=ui->txtMax->text().toDouble();
    int    nCount=0;
    for(int i=0;i<m_magDataSrc[m_nCurMagId].nCount;i++)
    {
        if(m_magDataSrc[m_nCurMagId].valsX[i]>minVal
            && m_magDataSrc[m_nCurMagId].valsY[i]>minVal
            && m_magDataSrc[m_nCurMagId].valsZ[i]>minVal
            && m_magDataSrc[m_nCurMagId].valsX[i]<maxVal
            && m_magDataSrc[m_nCurMagId].valsY[i]<maxVal
            && m_magDataSrc[m_nCurMagId].valsZ[i]<maxVal)
        {
            m_magDataCut[m_nCurMagId].keys.push_back(nCount);
            m_magDataCut[m_nCurMagId].valsX.push_back(m_magDataSrc[m_nCurMagId].valsX[i]);
            m_magDataCut[m_nCurMagId].valsY.push_back(m_magDataSrc[m_nCurMagId].valsY[i]);
            m_magDataCut[m_nCurMagId].valsZ.push_back(m_magDataSrc[m_nCurMagId].valsZ[i]);
            nCount++;
        }

    }
    m_magDataCut[m_nCurMagId].nCount=nCount;
    ui->curveCut->addPts(LX,m_magDataCut[m_nCurMagId].keys,m_magDataCut[m_nCurMagId].valsX);
    ui->curveCut->addPts(LY,m_magDataCut[m_nCurMagId].keys,m_magDataCut[m_nCurMagId].valsY);
    ui->curveCut->addPts(LZ,m_magDataCut[m_nCurMagId].keys,m_magDataCut[m_nCurMagId].valsZ);

    QString str=QString::fromLocal8Bit("源数据 %1条,截取后 %2条").arg(m_magDataSrc[m_nCurMagId].nCount).arg(nCount);
    ui->labShow->setText(str);
}
void MagCalForm::on_cbxMag_currentTextChanged(const QString &arg1)
{
    int nMagid=arg1.right(1).toInt();
    if(nMagid>0 && nMagid<MAX_MAG)
    {
        m_nCurMagId=nMagid;
        ShowSrcMagData();
    }
}
void MagCalForm::on_pbSaveMag_clicked()
{
    if(m_magDataCut[m_nCurMagId].nCount<MIN_DATA_NUM)
    {
        QMessageBox::warning(NULL,QString::fromLocal8Bit("警告"),
                 QString::fromLocal8Bit("截取后的外磁的数据不能少于300条"));
        return;
    }
    EllipsoidMag cal;
    cal.Mag_Fit(m_magDataCut[m_nCurMagId]);  //计算磁偏
    for(int i=0;i<9;i++)
    {
        if(qIsNaN(m_magDataCut[m_nCurMagId].magB[i]))
        {
            QString str=QString::fromLocal8Bit("外磁%1,计算出错，非法值").arg(m_nCurMagId);
            QMessageBox::warning(NULL,QString::fromLocal8Bit("警告"),str);
            return;
        }
    }
    //写本地
    if(ui->cbRecToFile->isChecked())
    {
        QString tmp;
        QString str=m_sBasePath+QString("inMag%1.csv").arg(m_nCurMagId);
        QFile fIn(str);
        if(fIn.open(QFile::WriteOnly))
        {
            for(int i=0;i<m_magDataSrc[m_nCurMagId].nCount;i++)
            {
                tmp=QString("%1,%2,%3\r\n").arg(m_magDataSrc[m_nCurMagId].valsX[i],0,'f',4)
                        .arg(m_magDataSrc[m_nCurMagId].valsY[i],0,'f',4)
                        .arg(m_magDataSrc[m_nCurMagId].valsZ[i],0,'f',4);
                        //.arg(m_magDataSrc[m_nCurMagId].err[i],0,'f',8);
                fIn.write(tmp.toStdString().c_str());
            }
            fIn.close();
        }

        str=m_sBasePath+QString("OutMag%1.csv").arg(m_nCurMagId);
        QFile fOut(str);
        if(fOut.open(QFile::WriteOnly))
        {
            for(int i=0;i<9;i++)
            {
               if(i==0)
                   tmp=QString::number(m_magDataCut[m_nCurMagId].magB[i],'g',10);
               else
                   tmp+=","+QString::number(m_magDataCut[m_nCurMagId].magB[i],'g',10);
            }
            fOut.write(tmp.toStdString().c_str());
            fOut.close();
        }
    }
    //写飞控

    QString str=QString::fromLocal8Bit("外磁%1,计算并写入成功").arg(m_nCurMagId);
    QMessageBox::information(NULL,QString::fromLocal8Bit("信息"),str);
}
void MagCalForm::RecvMagData(int magId,const float* x, const float* y, const float* z, int sz)
{
    if(magId<0 || magId>=MAX_MAG)
        return;
    m_magDataSrc[magId].Clear();
    m_magDataSrc[magId].nCount=sz;
    for(int i=0;i<sz;i++)
    {
        m_magDataSrc[magId].keys.push_back(i);
        m_magDataSrc[magId].valsX.push_back(x[i]);
        m_magDataSrc[magId].valsY.push_back(y[i]);
        m_magDataSrc[magId].valsZ.push_back(z[i]);
    }
    QString str=QString::fromLocal8Bit("外磁%1").arg(magId);
    if(ui->cbxMag->findText(str)<0)
        ui->cbxMag->addItem(str);
}

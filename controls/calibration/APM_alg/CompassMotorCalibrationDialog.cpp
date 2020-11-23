//#include "QsLog.h"
//#include "qcustomplot.h"
//#include "UASInterface.h"
//#include "UAS.h"
//#include "UASManager.h"
//#include "QGCUASParamManager.h"
#include <QMessageBox>

#include "CompassMotorCalibrationDialog.h"
#include "ui_CompassMotorCalibrationDialog.h"
#include "controllers/frmmaincontroller.h"

static const int GRAPH_ID_INTERFERENCE = 0;
static const int GRAPH_ID_CURRENT = 1;

static const QColor GRAPH_COLOR_INTERFERENCE = Qt::blue; // Blue
static const QColor GRAPH_COLOR_CURRENT = Qt::red; // Red
static const QColor GRAPH_COLOR_INTERFERENCE_FILL = QColor(0, 0, 255, 20); // Blue
static const QColor GRAPH_COLOR_CURRENT_FILL = QColor(0, 255, 0, 20); // Red

CompassMotorCalibrationDialog::CompassMotorCalibrationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CompassMotorCalibrationDialog)
    //m_uasInterface(NULL)
{
    ui->setupUi(this);

//    QCustomPlot* customPlot = ui->customPlot;
//    customPlot->addGraph();
//    customPlot->graph(GRAPH_ID_INTERFERENCE)->setPen(QPen(GRAPH_COLOR_INTERFERENCE)); // line color blue for first graph
//    customPlot->graph(GRAPH_ID_INTERFERENCE)->setBrush(QBrush(GRAPH_COLOR_INTERFERENCE_FILL)); // first graph will be filled with translucent blue

//    customPlot->xAxis->setLabel("Throttle (%)");

//    customPlot->yAxis->setLabel("Interference (%)");
//    customPlot->yAxis->setLabelColor(GRAPH_COLOR_INTERFERENCE);
//    customPlot->xAxis->setRange(0,100);
//    customPlot->yAxis->setRange(0,100);

//    customPlot->addGraph();
//    customPlot->graph(GRAPH_ID_CURRENT)->setPen(QPen(GRAPH_COLOR_CURRENT)); // line color red for second graph
//    customPlot->graph(GRAPH_ID_CURRENT)->setBrush(QBrush(GRAPH_COLOR_CURRENT_FILL));

//    customPlot->yAxis2->setVisible(true);
//    customPlot->yAxis2->setLabel("Amps (A)");
//    customPlot->yAxis2->setLabelColor(GRAPH_COLOR_CURRENT);
//    customPlot->xAxis2->setRange(0,100);
//    customPlot->yAxis2->setRange(0,50);

//    customPlot->replot();

    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
    connect(this, SIGNAL(about), this, SLOT(rejected()));
    //connect(UASManager::instance(),SIGNAL(activeUASSet(UASInterface*)),this,SLOT(activeUASSet(UASInterface*)));
    //activeUASSet(UASManager::instance()->getActiveUAS());

    int ok = QMessageBox::warning(this, "Compass Motor Calibration", tr("CAUTION: Starting the compass motor calibration arms the motors.\n"
                                                               "Please make sure you have read and followed all instructions"
                                                               "before untertaking the calibration as serious injury could occur!"),
                         QMessageBox::Ok, QMessageBox::Cancel);
    if (ok == QMessageBox::Cancel){
        QTimer::singleShot(100, this, SLOT(cancelCalibration()));
    }

}

CompassMotorCalibrationDialog::~CompassMotorCalibrationDialog()
{
    cancelCalibration();
    delete ui;
}

void CompassMotorCalibrationDialog::makeConnection()
{

        connect(ui->startButton, SIGNAL(clicked()), this, SLOT(startCalibration()));
        connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stopCalibration()));
//        connect(m_uasInterface, SIGNAL(compassMotCalibration(mavlink_compassmot_status_t*)),
//                this, SLOT(compassMotCalibration(mavlink_compassmot_status_t*)));

}

void CompassMotorCalibrationDialog::compassMotCalibration(mavlink_compassmot_status_t *compassmot_status)
{
    x_scalar = compassmot_status->CompensationX;
    y_scalar = compassmot_status->CompensationY;
    z_scalar = compassmot_status->CompensationZ;

}

void CompassMotorCalibrationDialog::textMessageReceived(int uasid, int componentid,
                                                        int severity, QString text)
{
    Q_UNUSED(uasid);
    Q_UNUSED(componentid);
    Q_UNUSED(severity);
    ui->messageTextEdit->insertPlainText(text +"\n");
}

void CompassMotorCalibrationDialog::startCalibration()
{
    FrmMainController::Instance()->startCompassMotCalibration();
    connect(FrmMainController::Instance(), SIGNAL(textMessageReceived(int,int,int,QString)),
            this, SLOT(textMessageReceived(int,int,int,QString)), Qt::UniqueConnection);
}



void CompassMotorCalibrationDialog::stopCalibration()
{
    disconnect(FrmMainController::Instance(), SIGNAL(textMessageReceived(int,int,int,QString)),
            this, SLOT(textMessageReceived(int,int,int,QString)));

    // Need to send ACK
    //m_uasInterface->executeCommandAck(MAV_CMD_PREFLIGHT_CALIBRATION, MAV_RESULT_ACCEPTED);
    FrmMainController::Instance()->SendCommandACK(MAV_CMD_PREFLIGHT_CALIBRATION,MAV_RESULT_ACCEPTED);
    ui->messageTextEdit->insertPlainText("Calibration Stopped\n");
}

void CompassMotorCalibrationDialog::okButtonClicked()
{
//    if(m_uasInterface){
        cancelCalibration();
      //  QLOG_DEBUG() << "COMPASSMOT: accepted = retrieving new values";
//        m_uasInterface->requestParameter(1, "COMPASS_MOT_X");
//        m_uasInterface->requestParameter(1, "COMPASS_MOT_Y");
//        m_uasInterface->requestParameter(1, "COMPASS_MOT_Z");
        FrmMainController::Instance()->cancelCompassMotCalibration();
        QMessageBox::information(this, "Sucess!",
                                 QString("New values have been stored\n X:%1 Y:%2 Z:%3")
                                 .arg(QString::number(x_scalar))
                                 .arg(QString::number(y_scalar))
                                 .arg(QString::number(z_scalar)));
//    }
    accept();
}

void CompassMotorCalibrationDialog::cancelCalibration()
{
//    if(m_uasInterface){
//        m_uasInterface->executeCommandAck(MAV_CMD_PREFLIGHT_CALIBRATION, MAV_RESULT_FAILED);
//    }
    FrmMainController::Instance()->SendCommandACK(MAV_CMD_PREFLIGHT_CALIBRATION,MAV_RESULT_FAILED);
}

void CompassMotorCalibrationDialog::closeEvent(QCloseEvent *)
{
    cancelCalibration();
}


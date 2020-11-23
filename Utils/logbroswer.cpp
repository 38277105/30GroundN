#include "logbroswer.h"
#include <QHBoxLayout>
#include <QCoreApplication>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include <QKeyEvent>

LogBroswer::LogBroswer(QWidget *parent) : QWidget(parent)
{
    this->resize(800, 600);
    this->setWindowTitle(QString::fromLocal8Bit("日志窗口"));
    browser =new QTextBrowser();
    qDebug()<<"browser:"<<browser;
    QHBoxLayout* ly=new QHBoxLayout();
    qDebug()<<"ly:"<<ly;
    ly->addWidget(browser);
    this->setLayout(ly);

    connect(this,SIGNAL(SIGNAL_outputMessage(QtMsgType,QMessageLogContext,QString)),this,SLOT(outputMessage(QtMsgType,QMessageLogContext,QString)));
}
LogBroswer::~LogBroswer()
{

}
void LogBroswer::logMsg(QString info)
{
    QCoreApplication::processEvents();
    //qDebug()<<info;
}
void LogBroswer::outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString message;
    switch(type)
    {
        case QtDebugMsg:
        message = QString("Debug:");
        break;
        case QtWarningMsg:
        message = QString("Warning:");
        break;
        case QtCriticalMsg:
        message = QString("Critical:");
        break;
        case QtFatalMsg:
        message = QString("Fatal:");
    }
    browser->append(message.append(msg));
}

void LogBroswer::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton answer = QMessageBox::question(
    this,
    tr("Close Log Browser?"),
    tr("Do you really want to close the log browser?"),
    QMessageBox::Yes | QMessageBox::No
    );
    if (answer == QMessageBox::Yes)
    event->accept();
    else
    event->ignore();
}

void LogBroswer::keyPressEvent(QKeyEvent *event)
{
    event->ignore();
}

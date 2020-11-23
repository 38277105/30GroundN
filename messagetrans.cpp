#include "messagetrans.h"
#include <QApplication>
#include <QTextStream>
#include <QDebug>

QFile MessageTrans::m_logDebugFile("MyDebugLog.txt");
QFile MessageTrans::m_logWarningFile("MyWarningLog.txt");
QFile MessageTrans::m_logErrorFile("MyErrorLog.txt");
MessageTrans::MessageTrans(QObject *parent) : QObject(parent)
{

}

MessageTrans::~MessageTrans()
{
    if(m_logDebugFile.isOpen())
        m_logDebugFile.close();
    if(m_logWarningFile.isOpen())
        m_logWarningFile.close();
    if(m_logErrorFile.isOpen())
        m_logErrorFile.close();
}

void MessageTrans::Init()
{
    // 获取命令行参数
    QStringList args = QCoreApplication::arguments();
    qDebug() << "Arguments : " << args;
    if (args.count() < 2)
        return;
    QString sCmd=args.at(1);
    if(sCmd=="-f")
    {
        qInstallMessageHandler(&MessageTrans::TranMsg);
    }
}

void MessageTrans::TranMsg(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString txtMessage;
    switch (type)
    {
        case QtDebugMsg:    //调试信息提示
        {
            if(!m_logDebugFile.isOpen())
                m_logDebugFile.open(QFile::WriteOnly|QFile::Truncate);
            QTextStream textStream(&m_logDebugFile);
            txtMessage = QString("%1  %2  %3\r\n").arg(context.file).arg(context.line).arg(msg);
            textStream << txtMessage << endl;
            m_logDebugFile.flush();
            break;
        }
        case QtWarningMsg:    //一般的warning提示
        {
            return;
            if(!m_logWarningFile.isOpen())
                m_logWarningFile.open(QFile::WriteOnly|QFile::Truncate);
            QTextStream textStream(&m_logWarningFile);
            txtMessage = QString("%1  %2  %3\r\n").arg(context.file).arg(context.line).arg(msg);
            textStream << txtMessage << endl;
            m_logWarningFile.flush();
            break;
        }
        case QtCriticalMsg:    //严重错误提示
        case QtFatalMsg:    //致命错误提示
        {
            if(!m_logErrorFile.isOpen())
                m_logErrorFile.open(QFile::WriteOnly|QFile::Truncate);
            QTextStream textStream(&m_logErrorFile);
            txtMessage = QString("%1  %2  %3\r\n").arg(context.file).arg(context.line).arg(msg);
            textStream << txtMessage << endl;
            m_logErrorFile.flush();
            if(type==QtFatalMsg)
                abort();
            break;
        }
    }
}

#include "runninglog.h"
#include <QDir>
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>
runningLog::runningLog(QObject *parent) : QObject(parent)
{
    initDir();
    m_file = NULL;
    m_lines = 0;
}

void runningLog::start(){
    if(m_file){
        m_lines = 0;
        fflush(m_file);
        fclose(m_file);
    }
    QString filename = RUNNINGLOG_DIR+QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    m_file = fopen(filename.toLocal8Bit(), "wb");
    if(!m_file){
        QMessageBox::warning(NULL,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("创建实时日志文件失败"),QMessageBox::Yes);
        return;
    }
}

int runningLog::parserunningLogFile(){
    FILE* file = fopen("./data/runninglog/20170224171513", "rb");
    if(!file){
        QMessageBox::warning(NULL,QString::fromUtf8("提示"),QString::fromUtf8("打开实时日志文件失败"),QMessageBox::Yes);
        return false;
    }
    while(!feof(file)){
        mavlink_message_t message;
        int len = fread(&message, 1, sizeof(message), file);
        if(len == 0){
            continue;
        }
        qDebug() << "read num:" << len << '\n';
        printMessage(message);
        emit runninglogMessage_siganl(message);
        //Sleep(300);
    }
    return true;
}

void runningLog::flushfilebuffer(){
    fflush(m_file);
}

void runningLog::messLogData(mavlink_message_t message){
    int len = fwrite(&message, 1, sizeof(message), m_file);
    fflush(m_file);
    qDebug() << "write num:" << len << '\n';
    printMessage(message);
}

void runningLog::initDir(){
    //创建目录
    QDir dir;
    if(!dir.exists(RUNNINGLOG_DIR)){
        dir.mkdir(RUNNINGLOG_DIR);
    }
}

void runningLog::printMessage(mavlink_message_t message){
    m_lines++;
    int index = 0;
    while(true){
        if(message.payload64[index] == 0 && message.payload64[index+1] == 0 && message.payload64[index+2] == 0){
            break;
        }
        qDebug() << message.payload64[index] << '\n';
        index++;
    }
    qDebug() << sizeof(message) << '/' << message.msgid<< '/' << m_lines << '\n';
}

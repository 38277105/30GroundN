#include "logger.h"
#include <QDateTime>
#include <QTextStream>

static Logger* m_logger=NULL;
Logger::Logger(QObject *parent) : QObject(parent)
{
    //fileName=fileName.sprintf("log_%s.txt",QDateTime::currentDateTime().toString("yyyyMMddHHmm"));
//    QDateTime dt;
//    QTime time;
//    QDate date;
//    dt.setTime(time.currentTime());
//    dt.setDate(date.currentDate());
//    QString t_timestr = dt.toString("yyyy_MM_dd_hh");

//    memset(fileName,0,128*sizeof(char));
//    std::sprintf(fileName,"./Data_%s",t_timestr.toStdString().c_str());
//    if((m_fp=fopen(fileName,"a+"))==NULL)
//    {
//       m_fp=fopen(fileName,"w+");
//    }
}
Logger::~Logger()
{
    fclose(m_fp);
}
void Logger::LogE(QString content)
{
    std::fprintf(m_fp,"[Exception happen at:]%s\n\n\n",content);
}
void Logger::LogN(QString content)
{
    std::fprintf(m_fp,"[Note:]%s\n\n\n",content);
}
void Logger::LogInfo(QString content)
{
//        QTextStream txtOutput(m_fp);
//        txtOutput << content << endl;
//        fflush(m_fp);
}
void Logger::LogC(QString content)
{
    std::fprintf(m_fp,"[Content:]%s\n\n\n",content);
}
Logger* Logger::Instance()
{
   if(m_logger==NULL)
       m_logger=new Logger(NULL);
   return m_logger;
}

void Logger::LogStruct(mavlink_message_t msg)
{
//    if(NULL!=m_fp)
//    {
//     std::fwrite(&msg,sizeof(mavlink_message_t),1,m_fp);
//    }

}

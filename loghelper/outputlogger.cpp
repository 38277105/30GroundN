#include "outputlogger.h"
#include <QHBoxLayout>

OutPutLogger::OutPutLogger(QObject *parent) : QObject(parent)
{
    browser =new QTextBrowser();

}
OutPutLogger::~OutPutLogger()
{
   //delete browser;
}
QTextBrowser* OutPutLogger::getBrowser()
{
   return browser;
}
void OutPutLogger::logmsg(QString& strcontent)
{
    browser->append(strcontent);
}
void OutPutLogger::clear()
{
    browser->clear();
}

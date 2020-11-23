#ifndef OUTPUTLOGGER_H
#define OUTPUTLOGGER_H

#include <QWidget>
#include <QTextBrowser>

class OutPutLogger : public QObject
{
    Q_OBJECT
public:
    explicit OutPutLogger(QObject *parent = 0);
    ~OutPutLogger();
signals:

public slots:
    void logmsg(QString& strcontent);
    void clear();
public:
    QTextBrowser* getBrowser();
private:
    QTextBrowser* browser;
};

#endif // OUTPUTLOGGER_H

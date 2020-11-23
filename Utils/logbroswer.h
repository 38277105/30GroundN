#ifndef LOGBROSWER_H
#define LOGBROSWER_H

#include <QWidget>
#include <QTextBrowser>

class LogBroswer : public QWidget
{
    Q_OBJECT
public:
    explicit LogBroswer(QWidget *parent = 0);
    ~LogBroswer();

signals:

public slots:
    void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    void logMsg(QString info);
private:
    QTextBrowser* browser;
protected:
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);

signals:
    void SIGNAL_outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);
};

#endif // LOGBROSWER_H

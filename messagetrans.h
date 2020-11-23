#ifndef MESSAGETRANS_H
#define MESSAGETRANS_H

#include <QObject>
#include <QFile>

class MessageTrans : public QObject
{
    Q_OBJECT
public:
    explicit MessageTrans(QObject *parent = 0);
    ~MessageTrans();
    void Init();
private:
    static void TranMsg(QtMsgType type, const QMessageLogContext &context, const QString &msg);
private:
    static QFile  m_logDebugFile;
    static QFile  m_logWarningFile;
    static QFile  m_logErrorFile;
};

#endif // MESSAGETRANS_H

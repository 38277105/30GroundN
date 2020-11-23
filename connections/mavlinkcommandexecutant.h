#ifndef MAVLINKCOMMANDEXECUTANT_H
#define MAVLINKCOMMANDEXECUTANT_H

#include <QObject>
//#include <common/mavlink.h>
#include "connections/CommonStru.h"
#include <map>
#include <qtimer.h>
#include <qcoreevent.h>

class MavLinkCommandExecutant:public QObject
{
    Q_OBJECT
public:
    MavLinkCommandExecutant();

public:
    void shortCommand(mavlink_command_int_t& mavlinkCommand);
    void longCommand(mavlink_command_long_t& mavlinkCommand);
    void commandAck(mavlink_command_ack_t& commandAck);

private slots:
    void __timerOut();
private:
    uint16_t __command;
    QTimer __timer;
signals:
    void onCommandAck(uint16_t command, uint8_t result);
};

#endif // MAVLINKCOMMANDEXECUTANT_H

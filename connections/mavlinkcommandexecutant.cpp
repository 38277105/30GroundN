#include "mavlinkcommandexecutant.h"

MavLinkCommandExecutant::MavLinkCommandExecutant()
{
    connect(&__timer, SIGNAL(timeout()), this, SLOT(__timerOut()));
}

void MavLinkCommandExecutant::__timerOut(){
    emit onCommandAck( __command, 100);
    __timer.stop();
}

void MavLinkCommandExecutant::shortCommand(mavlink_command_int_t& mavlinkCommand){
    __timer.start(1000);
    __command = mavlinkCommand.command;
}

void MavLinkCommandExecutant::longCommand(mavlink_command_long_t& mavlinkCommand){
    __timer.start(1000);
    __command = mavlinkCommand.command;
}

void MavLinkCommandExecutant::commandAck(mavlink_command_ack_t& commandAck){
    //短命令应答
    emit onCommandAck(commandAck.command, commandAck.result);
    __timer.stop();
}

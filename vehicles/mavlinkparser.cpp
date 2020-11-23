#include "mavlinkparser.h"

MavLinkParser::MavLinkParser()
{

}
MavLinkParser::~MavLinkParser(){}

QVariant MavLinkParser::mavlink_Parse(int systemid,char* buffer)
{
    switch (systemid) {
    case 1:
        return 1; //类型1
        break;
    case 2:
        return 1; //类型1
        break;
    case 3:
        return 1; //类型1
        break;
    default:
        return 0;
        break;
    }
}

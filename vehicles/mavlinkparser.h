#ifndef MAVLINKPARSER_H
#define MAVLINKPARSER_H

#include <QObject>
#include "linkparser.h"
#include "qvariant.h"

class MavLinkParser : public LinkParser
{
    Q_OBJECT
private:
    MavLinkParser();
    ~MavLinkParser();

signals:

public slots:
public :
    QVariant mavlink_Parse(int systemid,char* buffer);
};

#endif // MAVLINKPARSER_H

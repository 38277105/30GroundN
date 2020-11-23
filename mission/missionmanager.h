#ifndef MISSIONMANAGER_H
#define MISSIONMANAGER_H

#include <QObject>

class MissionManager : public QObject
{
    Q_OBJECT
public:
    explicit MissionManager(QObject *parent = 0);

signals:

public slots:
};

#endif // MISSIONMANAGER_H

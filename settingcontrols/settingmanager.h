#ifndef SETTINGMANAGER_H
#define SETTINGMANAGER_H

#include <QObject>
#include <QString>
#include "connections/datastruct.h"
//#include <common/mavlink.h>
#include "connections/CommonStru.h"
#include <stdint.h>
#include <QMap>

class SettingManager : public QObject
{
    Q_OBJECT
public:
    explicit SettingManager(QObject *parent = 0);
    ~SettingManager();

   static SettingManager* Instance();

    QList<ParamData>* LoadSettings(QString filePath);

    QList<ParamData>* LoadPartSettings(QString config,QString filePath);

    void SaveSetting(QList<ParamData>* plist,QString filePath);

    QList<ParamData>* getParametersFromControl();

    QList<ParamData>* getFullParameters();

    QMap<int,QString> m_types;

    static QList<MultiData>* getMultilang();

    bool _ParamExist(char* argName);


signals:

public slots:

private:
     QList<ParamData>* m_plist;
     QList<ParamData>* m_full_plist;

};



#endif // SETTINGMANAGER_H

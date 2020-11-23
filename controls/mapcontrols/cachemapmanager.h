#ifndef CACHEMAPMANAGER_H
#define CACHEMAPMANAGER_H

#include <QWidget>
#include <QtXml>
namespace Ui {
class CacheMapManager;
}

enum CMDataType{
    LEFTUP,
    LEFTDOWN,
    RIGHTUP,
    RIGHTDOWN,
};

typedef struct{
    QString    strName;
    QString    strZoom;
    QString    strLeftUpLat;
    QString    strLeftDownLat;
    QString    strRightUpLat;
    QString    strRightDownLat;

    QString    strLeftUpLng;
    QString    strLeftDownLng;
    QString    strRightUpLng;
    QString    strRightDownLng;
}CacheMapInfoData;

class CacheMapManager : public QWidget
{
    Q_OBJECT

public:
    explicit CacheMapManager();
    ~CacheMapManager();
private:
    void initZooms();
    void createRecord();
    void UpdateRecordXml();
    int  ReadRecordXml();
    int  WriteRecordXml();
    int  UpdateShowData(int index);
private:
    QString                   m_fileName;
    QList<CacheMapInfoData*>  m_recordList;
    Ui::CacheMapManager *   ui;
};

#endif // CACHEMAPMANAGER_H

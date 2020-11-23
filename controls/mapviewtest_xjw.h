#ifndef MAPVIEWTEST_XJW_H
#define MAPVIEWTEST_XJW_H
#include <QtCore>
#include <QtGui>
#include <opmapwidget.h>

using namespace mapcontrol;

class MapViewTest_XJW : public OPMapWidget
{
Q_OBJECT
public:
    explicit  MapViewTest_XJW(QWidget *parent = 0);
    virtual ~MapViewTest_XJW();
    void setConf(QSettings *conf);
    QSettings* m_conf;
};

#endif // MAPVIEWTEST_XJW_H

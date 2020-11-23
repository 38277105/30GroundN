#ifndef MAINVIEW_H
#define MAINVIEW_H
#include <QtCore>
#include <QtGui>
#include <QWidget>
class MapViewTest_XJW;

class MainView : public QWidget
{
    Q_OBJECT
public:
    explicit MainView(QWidget *parent = 0);
    void initMap();
    MapViewTest_XJW* m_mapView;
    QSettings* m_conf;

signals:

public slots:
};

#endif // MAINVIEW_H

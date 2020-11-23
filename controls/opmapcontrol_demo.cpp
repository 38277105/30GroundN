#include <QApplication>
#include <QWidget>

#include <QtCore>
#include <QtGui>

#include "MapWidget_TestWindow.h"
#include "mapviewtest_xjw.h"
#include "mainview.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTestWin w;
    w.show();
//  MainView mv;
//  mv.show();

    return a.exec();
}

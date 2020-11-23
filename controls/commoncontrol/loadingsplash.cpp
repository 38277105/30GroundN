#include "loadingsplash.h"

LoadingSplash::LoadingSplash(const QPixmap& pixmap) : QSplashScreen(pixmap)
{
    ProgressBar = new QProgressBar(this); //
    ProgressBar->setGeometry(80,386-80,420,24);
    ProgressBar->setRange(0, 100);
    ProgressBar->setValue(0);

    connect(ProgressBar, SIGNAL(valueChanged(int)), this, SLOT(progressChanged(int))); //值改变时，立刻repaint

    QFont font;
    font.setPointSize(20);
    ProgressBar->setFont(font); // 设置进度条里面的字体

    QPalette palette;
    QBrush brush= QBrush(QColor(230,230,230));

    palette.setBrush(ProgressBar->backgroundRole(),brush);
    palette.setBrush(ProgressBar->foregroundRole(),brush);
    ProgressBar->setPalette(palette);
}

LoadingSplash::~LoadingSplash()
{

}
void LoadingSplash::setProgress(int value) //外部改变进度
{
     ProgressBar->setValue(value);
}

void LoadingSplash::progressChanged(int)
{
    repaint();
}

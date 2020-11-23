#include "commonvideoplayer.h"
#include "VLCQtCore/Common.h"
#include "VLCQtWidgets/WidgetVideo.h"
#include <VLCQtCore/Common.h>
#include <VLCQtCore/Instance.h>
#include <VLCQtCore/Media.h>
#include <VLCQtCore/MediaPlayer.h>
#include <QGridLayout>
#include <QPainter>

CommonVideoPlayer::CommonVideoPlayer(QWidget *parent) : QWidget(parent)
{

    setWindowOpacity(1);
    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("QWidget{border:0px}");
    initialize();
//    OpenUrl("udp://@:2000");
}

void CommonVideoPlayer::initialize()
{
    QGridLayout* _qlayout=new QGridLayout();

    _videoWidget = new VlcWidgetVideo();
    _videoWidget->setObjectName(QStringLiteral("vlc_widget_form"));
    this->setLayout(_qlayout);
    _qlayout->addWidget(_videoWidget,0,0,1,1);

    _instance = new VlcInstance(VlcCommon::args(), this);//加载各种插件
    _player = new VlcMediaPlayer(_instance);
    _player->setVideoWidget(_videoWidget);

    _videoWidget->setMediaPlayer(_player);


}
void CommonVideoPlayer::OpenUrl(QString VideoURL)
{
    _media = new VlcMedia(VideoURL, _instance);

    _player->open(_media);
}
void CommonVideoPlayer::OpenLocal(QString filePath)
{
    _media = new VlcMedia(filePath,true, _instance);
    _player->open(_media);
}
void CommonVideoPlayer::paintEvent(QPaintEvent * event)
{
    QPixmap pixmap(_videoWidget->size());
    _videoWidget->render(&pixmap);
    QPainter painter(this);

    QTransform transform;
    transform.translate(width()/2, 0);
    transform.rotate(90,Qt::YAxis);
    painter.setTransform(transform);
    painter.drawPixmap(-1*width()/2,0,pixmap);
}

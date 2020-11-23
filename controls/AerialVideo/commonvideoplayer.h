#ifndef COMMONVIDEOPLAYER_H
#define COMMONVIDEOPLAYER_H



#include <QWidget>

class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;
class VlcWidgetVideo;


class CommonVideoPlayer : public QWidget
{
    Q_OBJECT
public:
    explicit CommonVideoPlayer(QWidget *parent = 0);

signals:

public slots:

private:
    VlcInstance *_instance;
    VlcMedia *_media;
    VlcMediaPlayer *_player;

    VlcWidgetVideo* _videoWidget;

private:
    void initialize();
public:
    void OpenUrl(QString VideoURL);
    void OpenLocal(QString filePath);

protected:
    void  paintEvent(QPaintEvent * event);

};

#endif // COMMONVIDEOPLAYER_H

#ifndef LOADINGSPLASH_H
#define LOADINGSPLASH_H

#include <QObject>
#include <QSplashScreen>
#include <QProgressBar>

class LoadingSplash : public QSplashScreen
{
    Q_OBJECT
public:
    explicit LoadingSplash(const QPixmap& pixmap);
    ~LoadingSplash();
    void setProgress(int value); //外部改变进度
signals:

private slots:
    void progressChanged(int);
private:
    QProgressBar *ProgressBar;
};

#endif // LOADINGSPLASH_H

#ifndef SIGNALCONTROL_H
#define SIGNALCONTROL_H

#include <QWidget>

namespace Ui {
class SignalControl;
}

class SignalControl : public QWidget
{
    Q_OBJECT

public:
    explicit SignalControl(QWidget *parent = 0);
    ~SignalControl();
    //level->5:较强\4:强\3:中\2:弱 <1较弱 0：无;
    void setSignalLevel(int level);

    void setMode(QString mode);

    void setVolt(float volt);

    void setLockState(bool IsLocked);

private:
    Ui::SignalControl *ui;
    QList<QString> m_imageList;
};

#endif // SIGNALCONTROL_H

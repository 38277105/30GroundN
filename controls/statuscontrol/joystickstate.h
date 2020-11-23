#ifndef JOYSTICKSTATE_H
#define JOYSTICKSTATE_H

#include <QWidget>

namespace Ui {
class joystickState;
}

class joystickState : public QWidget
{
    Q_OBJECT

public:
    explicit joystickState(QWidget *parent = 0);
    ~joystickState();

private:
    Ui::joystickState *ui;
};

#endif // JOYSTICKSTATE_H

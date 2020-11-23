#include "joystickstate.h"
#include "ui_joystickstate.h"

joystickState::joystickState(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::joystickState)
{
    ui->setupUi(this);
    //ui->rc_type->setStyleSheet("QRadioButton::indicator {  border-radius: 8px;background-color: green;}");
}

joystickState::~joystickState()
{
    delete ui;
}

#include "joysticktoflycontroller.h"
#include <QDebug>

JoyStickToFlyController::JoyStickToFlyController(QObject *parent) : QObject(parent)
{
    if (SDL_InitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE) < 0) {
        qWarning() << "Couldn't initialize SimpleDirectMediaLayer:" << SDL_GetError();
        return;
    }
    // Load available joysticks
    qDebug() << "Available joysticks";

    m_bOpen=false;
    m_joyStick=NULL;
    for (int i=0; i<SDL_NumJoysticks(); i++)
    {
        QString name = SDL_JoystickName(i);


            int axisCount, buttonCount;

            SDL_Joystick* sdlJoystick = SDL_JoystickOpen(i);
            axisCount = SDL_JoystickNumAxes(sdlJoystick);
            buttonCount = SDL_JoystickNumButtons(sdlJoystick);
            SDL_JoystickClose(sdlJoystick);

            qDebug() << "\t" << name << "axes:" << axisCount << "buttons:" << buttonCount;
            m_joyStick = new FancyJoyStick(name, axisCount, buttonCount, i, this);


            qDebug() << "the index of joystick is" <<i;
    }
}

bool JoyStickToFlyController::isOpened()
{
   return m_bOpen;
}

void JoyStickToFlyController::startPolling(void)
{
    if(m_joyStick!=NULL)
    {
        disconnect(m_joyStick, &FancyJoyStick::joystick_Front, this, &JoyStickToFlyController::joystick_Front);
        connect(m_joyStick, &FancyJoyStick::joystick_Front,  this, &JoyStickToFlyController::joystick_Front, Qt::DirectConnection);

        disconnect(m_joyStick, &FancyJoyStick::joystick_Left, this, &JoyStickToFlyController::joystick_Left);
        connect(m_joyStick, &FancyJoyStick::joystick_Left,  this, &JoyStickToFlyController::joystick_Left, Qt::DirectConnection);

        disconnect(m_joyStick, &FancyJoyStick::joystick_HeightUp, this, &JoyStickToFlyController::joystick_HeightUp);
        connect(m_joyStick, &FancyJoyStick::joystick_HeightUp,  this, &JoyStickToFlyController::joystick_HeightUp);

        disconnect(m_joyStick, &FancyJoyStick::joystick_yaw, this, &JoyStickToFlyController::joystick_yaw);
        connect(m_joyStick, &FancyJoyStick::joystick_yaw,  this, &JoyStickToFlyController::joystick_yaw, Qt::DirectConnection);
        m_joyStick->fstart();
        qDebug() << "::: start";
        m_bOpen=true;
    }
}

void JoyStickToFlyController::stopPolling(void)
{
    if(m_joyStick!=NULL)
    {
        disconnect(m_joyStick, &FancyJoyStick::joystick_Front, this, &JoyStickToFlyController::joystick_Front);
        disconnect(m_joyStick, &FancyJoyStick::joystick_Left, this, &JoyStickToFlyController::joystick_Left);


        disconnect(m_joyStick, &FancyJoyStick::joystick_HeightUp, this, &JoyStickToFlyController::joystick_HeightUp);


        disconnect(m_joyStick, &FancyJoyStick::joystick_yaw, this, &JoyStickToFlyController::joystick_yaw);
        m_joyStick->stop();
        m_bOpen=false;
    }
}

void JoyStickToFlyController::joystick_Front(float frontspeed)//-9000~9000
{
        qDebug()<<"go front:"<<frontspeed;
}

void JoyStickToFlyController::joystick_Left(float leftspeed)//-9000~9000
{
        qDebug()<<"change left:"<<leftspeed;
}

void JoyStickToFlyController::joystick_HeightUp(float updown)//-9000~9000
{

        qDebug()<<"throttle up:"<<updown;

}

void JoyStickToFlyController::joystick_yaw(float yawspeed)//-9000~9000
{

        qDebug()<<"go yaw:"<<yawspeed;
}

void JoyStickToFlyController::joystick_control(int front, int left, int up, int yaw){
    qDebug() << "front: " << front << " left: " << left << " up: " << up << " yaw: " << yaw;
    if(FrmMainController::Instance()->__vehicle){
        FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.sendRC_channels_override(front, left, up, yaw, 0,0);
    }
}

void JoyStickToFlyController::joystick_button1_2(int button1, int button2){
    if(button1 == 1 && button2 == 0){
        qDebug() << "unlock";
        if(FrmMainController::Instance()->__vehicle){
            FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.setModeArm(1);
        }
    }
    if(button1 == 0 && button2 == 1){
        qDebug() << "lock";
        if(FrmMainController::Instance()->__vehicle){
            FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.setModeArm(0);
        }
    }
    qDebug() << "button1: " << button1 << " button2: " << button2;
}

#ifndef FANCYJOYSTICK_H
#define FANCYJOYSTICK_H

#include <QObject>
#include <QThread>

#include "SDL.h"
#include <QMap>

class PodController;
class JoyStickToFlyController;
class FancyJoyStick : public QThread
{
    Q_OBJECT
public:
     FancyJoyStick(const QString& name, int axisCount, int buttonCount, int sdlIndex, JoyStickToFlyController* pJstfC);
     ~FancyJoyStick();

     typedef struct {
         int     min;
         int     max;
         int     center;
         bool    reversed;
     } Calibration_t;




     float _adjustRange(int value, Calibration_t calibration);

     void loadconfig();

     // Override from QThread
     virtual void run(void);
     void fstart();
     void stop();

signals:
        void joystick_Front(float frontspeed);//-2000~2000

        void joystick_Left(float leftspeed);//-2000~2000

        void joystick_HeightUp(float updown);//-1¡¢0¡¢1

        void joystick_yaw(float yawspeed);//yawspeed

public slots:

private:
     int     _sdlIndex;      ///< Index for SDL_JoystickOpen

     bool    _exitThread;    ///< true: signal thread to exit

     QString _name;
     bool    _calibrated;
     int     _axisCount;
     int     _buttonCount;
     PodController*  _MsgHandle;
     JoyStickToFlyController* jstfC;
     QMap<QString,int> _AxisMaps;

     //control variable;
     int                           m_roll;
     int                           m_yaw;
     int                           m_pitch;
     int                           m_thro;
     int                             m_maxNumOfAxis;
     int                             m_maxNumOfNegativeAxis;
};

#endif // FANCYJOYSTICK_H

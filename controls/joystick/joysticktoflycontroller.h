#ifndef JOYSTICKTOFLYCONTROLLER_H
#define JOYSTICKTOFLYCONTROLLER_H

#include <QObject>
#include "fancyjoystick.h"
#include "controllers/frmmaincontroller.h"
/*
 *使用须知：
 * 1. 从摇杆过来的数据应该存到变量里面，通过设置间隔将变量里面的数据组包到RC_override_t中发出。
 * 2. IsPointMode()需要重新实现一下。判断是定点模式的时候
 * 3. 前后左右都转化为速度，与老铁彭一下速度的范围，做一下线性映射。
 * 4. 油门与滑块的行程对应。对应的是上升和下降速度。
 * 5. 转头是转动摇杆。将摇杆的数据映射到转动的角速度上，与老铁对一下协议。
 * 6. 每一项添加一个归零处理，可以认为输出的原始值的绝对值<1000，摇杆没有输出。
*/
class JoyStickToFlyController : public QObject
{
    Q_OBJECT
public:
    explicit JoyStickToFlyController(QObject *parent = 0);
public:
        bool isOpened();

        void startPolling(void);

        void stopPolling(void);

signals:

public slots:
        void joystick_Front(float frontspeed);//-9000~9000

        void joystick_Left(float leftspeed);//-9000~9000

        void joystick_HeightUp(float updown);//-9000~9000

        void joystick_yaw(float yawspeed);//-9000~9000

        void joystick_control(int front, int left, int up, int yaw);

        void joystick_button1_2(int button1, int button2);
public:
         FancyJoyStick* m_joyStick;
         int            m_roll_l;
         int            m_yaw_l;
         int            m_pitch_l;
         int            m_thro_l;
         bool           m_bOpen;

};

#endif // JOYSTICKTOFLYCONTROLLER_H

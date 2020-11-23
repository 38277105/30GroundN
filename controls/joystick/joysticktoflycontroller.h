#ifndef JOYSTICKTOFLYCONTROLLER_H
#define JOYSTICKTOFLYCONTROLLER_H

#include <QObject>
#include "fancyjoystick.h"
#include "controllers/frmmaincontroller.h"
/*
 *ʹ����֪��
 * 1. ��ҡ�˹���������Ӧ�ô浽�������棬ͨ�����ü����������������������RC_override_t�з�����
 * 2. IsPointMode()��Ҫ����ʵ��һ�¡��ж��Ƕ���ģʽ��ʱ��
 * 3. ǰ�����Ҷ�ת��Ϊ�ٶȣ���������һ���ٶȵķ�Χ����һ������ӳ�䡣
 * 4. �����뻬����г̶�Ӧ����Ӧ�����������½��ٶȡ�
 * 5. תͷ��ת��ҡ�ˡ���ҡ�˵�����ӳ�䵽ת���Ľ��ٶ��ϣ���������һ��Э�顣
 * 6. ÿһ�����һ�����㴦��������Ϊ�����ԭʼֵ�ľ���ֵ<1000��ҡ��û�������
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

#include "fancyjoystick.h"
#include "qdebug.h"
#include <QFile>
#include <QDomDocument>
#include <QTextStream>
#include <QDomNode>
#include <QTextCodec>
#include <QDir>
#include "joysticktoflycontroller.h"
FancyJoyStick::FancyJoyStick(const QString& name, int axisCount, int buttonCount, int sdlIndex, JoyStickToFlyController* pJstfC)
    :_sdlIndex(sdlIndex)
    , _exitThread(false)
    , _name(name)
    , _axisCount(axisCount)
    , _buttonCount(buttonCount)
    ,jstfC(pJstfC)
{
  _MsgHandle=NULL;
  m_maxNumOfNegativeAxis = 32768;
  m_maxNumOfAxis = 32768*2;
}

FancyJoyStick::~FancyJoyStick()
{

}


float FancyJoyStick::_adjustRange(int value, Calibration_t calibration)
{
    float valueNormalized;
    float axisLength;
    float axisBasis;

    if (value > calibration.center)
    {
        axisBasis = 1.0f;
        valueNormalized = value - calibration.center;
        axisLength =  calibration.max - calibration.center;
    } else {
        axisBasis = -1.0f;
        valueNormalized = calibration.center - value;
        axisLength =  calibration.center - calibration.min;
    }

    float axisPercent = valueNormalized / axisLength;

    float correctedValue = axisBasis * axisPercent;

    if (calibration.reversed) {
        correctedValue *= -1.0f;
    }

    return std::max(-1.0f, std::min(correctedValue, 1.0f));
}

 void FancyJoyStick::run(void)
{
    SDL_Joystick* sdlJoystick = SDL_JoystickOpen(_sdlIndex);

    if (!sdlJoystick) {
        qDebug() << "SDL_JoystickOpen failed:" << SDL_GetError();
        return;
    }
    loadconfig();

    while (!_exitThread) {
        SDL_JoystickUpdate();

        bool IsThrottleState=false;
        // Update buttons
//        for (int buttonIndex=0; buttonIndex<_buttonCount; buttonIndex++) {
//            bool newButtonValue = !!SDL_JoystickGetButton(sdlJoystick, buttonIndex);
//            if(buttonIndex==0&&newButtonValue==true)
//            {
//                IsThrottleState=true;
//            }
//             //emit rawButtonPressedChanged(buttonIndex, newButtonValue);
//            //qDebug()<<"button_key:"<<buttonIndex<<"  button_value:"<<newButtonValue;
//        }
        int button1 = SDL_JoystickGetButton(sdlJoystick, 0);
        int button2 = SDL_JoystickGetButton(sdlJoystick, 1);
        int roll,pitch,yaw,throttle;
        roll = SDL_JoystickGetAxis(sdlJoystick, _AxisMaps["ROLL"]);
        pitch= SDL_JoystickGetAxis(sdlJoystick, _AxisMaps["PITCH"]);
        yaw  = SDL_JoystickGetAxis(sdlJoystick, _AxisMaps["YAW"]);
        throttle  = SDL_JoystickGetAxis(sdlJoystick, _AxisMaps["THRO"]);

        //normalized to -1~1;
        float roll_percent = (roll+m_maxNumOfNegativeAxis)/(m_maxNumOfAxis*1.0);
        float pitch_percent = (pitch+m_maxNumOfNegativeAxis)/(m_maxNumOfAxis*1.0);
        float yaw_percent = (yaw+m_maxNumOfNegativeAxis)/(m_maxNumOfAxis*1.0);
        float throttle_percent = (throttle+m_maxNumOfNegativeAxis)/(m_maxNumOfAxis*1.0);

        int roll_l,pitch_l,yaw_l,throttle_l;
        if(roll_l != roll){
            qDebug() << "roll: " << roll << "  roll_percent: " << roll_percent;
            roll_l = roll;
        }
        if(pitch_l != pitch){
            qDebug() << "pitch: " << pitch<< "  pitch_percent: " << pitch_percent;
            pitch_l = pitch;
        }
        if(yaw_l != yaw){
            qDebug() << "yaw: " << yaw<< "  yaw_percent: " << yaw_percent;
            yaw_l = yaw;
        }
        if(throttle_l != throttle){
            qDebug() << "throttle: " << throttle<< "  throttle_percent: " << throttle_percent;
            throttle_l = throttle;
        }
//        if(std::abs(roll_percent)<=0.08f)
//        {
//            roll_percent=0.0f;
//        }
//        if(std::abs(pitch_percent)<=0.08f)
//        {
//            pitch_percent=0.0f;
//        }
//        if(std::abs(yaw_percent)<=0.08f)
//        {
//            yaw_percent=0.0f;
//        }
//         roll_percent=std::max(-1.0f,std::min(roll_percent,1.0f));
//         pitch_percent=std::max(-1.0f,std::min(pitch_percent,1.0f));
//         yaw_percent=std::max(-1.0f,std::min(yaw_percent,1.0f));
//         throttle_percent=std::max(-1.0f,std::min(throttle_percent,1.0f));

         if(0.0 <= roll_percent && roll_percent < 3.5){
            m_roll = _AxisMaps["ROLL_MIN"];

         }else if(3.5 <= roll_percent && roll_percent < 7.5){
            m_roll = _AxisMaps["ROLL_CENTER"];

         }else if(7.5 <= roll_percent && roll_percent <= 10.0){
            m_roll = _AxisMaps["ROLL_MAX"];

         }

         if(0.0 <= pitch_percent && pitch_percent < 3.5){
            m_pitch = _AxisMaps["PITCH_MIN"];

         }else if(3.5 <= pitch_percent && pitch_percent < 7.5){
            m_pitch = _AxisMaps["PITCH_CENTER"];

         }else if(7.5 <= pitch_percent && pitch_percent <= 10.0){
            m_pitch = _AxisMaps["PITCH_MAX"];
         }

         if(0.0 <= yaw_percent && yaw_percent < 3.5){
            m_yaw = _AxisMaps["YAW_MIN"];

         }else if(3.5 <= yaw_percent && yaw_percent < 7.5){
            m_yaw = _AxisMaps["YAW_CENTER"];

         }else if(7.5 <= yaw_percent && yaw_percent <= 10.0){
            m_yaw = _AxisMaps["YAW_MAX"];

         }

         if(0.0 <= throttle_percent && throttle_percent < 3.5){
            m_thro = _AxisMaps["THRO_MAX"];

         }else if(3.5 <= throttle_percent && throttle_percent < 7.5){
            m_thro = _AxisMaps["THRO_CENTER"];

         }else if(7.5 <= throttle_percent && throttle_percent <= 10.0){
            m_thro = _AxisMaps["THRO_MIN"];

         }

//         emit joystick_yaw(m_yaw);
//         emit joystick_Front(m_pitch);
//         emit joystick_Left(m_roll);
//         emit joystick_HeightUp(m_thro);

//         jstfC->joystick_yaw(m_yaw);
//         jstfC->joystick_Front(m_pitch);
//         jstfC->joystick_Left(m_roll);
//         jstfC->joystick_HeightUp(m_thro);

         jstfC->joystick_control(m_pitch, m_roll, m_thro, m_yaw);
         jstfC->joystick_button1_2(button1, button2);
        // Sleep, update rate of joystick is approx. 25 Hz (1000 ms / 25 = 40 ms)
        QThread::msleep(20);
    }

    SDL_JoystickClose(sdlJoystick);
}

 void FancyJoyStick::fstart(){
    _exitThread=false;
    QThread::start();
 }

 void FancyJoyStick::stop()
 {
    _exitThread=true;
 }

 void FancyJoyStick::loadconfig()
 {


     QDir current;
     QString path=current.currentPath();
     QFile file("./data/joystick_config.xml");
     qDebug()<<path+"/data/joystick_config.xml"<<"------------------------";
     if(!file.open(QFile::ReadOnly))
     {

         //QMessageBox::information(NULL, QString("title"), QString("open error!"));
        qDebug() << file.errorString();
     }

     QTextStream vReadStream(&file);
     QTextCodec *vCodec = QTextCodec::codecForName("GBK");
     vReadStream.setCodec(vCodec);
     QString vXmlDataStr = vReadStream.readAll();
     file.close();


     QDomDocument document;



     if(!document.setContent(vXmlDataStr.toLocal8Bit()))
     {
         //QMessageBox::information(NULL, QString("title"), QString("parse file failed "));
     }
     if(document.isNull())
     {
         //QMessageBox::information(NULL, QString("title"), QString("document is null!"));
     }
     QDomElement root = document.documentElement();
     QDomNode settingItem = root.firstChild();
     while(!settingItem.isNull())
     {
         _AxisMaps[settingItem.toElement().attribute("Key")]=settingItem.toElement().attribute("Value").toInt();
         qDebug()<<settingItem.toElement().attribute("Key")<<settingItem.toElement().attribute("Value").toInt();
         settingItem=settingItem.nextSibling();
     }
 }

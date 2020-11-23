#include "wpcmdcbo.h"
#include <qdebug.h>
wpcmdCbo::wpcmdCbo(QWidget *parent):QComboBox(parent)
{
    initCommonMap();
    this->addItems(m_cmdList);
    this->setCurrentIndex(1);
    m_state = 16;
    connect(this, SIGNAL(currentIndexChanged(QString)), this, SLOT(CboValueChanged(QString)));
}

void wpcmdCbo::initItems(const int& cItem){
    this->addItems(m_cmdList);
    this->setCurrentIndex(this->findText(m_cmdMap.value(cItem)));
    connect(this, SIGNAL(currentIndexChanged(QString)), this, SLOT(CboValueChanged(QString)));
}

int wpcmdCbo::getCurrentIndex(){
    return m_state;
}

int wpcmdCbo::SetNowIndex(int index){
    m_state = index;
    this->setCurrentText(m_cmdMap.value(index));
    return true;
}

void wpcmdCbo::initCommonMap(){
    m_cmdMap.insert(16, "WAYPOINT");
    m_cmdMap.insert(17, "LOITER_UNLIM");
    m_cmdMap.insert(18, "LOITER_TURNS");
    m_cmdMap.insert(19, "LOITER_TIME");
    m_cmdMap.insert(31, "LOITER_TO_ALT");
    m_cmdMap.insert(20, "RETURN_TO_LAUNCH");
    m_cmdMap.insert(21, "LAND");
    m_cmdMap.insert(22, "TAKEOFF");
    m_cmdMap.insert(84, "VTOL_TAKEOFF");
    m_cmdMap.insert(85, "VTOL_LAND");
    m_cmdMap.insert(3000, "DO_VTOL_TRANSITION");
    m_cmdMap.insert(30, "CONTINUE_AND_CHANGE_ALT");
    m_cmdMap.insert(112, "CONDITION_DELAY");
    m_cmdMap.insert(113, "CONDITION_CHANGE_ALT");
    m_cmdMap.insert(114, "CONDITION_DISTANCE");
    m_cmdMap.insert(115, "CONDITION_YAW");
    m_cmdMap.insert(189, "DO_LAND_START");
    m_cmdMap.insert(201, "DO_SET_ROI");
    m_cmdMap.insert(210, "DO_INVERTED_FLIGHT");
    m_cmdMap.insert(176, "DO_SET_MODE");
    m_cmdMap.insert(206, "DO_SET_CAM_TRIGG_DIST");
    m_cmdMap.insert(177, "DO_JUMP");
    m_cmdMap.insert(178, "DO_CHANGE_SPEED");
    m_cmdMap.insert(179, "DO_SET_HOME");
    m_cmdMap.insert(181, "DO_SET_RELAY");
    m_cmdMap.insert(182, "DO_REPEAT_RELAY");
    m_cmdMap.insert(183, "DO_SET_SERVO");
    m_cmdMap.insert(184, "DO_REPEAT_SERVO");
    m_cmdMap.insert(202, "DO_DIGICAM_CONFIGURE");
    m_cmdMap.insert(203, "DO_DIGICAM_CONTROL");
    m_cmdMap.insert(204, "DO_MOUNT_CONFIGURE");
    m_cmdMap.insert(205, "DO_MOUNT_CONTROL");
    m_cmdMap.insert(208, "DO_PARACHUTE");
    m_cmdMap.insert(0, "UNKNOW");
    {
        for(QMap<int, QString>::iterator iter = m_cmdMap.begin(); iter != m_cmdMap.end(); iter++){
            m_cmdList << iter.value();
        }
    }
}

void wpcmdCbo::CboValueChanged(QString text){
    m_state = m_cmdMap.key(text);
}


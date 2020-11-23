#ifndef INFOMONITOR_H
#define INFOMONITOR_H

#include <QWidget>


union UnExtendinfo
{
    uint32_t info;
    struct
    {
        unsigned reserved1 : 2;
        unsigned hGPS : 1;         //水平定位用GPS
        unsigned reserved2 : 1;
        unsigned yawMag : 1;      //用磁定向
        unsigned reserved3 : 4;
        unsigned vAcc : 1;        //垂直定位用气压计
        unsigned reserved4 : 1;
        unsigned vGPS : 1;        //垂直定位用GPS
        unsigned hVison : 1;      //水平定位用视觉
        unsigned yawRTK : 1;      //用RTK定向
        unsigned vVison : 1;      //垂直定位用视觉
        unsigned reserved5 : 1;
        unsigned angleRTK : 16;     //RTK角度
    };
};

namespace Ui {
class InfoMonitor;
}
class QLabel;
class QImageLabel;
#define MAX_NUM 12
class InfoMonitor : public QWidget
{
    Q_OBJECT

public:
    explicit InfoMonitor(QWidget *parent = nullptr);
    ~InfoMonitor();
    void setStatus(uint32_t sys_info1,uint32_t sys_info2,uint16_t vdop,uint16_t hdop,int32_t h);
private:
    Ui::InfoMonitor *ui;
};

#endif // INFOMONITOR_H

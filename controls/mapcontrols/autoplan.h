#ifndef AUTOPLAN_H
#define AUTOPLAN_H

#include <QWidget>
#include "../MapWidget.h"
namespace Ui {
class AutoPlan;
}

typedef struct {
    QString         strVD;
    QString         strHD;
    QString         strAngle;
}APInfo;

class AutoPlan : public QWidget
{
    Q_OBJECT

public:
    explicit AutoPlan();
    ~AutoPlan();
private:
    void autoPlan_initTablePointsHead();
    bool autoPlan_isNum(const APInfo&);
signals:
    void autoPlan_finish_signal();
    void autoPlan_commitBoundaryData_signal(const QList<MapPointData>&);
    void autoPlan_updateInfoNums_siganl(const APInfo&);
    void autoPlan_clearBoundary_signal(const bool&);
    void autoPlan_isEnableClicke_signal(const bool&);
    void autoPlan_eraseOnePoint(const int&);
    void autoPlan_enableButtons();
private slots:
    void autoPlan_addOnePoint_slots(bool flag);
    void autoPlan_deleteOnePoint_slots(bool flag);
    void autoPlan_okSlo_slots(bool flag);
    void autoPlan_cancel_slots(bool flag);
    void autoPlan_resizeColumn_slots(int, int);
    void autoPlan_commit_slots(bool);
    void autoPlan_InfoEditFinished_slots();
    void autoPlan_isEnableClick_slots(const bool&);
    void autoPlan_mouseAddOnePoint_slots(const MapPointData&);
private:
    Ui::AutoPlan*                ui;
    QList<MapPointData>      m_boundaryData;
    QString                      m_horizonalDistance;
    QString                      m_verticalDistance;
    QString                      m_angle;
    bool                         m_isCommited;
};

#endif // AUTOPLAN_H

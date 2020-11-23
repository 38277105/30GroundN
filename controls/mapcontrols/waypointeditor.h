#ifndef WAYPOINTEDITOR_H
#define WAYPOINTEDITOR_H

#include <QWidget>

namespace Ui {
class WayPointEditor;
}

class WayPointEditor : public QWidget
{
    Q_OBJECT

public:
    explicit WayPointEditor(QWidget *parent = 0);
    ~WayPointEditor();
    void SetNumber(const double& lng, const double& lat, const double& alt);
private slots:
    void on_m_btn_ok_clicked();
    void on_m_btn_cancel_clicked();
signals:
    void transferNums(double lng, const double lat, const double alt);
private:
    double              m_longitude;
    double              m_latitude;
    double              m_altitude;
private:
    Ui::WayPointEditor *ui;
};

#endif // WAYPOINTEDITOR_H

#ifndef WARNINGFORM_H
#define WARNINGFORM_H

#include <stdint.h>
#include <QWidget>

namespace Ui {
class WarningForm;
}

class WarningForm : public QWidget
{
    Q_OBJECT

public:
    explicit WarningForm(QWidget *parent = 0);
    ~WarningForm();

    void UpdateStatus(uint32_t sys_sts, uint16_t gps_sts,uint16_t rtk_sts,uint16_t psirtk_sts);
    QString getMsg(uint16_t nSts);
private:
    Ui::WarningForm *ui;
};

#endif // WARNINGFORM_H

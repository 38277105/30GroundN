#ifndef critActInfo_H
#define critActInfo_H

#include <QWidget>
#include "../../controllers/frmmaincontroller.h"

namespace Ui {
class critActInfo;
}

class critActInfo : public QWidget
{
    Q_OBJECT

public:
    explicit critActInfo(QWidget *parent = 0);
    ~critActInfo();

private slots:

    void on_btn_readCritAct_clicked();

    void on_btn_readPID_clicked();

    void on_btn_readPosition_clicked();

    void on_btn_setCritAct_clicked();

    void on_btn_setPosition_clicked();

    void on_btn_setPID_clicked();

    void on_btn_resetCritAct_clicked();

    void on_btn_resetPosition_clicked();

    void on_btn_resetPID_clicked();

public slots:

    void updateCritActInfo(char *id, float value);

private:
    Ui::critActInfo *ui;
};

#endif // critActInfo_H

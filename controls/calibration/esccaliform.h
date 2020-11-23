#ifndef ESCCALIFORM_H
#define ESCCALIFORM_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class ESCCaliForm;
}

class ESCCaliForm : public QWidget
{
    Q_OBJECT

public:
    explicit ESCCaliForm(QWidget *parent = 0);
    ~ESCCaliForm();

private slots:
    void on_btn_ESCCali_clicked();

private:
    Ui::ESCCaliForm *ui;

public:
    QTimer m_timer;
    qint8  m_setCount;
    bool   m_bIsCalibrating;
public slots:
    void timer_tick();
};

#endif // ESCCALIFORM_H

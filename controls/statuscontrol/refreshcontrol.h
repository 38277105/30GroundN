#ifndef REFRESHCONTROL_H
#define REFRESHCONTROL_H

#include <QWidget>

namespace Ui {
class RefreshControl;
}

class RefreshControl : public QWidget
{
    Q_OBJECT

public:
    explicit RefreshControl(QWidget *parent = 0);
    ~RefreshControl();

private slots:
    void on_toolButton_clicked();

public:
    Ui::RefreshControl *ui;
};

#endif // REFRESHCONTROL_H

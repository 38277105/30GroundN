#ifndef PARAMRIGHTCONTROL_H
#define PARAMRIGHTCONTROL_H

#include <QWidget>

namespace Ui {
class ParamRightControl;
}

class ParamRightControl : public QWidget
{
    Q_OBJECT

public:
    explicit ParamRightControl(QWidget *parent = 0);
    ~ParamRightControl();

    Ui::ParamRightControl *ui;


private:

};

#endif // PARAMRIGHTCONTROL_H

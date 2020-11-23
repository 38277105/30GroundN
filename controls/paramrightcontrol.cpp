#include "paramrightcontrol.h"
#include "ui_paramrightcontrol.h"

ParamRightControl::ParamRightControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParamRightControl)
{
    ui->setupUi(this);
}

ParamRightControl::~ParamRightControl()
{
    delete ui;
}



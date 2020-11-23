#include "refreshcontrol.h"
#include "ui_refreshcontrol.h"

RefreshControl::RefreshControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RefreshControl)
{
    ui->setupUi(this);
}

RefreshControl::~RefreshControl()
{
    delete ui;
}

void RefreshControl::on_toolButton_clicked()
{

}

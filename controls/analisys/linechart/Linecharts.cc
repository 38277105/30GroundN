#include <QShowEvent>

#include "Linecharts.h"
//#include "MultiVehicleManager.h"
//#include "MainWindow.h"
//#include "UAS.h"

Linecharts::Linecharts(const QString& title, QAction* action, MAVLinkDecoder* decoder, QWidget *parent)
    : QWidget(parent)
    , _mavlinkDecoder(decoder)
{
   // init();

    this->setVisible(true);
    this->_newVehicleWidget(this);

}

QWidget* Linecharts::_newVehicleWidget( QWidget* parent)
{
    LinechartWidget* widget = new LinechartWidget( parent);
    widget->setBaseSize(640,480);

    // Connect valueChanged signals
    //connect(vehicle->uas(), &UAS::valueChanged, widget, &LinechartWidget::appendData);

    // Connect decoder
    connect(_mavlinkDecoder, &MAVLinkDecoder::valueChanged, widget, &LinechartWidget::appendData);

    // Select system
    widget->setActive(true);

    return widget;
}

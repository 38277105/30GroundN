/********************************************************************************
** Form generated from reading UI file 'mapripform.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPRIPFORM_H
#define UI_MAPRIPFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MapRipForm
{
public:
    QProgressBar *progressBar;
    QLabel *mainlabel;
    QLabel *statuslabel;
    QPushButton *cancelButton;

    void setupUi(QWidget *MapRipForm)
    {
        if (MapRipForm->objectName().isEmpty())
            MapRipForm->setObjectName(QStringLiteral("MapRipForm"));
        MapRipForm->resize(492, 151);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MapRipForm->sizePolicy().hasHeightForWidth());
        MapRipForm->setSizePolicy(sizePolicy);
        progressBar = new QProgressBar(MapRipForm);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(20, 70, 451, 23));
        progressBar->setValue(0);
        mainlabel = new QLabel(MapRipForm);
        mainlabel->setObjectName(QStringLiteral("mainlabel"));
        mainlabel->setGeometry(QRect(20, 10, 451, 16));
        statuslabel = new QLabel(MapRipForm);
        statuslabel->setObjectName(QStringLiteral("statuslabel"));
        statuslabel->setGeometry(QRect(20, 40, 451, 16));
        cancelButton = new QPushButton(MapRipForm);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));
        cancelButton->setGeometry(QRect(400, 110, 75, 23));

        retranslateUi(MapRipForm);
        QObject::connect(cancelButton, SIGNAL(clicked()), MapRipForm, SLOT(close()));

        QMetaObject::connectSlotsByName(MapRipForm);
    } // setupUi

    void retranslateUi(QWidget *MapRipForm)
    {
        MapRipForm->setWindowTitle(QApplication::translate("MapRipForm", "MapRipper", 0));
        mainlabel->setText(QApplication::translate("MapRipForm", "Currently ripping from:", 0));
        statuslabel->setText(QApplication::translate("MapRipForm", "Downloading tile", 0));
        cancelButton->setText(QApplication::translate("MapRipForm", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class MapRipForm: public Ui_MapRipForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPRIPFORM_H

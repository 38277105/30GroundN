#ifndef INFOVIEWITEM_H
#define INFOVIEWITEM_H

#include <QWidget>
#include <QLabel>
#include "Utils/zygroundglobalconfig.h"

class InfoViewItem : public QWidget
{
    Q_OBJECT
public:
    explicit InfoViewItem(QString title,QWidget *parent = 0);

    void SetValue(QString value);
    void SetTitle(QString title);
    void SetRange(float min,float max);
signals:

public slots:
private:
     QLabel* lbl_title;
     QLabel* lbl_value;
     float   m_nMinVal;
     float   m_nMaxVal;
};

#endif // INFOVIEWITEM_H

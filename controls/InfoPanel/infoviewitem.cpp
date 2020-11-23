#include "infoviewitem.h"

InfoViewItem::InfoViewItem(QString title,QWidget *parent) : QWidget(parent)
{

      lbl_title=new QLabel(this);
      QFont itmFont;
      //itmFont.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
      itmFont.setFamily(QString("Arial"));
      itmFont.setPointSize(10);
      lbl_title->setFont(itmFont);
      lbl_title->setGeometry(QRect(10, 0, 71, 21));
      int autoWidth=141;
      int autoleft=50;
      if(!ZYGroundGlobalConfig::RESOLUTION_1920)
      {
            autoWidth=120;
            autoleft=50;
      }
      else
      {
            autoWidth=143;
            autoleft=60;
      }
      this->setMaximumWidth(autoWidth);
      this->setMinimumWidth(autoWidth);
      this->setMinimumHeight(25);
      lbl_value=new QLabel(this);
      lbl_value->setFont(itmFont);
      lbl_value->setGeometry(QRect(autoleft, 0, 110, 21));
      SetTitle(title);

      m_nMaxVal=-1;
      m_nMinVal=-1;
}

void InfoViewItem::SetValue(QString value)
{
   lbl_value->setText(value);
   if(m_nMaxVal==-1 && m_nMinVal==-1)
       return;
   float val=value.toFloat();
   if(val <m_nMinVal || val> m_nMaxVal)
       lbl_title->setStyleSheet("background-color: rgb(255, 0, 0);");
   else
       lbl_title->setStyleSheet("background-color: rgb(0, 255, 0);");
}

void InfoViewItem::SetRange(float min, float max)
{
    m_nMaxVal=max;
    m_nMinVal=min;
}

void InfoViewItem::SetTitle(QString title)
{
  lbl_title->setText(title);
}

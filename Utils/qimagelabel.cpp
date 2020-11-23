#include "qimagelabel.h"
#include "./zygroundglobalconfig.h"
#include <QDebug>
#include <QThread>

QImageLabel::QImageLabel(QWidget *parent):
    QLabel(parent)
{
    m_strImagePath.clear();
    m_strText.clear();
    m_fontsize = 32;
    m_pen.setColor(qRgb(233, 233, 233));
}

void QImageLabel::paintEvent(QPaintEvent *event)
{
    QLabel::setText("");
    m_width = this->width();
    m_height = this->height();
    m_fitpixmap = m_pixmap.scaled(m_width, m_height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    if(m_fitpixmap.isNull())
    {
        return;
    }
    QPainter painter;
    painter.begin(&m_fitpixmap);
    painter.setPen(m_pen);
    painter.setFont(m_font);
    painter.drawText(0, 0, m_width, m_height, Qt::AlignCenter, m_strText);
    painter.end();

    painter.begin(this);
    painter.drawPixmap(0,0,m_width, m_height,m_fitpixmap);
    painter.end();
    QLabel::paintEvent(event);
}

void QImageLabel::setImage(QString strImagePath)
{
	if (m_strImagePath == strImagePath)
	{
		return;
	}
    m_strImagePath = strImagePath;

    m_Image.load(m_strImagePath);
    if (m_Image.isNull())
    {
        return;
    }
    m_pixmap = QPixmap::fromImage(m_Image);
	
}

void QImageLabel::setFontSize(qint32 fontsize)
{
    m_fontsize = fontsize;
    m_font.setFamily(tr("黑体"));
    m_font.setPointSize(m_fontsize);
}

void QImageLabel::setLabelText(QString strText)
{
	if (m_strText == strText)
	{
		return;
	}
    m_strText = strText;
}

void QImageLabel::setText(QString strText)
{
    setLabelText(strText);
}

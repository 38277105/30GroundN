#ifndef QIMAGELABEL_H
#define QIMAGELABEL_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QTime>

class QImageLabel : public QLabel
{
    Q_OBJECT
public:
    QImageLabel(QWidget *parent = 0);

    void setImage(QString strImagePath);

    void setFontSize(qint32 fontsize);

    void setLabelText(QString strText);

    void setText(QString strText);
    inline void setTextColor(const QRgb& color){
        m_pen.setColor(color);
    }

protected:
    void paintEvent(QPaintEvent *event);
public:
    QString m_strImagePath;
    QString m_strText;
    qint32  m_fontsize;
    QImage  m_Image;
    QPixmap m_pixmap;
    QPixmap m_fitpixmap;
    int      m_width;
    int      m_height;
    QPen    m_pen;
    QFont   m_font;
};

#endif // QIMAGELABEL_H

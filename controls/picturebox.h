#ifndef PICTUREBOX_H
#define PICTUREBOX_H

#include <QWidget>  
#include <QImage>  
#include <QPixmap>
#include <QMouseEvent>


class PictureBox : public QWidget  
{      
    Q_OBJECT
public:  
    enum PB_MODE {FIXED_SIZE, FIX_SIZE_CENTRED, AUTO_ZOOM, AUTO_SIZE};
    explicit PictureBox(QWidget *parent = 0);
    void setMode(PB_MODE mode);
    ~PictureBox();
    bool IsSelected;
private:  
    QPixmap m_pixmap;
    double m_scale;
    PB_MODE m_mode;
    QBrush m_brush;
    bool mousePressed;
protected:  
    void paintEvent(QPaintEvent * event);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
signals:  

public slots:  
    bool setImage(QImage &image, double scale = 1.0);
    void setBackground(QBrush brush);
    //Add by xingjiwei
    void setSelected();
};  

#endif // PICTUREBOX_H

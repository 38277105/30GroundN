#ifndef ZOOMCHANGEITEM_H
#define ZOOMCHANGEITEM_H

#include <QGraphicsPixmapItem>
#include <QObject>

namespace mapcontrol {

enum ZoomChangeType{
    ZOOMUP,
    ZOOMDOWN
};

class ZoomChangeItem : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    ZoomChangeItem();
    void setType(ZoomChangeType type);
signals:
    void signal_zoomChang(ZoomChangeType);
protected:
    void mousePressEvent ( QGraphicsSceneMouseEvent * event );
private:
    QPixmap picture;
    ZoomChangeType m_type;
};

} // end of namespace mapcontrol

#endif // ZOOMCHANGEITEM_H

#include "zoomChangeItem.h"
#include <QGraphicsSceneMouseEvent>

namespace mapcontrol {

ZoomChangeItem::ZoomChangeItem()
{
	
}

void ZoomChangeItem::setType(ZoomChangeType type){
    m_type = type;
    if( m_type == ZOOMUP){

    }else if(m_type == ZOOMDOWN){

    }
}

void ZoomChangeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        emit signal_zoomChang(m_type);
    }
}







} // end of namespace mapcontrol

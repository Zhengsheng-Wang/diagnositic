#include <cmath>
#include <QtMath>

#include "arrow.h"

//! [0]
/*
We set the source point as the median point of bottom edge of the source element block and,
the destination point as the median point of upper edge of the destination element block.
We set it selecable.
*/
Arrow::Arrow(Element *source, Element *dest):
    source(source), dest(dest){
    QRectF source_rect = source->boundingRect(), dest_rect = dest->boundingRect();
    source_point = mapFromItem(source, 0., source_rect.height() / 2.);
    dest_point = mapFromItem(dest, 0., - dest_rect.height() / 2.);
    
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}
//! [0]

//! [1]
/*
*/
QRectF Arrow::boundingRect() const{
    return QRectF(source_point, QSizeF(dest_point.x() - source_point.x(), 
                                       dest_point.y() - source_point.y())).
           normalized();
}
//! [1]

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setPen(QPen(Qt::black, arrow_width));

    QLineF line(source_point, dest_point);
    painter->drawLine(line);

    // Draw arrow head.
    qreal head_size = 5.;
    double angle = std::atan2(-line.dy(), line.dx());
    QPointF p1 = dest_point + QPointF(head_size * std::sin(angle - M_PI / 3), 
                                      head_size * std::cos(angle - M_PI / 3));
    QPointF p2 = dest_point + QPointF(head_size * std::sin(angle - M_PI + M_PI / 3), 
                                      head_size * std::cos(angle - M_PI + M_PI / 3));
    painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF() << dest_point << p1 << p2);
}
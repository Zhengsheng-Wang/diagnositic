#include <cmath>
#include <QtMath>

#include "arrow.h"

Arrow::Arrow(Element *source, Element *dest):
    source(source), dest(dest){
    QRectF source_rect = source->boundingRect(), dest_rect = dest->boundingRect();
    sourcePoint = mapFromItem(source, 0., source_rect.height()/2.);
    destPoint = mapFromItem(dest, 0., -dest_rect.height()/2.);
    
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

QRectF Arrow::boundingRect() const{
    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(), 
                                       destPoint.y() - sourcePoint.y())).
           normalized();
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setPen(QPen(Qt::black, arrowWidth));

    QLineF line(sourcePoint, destPoint);
    painter->drawLine(line);

    // Draw arrow head.
    qreal head_size = 5.;
    double angle = std::atan2(-line.dy(), line.dx());
    QPointF p1 = destPoint + QPointF(head_size * std::sin(angle - M_PI / 3), 
                                      head_size * std::cos(angle - M_PI / 3));
    QPointF p2 = destPoint + QPointF(head_size * std::sin(angle - M_PI + M_PI / 3), 
                                      head_size * std::cos(angle - M_PI + M_PI / 3));
    painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF() << destPoint << p1 << p2);
}
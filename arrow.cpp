#include <cmath>
#include <QtMath>

#include "arrow.h"

Arrow::Arrow(Element *source, Element *dest):
    source(source), dest(dest)
{
    QRectF source_rect = source->boundingRect(), dest_rect = dest->boundingRect();
    source_point = mapFromItem(source, 0., source_rect.height() / 2.);
    dest_point = mapFromItem(dest, 0., - dest_rect.height() / 2.);
}

QRectF Arrow::boundingRect() const{
    return QRectF(source_point, dest_point);
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setPen(QPen(Qt::black, arrow_width));

    QLineF line(source_point, dest_point);
    painter->drawLine(line);

    // Draw arrow head.
    qreal head_size = 10.;
    QPointF p1 = dest_point + QPointF(head_size * std::sin(M_PI / 6), -head_size * std::cos(M_PI / 6));
    QPointF p2 = dest_point + QPointF(-head_size * std::sin(M_PI / 6), -head_size * std::cos(M_PI / 6));
    painter->drawLine(dest_point, p1);
    painter->drawLine(dest_point, p2);
}
#include "element.h"
#include <QGraphicsItem>

Element::Element(Diagram *profile, const QString &spu_name, qreal w, qreal h):
    profile(profile), spu_name(spu_name), width(w), height(h){
}

QRectF Element::boundingRect() const{
    return QRectF(- width / 2. - outline_width / 2, - height / 2. - outline_width / 2, 
        width + outline_width, height + outline_width);
}


void Element::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setRenderHint(QPainter::Antialiasing);

    // Draw shape.
    painter->setPen(QPen(Qt::black, outline_width));
    QRect rect(- width / 2., - height / 2., width, height);
    painter->drawRoundedRect(rect, 10.0, 10.0, Qt::RelativeSize);
    QFont font("宋体");
    font.setPixelSize(15);
    painter->setFont(font);
    painter->drawText(rect, Qt::AlignCenter, spu_name);
}

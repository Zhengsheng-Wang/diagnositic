#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsItem>

#include "element.h"

class Arrow: public QGraphicsItem{
public:
    /*
    We set the source point as the median point of bottom edge of the source element block and,
    the destination point as the median point of upper edge of the destination element block.
    We set it selecable.
    */
    Arrow(Element *source, Element *dest);

    enum {Type = UserType + 2};
    int type() const override{
        return Type;
    }

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
private:
    Element *source, *dest;
    QPointF sourcePoint, destPoint;
    int arrowWidth = 2;
};


#endif 
#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsItem>
#include "element.h"

class Arrow: public QGraphicsItem{
public:
    Arrow(Element *source, Element *dest);

    enum {Type = UserType + 2};
    int type() const override{
        return Type;
    }

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
private:
    Element *source, *dest;
    QPointF source_point, dest_point;
    int arrow_width = 2;
};


#endif 
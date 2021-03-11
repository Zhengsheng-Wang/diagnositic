#ifndef ELEMENT_H
#define ELEMENT_H

#include <QGraphicsItem>
#include "diagram.h"

class Diagram;

class Element: public QGraphicsItem{
public:
    Element(Diagram *profile, const QString &spu_name, qreal w, qreal h);

    enum {Type = UserType + 1};
    int type() const override {return Type;}

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    // spu name.
    QString spu_name;
    // Diagram.
    Diagram *profile;
    // Order number.
    int order;
    // Size.
    qreal width, height;
    // Outline width.
    qreal outline_width = 2;
};

#endif
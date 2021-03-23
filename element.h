#ifndef ELEMENT_H
#define ELEMENT_H

#include <QGraphicsItem>

#include "diagram.h"

class Diagram;

class Element: public QGraphicsItem{
public:
    Element(Diagram *s, const QString &name, qreal w, qreal h, const QString &t);

    enum {Type = UserType + 1};
    int type() const override {return Type;}

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QString get_spu_name() const{
        return spu_name;
    }
    QString get_element_name() const{
        return element_name;
    }

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    // spu name.
    QString spu_name;
    // Diagram.
    Diagram *system;
    // Order number.
    int order;
    // Size.
    qreal width, height;
    // Outline width.
    qreal outline_width = 2;
    // Type.
    QString element_name;

public:
    static QString fault_spu_name;
};

#endif
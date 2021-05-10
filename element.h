#ifndef ELEMENT_H
#define ELEMENT_H

#include <QGraphicsItem>

#include "diagram.h"

class Diagram;

class Element: public QGraphicsItem{
public:
    /*
    Constructor.

    diagram_ -- the abstract diagram of diagnostic diagram.
    elementName_ -- the name of device or signal processing link.
    width_ -- the width of element block.
    height_ -- the height of element block.
    spuName_ -- the name of spu to which it belongs.
    */
    Element(Diagram *diagram_, const QString &elementName_, qreal width_, qreal height_, const QString &spuName_);

    enum {Type = UserType + 1};
    int type() const override {return Type;}

    /*
    Set the bounding rectangle of block in diagram.
    */
    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QString getElementName() const{
        return elementName;
    }
    QString getSPUName() const{
        return spuName;
    }

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    Diagram *diagram; // The profile of diagnostic diagram.

    QString elementName;  // Name of device(for device block) or name of signal processing link.

    qreal width, height; // Block size.

    QString spuName; // Name of spu to which this block belongs.

    qreal outlineWidth = 2; 

public:
    static QString faultDeviceName;  
    static QString faultSPUName;  
};

#endif
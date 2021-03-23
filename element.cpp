#include "element.h"
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

// ![0]
/*
Set the QGraphicsItem selectable.
*/
Element::Element(Diagram *s, const QString &name, qreal w, qreal h, const QString &t):
    system(s), element_name(name), width(w), height(h), spu_name(t){
        setFlag(QGraphicsItem::ItemIsSelectable, true);
}
// ![0]

// ![1]
// Initialize static spu name.
QString Element::fault_spu_name("");
// ![1]

//! [1]
/*
We set the bounding rectangle to be the original rectangle with a width of 
outline_width rendering by Antialias.
*/
QRectF Element::boundingRect() const{
    return QRectF(- width / 2. - outline_width / 2, - height / 2. - outline_width / 2, 
        width + outline_width, height + outline_width);
}
//! [1]

// ![2]
/*
We set the origin of item's local coordinate in it's center point. 
Set the text displayed on it's center.
*/
void Element::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setRenderHint(QPainter::Antialiasing);

    // Draw shape.
    // Mark the fault spu's blocks as red.
    if(spu_name == fault_spu_name){
        painter->setBrush(QBrush(QColor(Qt::red), Qt::SolidPattern));
    }
    else{
        painter->setBrush(QBrush(Qt::NoBrush));
    }
    painter->setPen(QPen(Qt::black, outline_width));
    QRect rect(- width / 2., - height / 2., width, height);
    painter->drawRoundedRect(rect, 10.0, 10.0, Qt::RelativeSize);
    QFont font("宋体");
    font.setPixelSize(15);
    painter->setFont(font);
    painter->drawText(rect, Qt::AlignCenter, element_name);
}
//! [2]

// ![3]
/*
*/
void Element::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event){
    // Get the item getting clicked.
    QGraphicsItem *item = system->scene()->itemAt(event->scenePos(), QTransform());
    Element *element = qgraphicsitem_cast<Element *>(item);

    // Reset the area.
    system->reset_scene_rect();

    // Draw detailed sub-diagram.
    system->draw_detailed_diagram(element->get_element_name(), "spu12");
}
// ![3]
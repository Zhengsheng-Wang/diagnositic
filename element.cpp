#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

#include "element.h"

// Initialize static spu name.
QString Element::faultSPUName("");
QString Element::faultDeviceName("");

Element::Element(Diagram *diagram_, const QString &elementName_, qreal width_, qreal height_, const QString &spuName_):
    diagram(diagram_), elementName(elementName_), width(width_), height(height_), spuName(spuName_){
        // Set the QGraphicsItem  selectable.
        setFlag(QGraphicsItem::ItemIsSelectable, true);
}

QRectF Element::boundingRect() const{
    return QRectF(-width/2. - outlineWidth/2., -height/2. - outlineWidth/2., 
        width + outlineWidth, height + outlineWidth);
}

void Element::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    // Set the origin of item's local coordinate as it's center point. 
    // Set the text displayed on it's center.


    // Draw shape.
    // Mark the fault signal processing links and devices as red, and draw hollow shape for normal ones.
    painter->setRenderHint(QPainter::Antialiasing);
    if(spuName == faultSPUName){
        painter->setBrush(QBrush(QColor(Qt::red), Qt::SolidPattern));
    }
    else if(elementName == faultDeviceName){
        painter->setBrush(QBrush(QColor(Qt::red), Qt::SolidPattern));
    }
    else{
        painter->setBrush(QBrush(Qt::NoBrush));
    }
    painter->setPen(QPen(Qt::black, outlineWidth));
    QRect rect(-width/2, -height/2., width, height);
    painter->drawRoundedRect(rect, 10.0, 10.0, Qt::RelativeSize);
    
    // Write the tag of block.
    QFont font("宋体");
    font.setPixelSize(15);
    painter->setFont(font);
    painter->drawText(rect, Qt::AlignCenter, elementName);
}

void Element::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event){
    //Double click a device element block showing it's spu structure on the right side.
    // Get the item getting clicked.
    QGraphicsItem *item = diagram->scene()->itemAt(event->scenePos(), QTransform());
    Element *element = qgraphicsitem_cast<Element*>(item);

    // Reset the area.
    diagram->resetSceneRect();

    // Draw detailed sub-diagram.
    diagram->drawDetailDiagram(element->getElementName());
}
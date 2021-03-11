#include <QKeyEvent>
#include <cmath>

#include "diagram.h"
#include "element.h"
#include "arrow.h"


Diagram::Diagram(QWidget *parent): QGraphicsView(parent){
    QGraphicsScene *scene = new QGraphicsScene();

    // Set dimensions.
    qreal shape_w = 100.0, shape_h = 40.0;
    qreal space_v = 20.0;
    int shape_num = 7;
    scene_w = shape_w * 3; 
    scene_h = shape_h * shape_num + space_v * (shape_num + 1);

    // Set scene
    scene->setSceneRect(0, 0, scene_w, scene_h);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);

    // Place shapes.
    QPoint center_point(scene_w / 4, space_v + shape_h / 2.);
    Element *section1 = new Element(this, QString("低噪声放大器"), shape_w, shape_h);
    scene->addItem(section1);
    section1->setPos(center_point);

    center_point.setY(center_point.y() + shape_h + space_v);
    Element *section2 = new Element(this, QString("光发射机"), 0.7 * shape_w, shape_h);
    scene->addItem(section2);
    section2->setPos(center_point);

    center_point.setY(center_point.y() + shape_h + space_v);
    Element *section3 = new Element(this, QString("光接收机"), 0.7 * shape_w, shape_h);
    scene->addItem(section3);
    section3->setPos(center_point);
    
    center_point.setY(center_point.y() + shape_h + space_v);
    Element *section4 = new Element(this, QString("射频开关"), 0.7 * shape_w, shape_h);
    scene->addItem(section4);
    section4->setPos(center_point);

    center_point.setY(center_point.y() + shape_h + space_v);
    Element *section5 = new Element(this, QString("下变频器"), 0.7 * shape_w, shape_h);
    scene->addItem(section5);
    section5->setPos(center_point);

    center_point.setY(center_point.y() + shape_h + space_v);
    Element *section6 = new Element(this, QString("中频开关"), 0.7 * shape_w, shape_h);
    scene->addItem(section6);
    section6->setPos(center_point);

    center_point.setY(center_point.y() + shape_h + space_v);
    Element *section7 = new Element(this, QString("解调器"), 0.7 * shape_w, shape_h);
    scene->addItem(section7);
    section7->setPos(center_point);

    // Linking lines.
    Arrow *arrow1_2 = new Arrow(section1, section2);
    scene->addItem(arrow1_2);
    Arrow *arrow2_3 = new Arrow(section2, section3);
    scene->addItem(arrow2_3);
    Arrow *arrow3_4 = new Arrow(section3, section4);
    scene->addItem(arrow3_4);
    Arrow *arrow4_5 = new Arrow(section4, section5);
    scene->addItem(arrow4_5);
    Arrow *arrow5_6 = new Arrow(section5, section6);
    scene->addItem(arrow5_6);
    Arrow *arrow6_7 = new Arrow(section6, section7);
    scene->addItem(arrow6_7);
}

#if QT_CONFIG(wheelevent)
void Diagram::wheelEvent(QWheelEvent *event){
    auto factor = std::pow(2., -event->angleDelta().y() / 240.0);
    scale(factor, factor);
}
#endif

/*
In mouse event handlers we implement the function that the users can move the perspective of 
scene in view by holding the left button and moving the curosr.
*/
void Diagram::mousePressEvent(QMouseEvent *event){
    // Make sure only when left button is pressed, does the perspective movement start.
    if(event->button() == Qt::LeftButton){
        grabbing = true;
        grabbing_start_point = mapToScene(event->pos());
    }
}
void Diagram::mouseMoveEvent(QMouseEvent *event){
    // Make sure only when this move event followed by a press event, does the perspective start moving.
    // We realize it by checking grabbing flag.
    if((event->buttons() & Qt::LeftButton) && grabbing){
        QPointF grabbing_cur_point = mapToScene(event->pos());

        // Calculate the movement step.
        qreal dx = grabbing_cur_point.x() - grabbing_start_point.x();
        qreal dy = grabbing_cur_point.y() - grabbing_start_point.y();

        QRectF original_scene_rect = sceneRect();
        setSceneRect(QRectF(original_scene_rect.x() - dx, original_scene_rect.y() - dy, 
            original_scene_rect.width(), original_scene_rect.height()));
    }
}
void Diagram::mouseReleaseEvent(QMouseEvent *event){
    // Reset grabbing flag.
    if((event->button() == Qt::LeftButton) && grabbing){
        grabbing = false;
    }
}

void Diagram::reset_scene_rect(){
    setSceneRect(QRectF(0, 0, scene_w, scene_h));
}
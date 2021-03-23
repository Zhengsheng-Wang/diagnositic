#include <cmath>

#include <QtMath>
#include <QKeyEvent>
#include <QStyleOptionGraphicsItem>

#include "diagram.h"
#include "element.h"
#include "arrow.h"


//! [0]
/*
We set the reference size of blocks in profile diagram.We set the scene weight as 5 times 
the weight of reference size, the height as the number of blocks times the height of 
reference size plusing the vertical intervals between each two block. We set boundary 
as the border between the popup detailed blocks area. After the size of scene being set, 
we set the minimum size of view to the size of scene. 
*/
Diagram::Diagram(QWidget *parent): QGraphicsView(parent){
    QGraphicsScene *scene = new QGraphicsScene();

    // Set item and scene dimension.
    shape_w = 100.0, shape_h = 40.0;
    space_v = 20.0;
    int shape_num = 7;
    // We set the whole scene weight 7 times the weight of reference block weight.
    scene_w = shape_w * 7;
    scene_h = shape_h * shape_num + space_v * (shape_num + 1);
    boundary = shape_w * 2;
    // Set view minimum size.
    setMinimumSize(QSize(scene_w, scene_h));

    // Set the view field.
    scene->setSceneRect(0, 0, scene_w, scene_h);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);

    // Add blocks.
    QPoint center_point(scene_w / 7, space_v + shape_h / 2.);
    Element *section1 = new Element(this, QString("低噪声放大器"), shape_w, shape_h, tr("amplifier"));
    scene->addItem(section1);
    section1->setPos(center_point);

    center_point.setY(center_point.y() + shape_h + space_v);
    Element *section2 = new Element(this, QString("光发射机"), 0.7 * shape_w, shape_h, tr("transmitter"));
    scene->addItem(section2);
    section2->setPos(center_point);

    center_point.setY(center_point.y() + shape_h + space_v);
    Element *section3 = new Element(this, QString("光接收机"), 0.7 * shape_w, shape_h, tr("receiver"));
    scene->addItem(section3);
    section3->setPos(center_point);
    
    center_point.setY(center_point.y() + shape_h + space_v);
    Element *section4 = new Element(this, QString("射频开关"), 0.7 * shape_w, shape_h, tr("radio"));
    scene->addItem(section4);
    section4->setPos(center_point);

    center_point.setY(center_point.y() + shape_h + space_v);
    Element *section5 = new Element(this, QString("下变频器"), 0.7 * shape_w, shape_h, tr("down"));
    scene->addItem(section5);
    section5->setPos(center_point);

    center_point.setY(center_point.y() + shape_h + space_v);
    Element *section6 = new Element(this, QString("中频开关"), 0.7 * shape_w, shape_h, tr("if"));
    scene->addItem(section6);
    section6->setPos(center_point);

    center_point.setY(center_point.y() + shape_h + space_v);
    Element *section7 = new Element(this, QString("解调器"), 0.7 * shape_w, shape_h, tr("demodulator"));
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


    Element::fault_spu_name = tr("spu12");
}
//! [0]

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

//! []
/*
After clicking the "重置" button at the bottom of viewer, this slot function is called.
First it reset the view field so the top-left corner of view is the point(0, 0) of scene.
Second it select all items to right of boundary, and remove them from the scene.
*/
void Diagram::reset_scene_rect(){
    // Set the vision field so the top-left corner of scene is point(0, 0).
    setSceneRect(QRectF(0, 0, scene_w, scene_h));
    centerOn(scene_w / 2., scene_h / 2.);

    // Remove items to the right of the boundary.
    QPainterPath path;
    path.addRect(QRectF(QPointF(boundary, 0), QPointF(scene_w, scene_h)));
    QGraphicsScene *scene = this->scene();
    scene->setSelectionArea(path);
    for(auto it : scene->selectedItems()){
        scene->removeItem(it);
    }
    scene->update();
}
//! []

//! []
/*
Draw detailed sub-digram according to the element type transfered to this function.
Call different drawing function according to the element type.
*/
void Diagram::draw_detailed_diagram(const QString &element_name, const QString &spu_name){
    if(element_name== tr("低噪声放大器")){
        draw_amplifier();
    }
    else if(element_name== tr("光发射机")){
        draw_transmitter();
    }
    else if(element_name== tr("光接收机")){
        draw_receiver();
    }
    else if(element_name== tr("射频开关")){
        draw_radio();
    }
    else if(element_name== tr("下变频器")){
        draw_down();
    }
    else if(element_name== tr("中频开关")){
        draw_if();
    }
    else if(element_name== tr("解调器")){
        draw_demodulator();
    }

    // Fresh this area.
    this->scene()->update(QRectF(boundary, 0, scene_w, scene_h));
}
//! []

//! []
/*
In draw_***() function we draw the inner signal processing links of a device. We draw those spus while keeping 
the links belong to same spu in a same height and those belong to later spu 
*/
void Diagram::draw_amplifier(){
    QGraphicsScene *scene= this->scene();
    QPointF center_point(boundary + 2 * shape_w + shape_w / 2., space_v + shape_h / 2.);

    // spu1
    Element *link1 = new Element(this, tr("电流"), 0.7 * shape_w, shape_h, tr("spu1"));
    scene->addItem(link1);
    link1->setPos(center_point);

    Element *link2 = new Element(this, tr("15V电源A"), shape_w, shape_h, tr("spu1"));
    scene->addItem(link2);
    link2->setPos(center_point + QPointF(-shape_w, shape_h + space_v));

    Element *link3 = new Element(this, tr("15V电源B"), shape_w, shape_h, tr("spu1"));
    scene->addItem(link3);
    link3->setPos(center_point + QPointF(shape_w, shape_h + space_v));

    // The signal flowing fixture.
    Element *fixture1 = new Element(this, tr(""), 0.1 * shape_w, 0.1 * shape_h, tr("fixture1"));
    scene->addItem(fixture1);
    fixture1->setPos(center_point + QPointF(0, 2 * (shape_h + space_v)));

    // spu2
    Element *link4 = new Element(this, tr("放大器"), 0.7 * shape_w, shape_h, tr("spu2"));
    scene->addItem(link4);
    link4->setPos(center_point + QPointF(0, 3 * (shape_h + space_v)));

    // Join one element block to another with arrow.
    Arrow *arrow1_fixture1 = new Arrow(link1, fixture1);
    scene->addItem(arrow1_fixture1);
    Arrow *arrow2_fixture1 = new Arrow(link2, fixture1);
    scene->addItem(arrow2_fixture1);
    Arrow *arrow3_fixture1 = new Arrow(link3, fixture1);
    scene->addItem(arrow3_fixture1);
    Arrow *arrow_fixture1_4 = new Arrow(fixture1, link4);
    scene->addItem(arrow_fixture1_4);
}
//! []

//! []
void Diagram::draw_transmitter(){
    QGraphicsScene *scene = this->scene();
    QPointF center_point(boundary + 2 * shape_w + shape_w / 2., space_v + shape_h / 2. + (space_v + shape_h));

    // spu3.
    Element *link1 = new Element(this, tr("电源"), 0.7 * shape_w, shape_h, tr("spu3"));
    scene->addItem(link1);
    link1->setPos(center_point);
    // spu4.
    Element *link2 = new Element(this, tr("发射放大器"), shape_w, shape_h, tr("spu4"));
    scene->addItem(link2);
    link2->setPos(center_point + QPointF(0, shape_h + space_v));
    // spu5.
    Element *link3 = new Element(this, tr("发射功率"), shape_w, shape_h, tr("spu5"));
    scene->addItem(link3);
    link3->setPos(center_point + QPointF(0, 2 * (shape_h + space_v)));

    // Join.
    Arrow *arrow1_2 = new Arrow(link1, link2);
    scene->addItem(arrow1_2);
    Arrow *arrow2_3 = new Arrow(link2, link3);
    scene->addItem(arrow2_3);
}
//! []

//! []
void Diagram::draw_receiver(){
    QGraphicsScene *scene = this->scene();
    QPointF center_point(boundary + 2 * shape_w + shape_w / 2., space_v + shape_h / 2. + 
                         2 * (shape_h + space_v));

    // spu6.
    Element *link1 = new Element(this, tr("电源"), 0.7 * shape_w, shape_h, tr("spu6"));
    scene->addItem(link1);
    link1->setPos(center_point);
    // spu7.
    Element *link2 = new Element(this, tr("接收功率"), shape_w, shape_h, tr("spu7"));
    scene->addItem(link2);
    link2->setPos(center_point + QPointF(0, shape_h + space_v));
    // spu8.
    Element *link3 = new Element(this, tr("接收放大器"), shape_w, shape_h, tr("spu8"));
    scene->addItem(link3);
    link3->setPos(center_point + QPointF(0, 2 * (shape_h + space_v)));

    // Join.
    Arrow *arrow1_2 = new Arrow(link1, link2);
    scene->addItem(arrow1_2);
    Arrow *arrow2_3 = new Arrow(link2, link3);
    scene->addItem(arrow2_3);
}
//! []

//! []
void Diagram::draw_radio(){
    QGraphicsScene *scene = this->scene();
    QPointF center_point(boundary + 2 * shape_w + shape_w / 2., space_v + shape_h / 2. + 
                         3 * (shape_h + space_v));

    // spu9.
    Element *link1 = new Element(this, tr("电源A"), 0.7 * shape_w, shape_h, tr("spu9"));
    scene->addItem(link1);
    link1->setPos(center_point + QPointF(-shape_w, 0));

    Element *link2 = new Element(this, tr("电源B"), 0.7 * shape_w, shape_h, tr("spu9"));
    scene->addItem(link2);
    link2->setPos(center_point + QPointF(shape_w, 0));

    // Fixture.
    Element *fixture9 = new Element(this, tr(""), 0.1 * shape_w, 0.1 * shape_h, tr("fixture9"));
    scene->addItem(fixture9);
    fixture9->setPos(center_point + QPointF(0, shape_h + space_v));


    // Join.
    Arrow *arrow1_fixture9 = new Arrow(link1, fixture9);
    scene->addItem(arrow1_fixture9);
    Arrow *arrow2_fixture9 = new Arrow(link2, fixture9);
    scene->addItem(arrow2_fixture9);
}
//! []

//! []
void Diagram::draw_down(){
    QGraphicsScene *scene = this->scene();
    QPointF center_point(boundary + 2 * shape_w + shape_w / 2., space_v + shape_h / 2. + 
                         3 * (shape_h + space_v));

    // spu10.
    Element *link1 = new Element(this, tr("5V电源"), shape_w, shape_h, tr("spu10"));
    scene->addItem(link1);
    link1->setPos(center_point + QPointF(-shape_w, 0));

    Element *link2 = new Element(this, tr("12V电源"), shape_w, shape_h, tr("spu10"));
    scene->addItem(link2);
    link2->setPos(center_point + QPointF(shape_w, 0));

    Element *fixture10 = new Element(this, tr(""), 0.1 * shape_w, 0.1 * shape_h, tr("spu10"));
    scene->addItem(fixture10);
    fixture10->setPos(center_point + QPointF(0, shape_h + space_v));


    // spu11.
    Element *link3 = new Element(this, tr("本振锁定"), shape_w, shape_h, tr("spu11"));
    scene->addItem(link3);
    link3->setPos(center_point + QPointF(0, 2 * (shape_h + space_v)));

    // spu12.
    Element *link4 = new Element(this, tr("变频"), 0.7 * shape_w, shape_h, tr("spu12"));
    scene->addItem(link4);
    link4->setPos(center_point + QPointF(0, 3 * (shape_h + space_v)));


    // Join.
    Arrow *arrow1_fixture10 = new Arrow(link1, fixture10);
    scene->addItem(arrow1_fixture10);
    Arrow *arrow2_fixture10 = new Arrow(link2, fixture10);
    scene->addItem(arrow2_fixture10);
    Arrow *arrow_fixture10 = new Arrow(fixture10, link3);
    scene->addItem(arrow_fixture10);
    Arrow *arrow3_4 = new Arrow(link3, link4);
    scene->addItem(arrow3_4);
}
//! []

//! []
void Diagram::draw_if(){
    QGraphicsScene *scene = this->scene();
    QPointF center_point(boundary + 2 * shape_w + shape_w / 2., space_v + shape_h / 2. + 
                         5 * (shape_h + space_v));
    
    // spu13.
    Element *link1 = new Element(this, tr("电源A"), 0.7 * shape_w, shape_h, tr("spu13"));
    scene->addItem(link1);
    link1->setPos(center_point + QPointF(-shape_w, 0));

    Element *link2 = new Element(this, tr("电源B"), 0.7 * shape_w, shape_h, tr("spu13"));
    scene->addItem(link2);
    link2->setPos(center_point + QPointF(shape_w, 0));

    Element *fixture13 = new Element(this, tr(""), 0.1 * shape_w, 0.1 * shape_h, tr("fixture13"));
    scene->addItem(fixture13);
    fixture13->setPos(center_point + QPointF(0, shape_h));

    // Join.
    Arrow *arrow1_fixture13 = new Arrow(link1, fixture13);
    scene->addItem(arrow1_fixture13);
    Arrow *arrow2_fixture13 = new Arrow(link2, fixture13);
    scene->addItem(arrow2_fixture13);
}
//! []

//! []
void Diagram::draw_demodulator(){
    QGraphicsScene *scene = this->scene();
    QPointF center_point(boundary + 2 * shape_w + shape_w / 2., space_v + shape_h / 2.);

    // Save original weight and height of element block and halve the weight and height.
    qreal original_shape_h = shape_h, original_space_v = space_v;
    shape_h /= 2.;
    space_v /= 2.;

    // spu14.
    Element *link1 = new Element(this, tr("VCC电压"), shape_w, shape_h, tr("spu14"));
    scene->addItem(link1);
    link1->setPos(center_point);

    Element *link2 = new Element(this, tr("2.5V电源"), shape_w, shape_h, tr("spu14"));
    scene->addItem(link2);
    link2->setPos(center_point + QPointF(-shape_w, shape_h + space_v)); 

    Element *link3 = new Element(this, tr("1.2V电源"), shape_w, shape_h, tr("spu14"));
    scene->addItem(link3);
    link3->setPos(center_point + QPointF(shape_w, shape_h + space_v));

    Element *fixture14 = new Element(this, tr(""), 0.1 * shape_w, 0.1 * shape_h, tr("fixture14"));
    scene->addItem(fixture14);
    fixture14->setPos(center_point + QPointF(0, 2 * (shape_h + space_v)));

    // spu15.
    Element *link4 = new Element(this, tr("载波恢复"), shape_w, shape_h, tr("spu15"));
    scene->addItem(link4);
    link4->setPos(center_point + QPointF(0, 3 * (shape_h + space_v)));

    // spu16.
    Element *link5 = new Element(this, tr("混频"), 0.7 * shape_w, shape_h, tr("spu16"));
    scene->addItem(link5);
    link5->setPos(center_point + QPointF(0, 4 * (shape_h + space_v)));

    // spu17.
    Element *link6 = new Element(this, tr("低通滤波"), shape_w, shape_h, tr("spu17"));
    scene->addItem(link6);
    link6->setPos(center_point + QPointF(0, 5 * (shape_h + space_v)));

    // spu18.
    Element *link7 = new Element(this, tr("时钟恢复"), shape_w, shape_h, tr("spu18"));
    scene->addItem(link7);
    link7->setPos(center_point + QPointF(0, 6 * (shape_h + space_v)));

    // spu19.
    Element *link8 = new Element(this, tr("抽样判决"), shape_w, shape_h, tr("spu19"));
    scene->addItem(link8);
    link8->setPos(center_point + QPointF(0, 7 * (shape_h + space_v)));

    // spu20.
    Element *link9 = new Element(this, tr("维特比译码"), shape_w, shape_h, tr("spu20"));
    scene->addItem(link9);
    link9->setPos(center_point + QPointF(0, 8 * (shape_h + space_v)));

    // spu21.
    Element *link10 = new Element(this, tr("帧同步"), shape_w, shape_h, tr("spu21"));
    scene->addItem(link10);
    link10->setPos(center_point + QPointF(0, 9 * (shape_h + space_v)));

    // Fixture.
    Element *fixture21 = new Element(this, tr(""), 0.1 * shape_w, 0.1 * shape_h, tr("fixture21"));
    scene->addItem(fixture21);
    fixture21->setPos(center_point + QPointF(0, 10 * (shape_h + space_v)));

    // spu22.
    Element *link11 = new Element(this, tr("RS译码"), shape_w, shape_h, tr("spu22"));
    scene->addItem(link11);
    link11->setPos(center_point + QPointF(-shape_w, 11 * (shape_h + space_v)));

    Element *link12 = new Element(this, tr("LDPC译码"), shape_w, shape_h, tr("spu22"));
    scene->addItem(link12);
    link12->setPos(center_point + QPointF(0, 12 * (shape_h + space_v)));

    Element *link13 = new Element(this, tr("解扰"), 0.7 * shape_w, shape_h, tr("spu22"));
    scene->addItem(link13);
    link13->setPos(center_point + QPointF(shape_w, 11 * (shape_h + space_v)));


    // Join.
    Arrow *arrow1_fixture14 = new Arrow(link1, fixture14);
    scene->addItem(arrow1_fixture14);
    Arrow *arrow2_fixture14 = new Arrow(link2, fixture14);
    scene->addItem(arrow2_fixture14);
    Arrow *arrow3_fixture14 = new Arrow(link3, fixture14);
    scene->addItem(arrow3_fixture14);
    Arrow *arrow_fixture14_4 = new Arrow(fixture14, link4);
    scene->addItem(arrow_fixture14_4);
    Arrow *arrow4_5 = new Arrow(link4, link5);
    scene->addItem(arrow4_5);
    Arrow *arrow5_6 = new Arrow(link5, link6);
    scene->addItem(arrow5_6);
    Arrow *arrow6_7 = new Arrow(link6, link7);
    scene->addItem(arrow6_7);
    Arrow *arrow7_8 = new Arrow(link7, link8);
    scene->addItem(arrow7_8);
    Arrow *arrow8_9 = new Arrow(link8, link9);
    scene->addItem(arrow8_9);
    Arrow *arrow9_10 = new Arrow(link9, link10);
    scene->addItem(arrow9_10);
    Arrow *arrow10_fixture21 = new Arrow(link10, fixture21);
    scene->addItem(arrow10_fixture21);
    Arrow *arrow_fixture21_11 = new Arrow(fixture21, link11);
    scene->addItem(arrow_fixture21_11);
    Arrow *arrow_fixture21_12 = new Arrow(fixture21, link12);
    scene->addItem(arrow_fixture21_12);
    Arrow *arrow_fixture21_13 = new Arrow(fixture21, link13);
    scene->addItem(arrow_fixture21_13);

    // Restore the original value of weight and height of element block.
    shape_h = original_shape_h, space_v = original_space_v;
}
//! []
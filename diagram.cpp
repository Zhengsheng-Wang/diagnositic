#include <cmath>

#include <QtMath>
#include <QKeyEvent>
#include <QStyleOptionGraphicsItem>

#include "diagram.h"
#include "element.h"
#include "arrow.h"

Diagram::Diagram(QWidget *parent): QGraphicsView(parent){
    QGraphicsScene *scene = new QGraphicsScene();


    // Set the dimensions of blocks and scene.
    // 
    // We generate the dimensions of scene according to those of blocks, the vertical space 
    // between 2 vertically adjacent blocks, the number of device blocks.
    // 
    // We divide the scene into 2 area.  One shows the device blocks, and another shows the 
    // signal processing link blocks.  We set a invisible border between them.
    //
    // We don't want the viewport of scene to be resized smaller than the size of scene itself, 
    // so we set the minimum size of the viewport te be the size of scene. 
    blockWidth = 100.0, blockHeight = 40.0;
    verticalSpace = 20.0;
    int deviceBlockNumber = 7;
    sceneWidth = blockWidth * 7;
    sceneHeight = blockHeight * deviceBlockNumber + verticalSpace * (deviceBlockNumber + 1);
    border = blockWidth * 2;
    setMinimumSize(QSize(sceneWidth, sceneHeight));
    scene->setSceneRect(0, 0, sceneWidth, sceneHeight);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);


    // Add device blocks and connect them by solid arrow lines.
    //
    // Because a device contains several spus so the "spuName" of each device is set to "###".
    // Why we don't use empty string as their "spuName" is that "faultSPUName" is initially set to
    // empty string, and everytime we render a block we first check whether it is fault so device
    // block will be classified as fault if we use empty string as it's "spuName".
    QPoint centerPoint(sceneWidth / 7, verticalSpace + blockHeight / 2.);
    Element *section1 = new Element(this, QString("低噪声放大器"), blockWidth, blockHeight, tr("###"));
    scene->addItem(section1);
    section1->setPos(centerPoint);

    centerPoint.setY(centerPoint.y() + blockHeight + verticalSpace);
    Element *section2 = new Element(this, QString("光发射机"), 0.7 * blockWidth, blockHeight, tr("###"));
    scene->addItem(section2);
    section2->setPos(centerPoint);

    centerPoint.setY(centerPoint.y() + blockHeight + verticalSpace);
    Element *section3 = new Element(this, QString("光接收机"), 0.7 * blockWidth, blockHeight, tr("###"));
    scene->addItem(section3);
    section3->setPos(centerPoint);
    
    centerPoint.setY(centerPoint.y() + blockHeight + verticalSpace);
    Element *section4 = new Element(this, QString("射频开关"), 0.7 * blockWidth, blockHeight, tr("###"));
    scene->addItem(section4);
    section4->setPos(centerPoint);

    centerPoint.setY(centerPoint.y() + blockHeight + verticalSpace);
    Element *section5 = new Element(this, QString("下变频器"), 0.7 * blockWidth, blockHeight, tr("###"));
    scene->addItem(section5);
    section5->setPos(centerPoint);

    centerPoint.setY(centerPoint.y() + blockHeight + verticalSpace);
    Element *section6 = new Element(this, QString("中频开关"), 0.7 * blockWidth, blockHeight, tr("###"));
    scene->addItem(section6);
    section6->setPos(centerPoint);

    centerPoint.setY(centerPoint.y() + blockHeight + verticalSpace);
    Element *section7 = new Element(this, QString("解调器"), 0.7 * blockWidth, blockHeight, tr("###"));
    scene->addItem(section7);
    section7->setPos(centerPoint);

    Arrow *arrow1To2 = new Arrow(section1, section2);
    scene->addItem(arrow1To2);
    Arrow *arrow2To3 = new Arrow(section2, section3);
    scene->addItem(arrow2To3);
    Arrow *arrow3To4 = new Arrow(section3, section4);
    scene->addItem(arrow3To4);
    Arrow *arrow4To5 = new Arrow(section4, section5);
    scene->addItem(arrow4To5);
    Arrow *arrow5To6 = new Arrow(section5, section6);
    scene->addItem(arrow5To6);
    Arrow *arrow6To7 = new Arrow(section6, section7);
    scene->addItem(arrow6To7);
}

#if QT_CONFIG(wheelevent)
void Diagram::wheelEvent(QWheelEvent *event){
    auto factor = std::pow(2., -event->angleDelta().y() / 240.0);
    scale(factor, factor);
}
#endif

void Diagram::mousePressEvent(QMouseEvent *event){
    // Make sure only when left button is pressed, does the perspective movement start.
    if(event->button() == Qt::LeftButton){
        grabbing = true;
        grabbingStartPoint = mapToScene(event->pos());
    }
}
void Diagram::mouseMoveEvent(QMouseEvent *event){
    // Make sure only when this move event followed by a press event, does the perspective start moving.
    // We realize it by checking grabbing flag.
    if((event->buttons() & Qt::LeftButton) && grabbing){
        QPointF grabbingCurrentPoint = mapToScene(event->pos());

        // Calculate the movement step.
        qreal dx = grabbingCurrentPoint.x() - grabbingStartPoint.x();
        qreal dy = grabbingCurrentPoint.y() - grabbingStartPoint.y();

        QRectF originalSceneRect = sceneRect();
        setSceneRect(QRectF(originalSceneRect.x() - dx, originalSceneRect.y() - dy, 
            originalSceneRect.width(), originalSceneRect.height()));
    }
}
void Diagram::mouseReleaseEvent(QMouseEvent *event){
    // Reset grabbing flag.
    if((event->button() == Qt::LeftButton) && grabbing){
        grabbing = false;
    }
}

void Diagram::resetSceneRect(){
    // Set the top-left corner of the vision field point(0, 0).
    setSceneRect(QRectF(0, 0, sceneWidth, sceneHeight));
    centerOn(sceneWidth / 2., sceneHeight / 2.);

    // Remove items to the right of the border.
    QPainterPath path;
    path.addRect(QRectF(QPointF(border, 0), QPointF(sceneWidth, sceneHeight)));
    QGraphicsScene *scene = this->scene();
    scene->setSelectionArea(path);
    for(auto it : scene->selectedItems()){
        scene->removeItem(it);
    }
    scene->update();
}

void Diagram::drawDetailDiagram(const QString &elementName){
    // Draw detailed sub-digram according to the for "elementName" device.
    // Call different drawing function according to the element type.
    if(elementName== tr("低噪声放大器")){
        drawAmplifier();
    }
    else if(elementName== tr("光发射机")){
        drawTransmitter();
    }
    else if(elementName== tr("光接收机")){
        drawReceiver();
    }
    else if(elementName== tr("射频开关")){
        drawRadio();
    }
    else if(elementName== tr("下变频器")){
        drawDown();
    }
    else if(elementName== tr("中频开关")){
        drawIf();
    }
    else if(elementName== tr("解调器")){
        drawDemodulator();
    }

    // Fresh this area.
    this->scene()->update(QRectF(border, 0, sceneWidth, sceneHeight));
}

void Diagram::drawAmplifier(){
    QGraphicsScene *scene= this->scene();
    QPointF centerPoint(border + 2 * blockWidth + blockWidth / 2., verticalSpace + blockHeight / 2.);

    // spu1
    Element *link1 = new Element(this, tr("电流"), 0.7 * blockWidth, blockHeight, tr("spu1"));
    scene->addItem(link1);
    link1->setPos(centerPoint);

    Element *link2 = new Element(this, tr("15V电源A"), blockWidth, blockHeight, tr("spu1"));
    scene->addItem(link2);
    link2->setPos(centerPoint + QPointF(-blockWidth, blockHeight + verticalSpace));

    Element *link3 = new Element(this, tr("15V电源B"), blockWidth, blockHeight, tr("spu1"));
    scene->addItem(link3);
    link3->setPos(centerPoint + QPointF(blockWidth, blockHeight + verticalSpace));

    // The signal flowing fixture.
    Element *fixture1 = new Element(this, tr("fixture1"), 0.1 * blockWidth, 0.1 * blockHeight, tr("###"));
    scene->addItem(fixture1);
    fixture1->setPos(centerPoint + QPointF(0, 2 * (blockHeight + verticalSpace)));

    // spu2
    Element *link4 = new Element(this, tr("放大器"), 0.7 * blockWidth, blockHeight, tr("spu2"));
    scene->addItem(link4);
    link4->setPos(centerPoint + QPointF(0, 3 * (blockHeight + verticalSpace)));

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

void Diagram::drawTransmitter(){
    QGraphicsScene *scene = this->scene();
    QPointF centerPoint(border + 2 * blockWidth +  blockWidth / 2., 
                        verticalSpace + blockHeight / 2. + (verticalSpace + blockHeight));

    // spu3.
    Element *link1 = new Element(this, tr("电源"), 0.7 * blockWidth, blockHeight, tr("spu3"));
    scene->addItem(link1);
    link1->setPos(centerPoint);
    // spu4.
    Element *link2 = new Element(this, tr("发射放大器"), blockWidth, blockHeight, tr("spu4"));
    scene->addItem(link2);
    link2->setPos(centerPoint + QPointF(0, blockHeight + verticalSpace));
    // spu5.
    Element *link3 = new Element(this, tr("发射功率"), blockWidth, blockHeight, tr("spu5"));
    scene->addItem(link3);
    link3->setPos(centerPoint + QPointF(0, 2 * (blockHeight + verticalSpace)));

    // Join.
    Arrow *arrow1To2 = new Arrow(link1, link2);
    scene->addItem(arrow1To2);
    Arrow *arrow2To3 = new Arrow(link2, link3);
    scene->addItem(arrow2To3);
}
//! []

//! []
void Diagram::drawReceiver(){
    QGraphicsScene *scene = this->scene();
    QPointF centerPoint(border + 2 * blockWidth +  blockWidth / 2., verticalSpace + blockHeight / 2. + 
                         2 * (blockHeight + verticalSpace));

    // spu6.
    Element *link1 = new Element(this, tr("电源"), 0.7 * blockWidth, blockHeight, tr("spu6"));
    scene->addItem(link1);
    link1->setPos(centerPoint);
    // spu7.
    Element *link2 = new Element(this, tr("接收功率"), blockWidth, blockHeight, tr("spu7"));
    scene->addItem(link2);
    link2->setPos(centerPoint + QPointF(0, blockHeight + verticalSpace));
    // spu8.
    Element *link3 = new Element(this, tr("接收放大器"), blockWidth, blockHeight, tr("spu8"));
    scene->addItem(link3);
    link3->setPos(centerPoint + QPointF(0, 2 * (blockHeight + verticalSpace)));

    // Join.
    Arrow *arrow1To2 = new Arrow(link1, link2);
    scene->addItem(arrow1To2);
    Arrow *arrow2To3 = new Arrow(link2, link3);
    scene->addItem(arrow2To3);
}

void Diagram::drawRadio(){
    QGraphicsScene *scene = this->scene();
    QPointF centerPoint(border + 2 * blockWidth + blockWidth / 2., verticalSpace + blockHeight / 2. + 
                         3 * (blockHeight + verticalSpace));

    // spu9.
    Element *link1 = new Element(this, tr("电源A"), 0.7 * blockWidth, blockHeight, tr("spu9"));
    scene->addItem(link1);
    link1->setPos(centerPoint + QPointF(-blockWidth, 0));

    Element *link2 = new Element(this, tr("电源B"), 0.7 * blockWidth, blockHeight, tr("spu9"));
    scene->addItem(link2);
    link2->setPos(centerPoint + QPointF(blockWidth, 0));

    // Fixture.
    Element *fixture9 = new Element(this, tr("fixture9"), 0.1 * blockWidth, 0.1 * blockHeight, tr("###"));
    scene->addItem(fixture9);
    fixture9->setPos(centerPoint + QPointF(0, blockHeight + verticalSpace));


    // Join.
    Arrow *arrow1_fixture9 = new Arrow(link1, fixture9);
    scene->addItem(arrow1_fixture9);
    Arrow *arrow2_fixture9 = new Arrow(link2, fixture9);
    scene->addItem(arrow2_fixture9);
}

void Diagram::drawDown(){
    QGraphicsScene *scene = this->scene();
    QPointF centerPoint(border + 2 * blockWidth + blockWidth / 2., verticalSpace + blockHeight / 2. + 
                         3 * (blockHeight + verticalSpace));

    // spu10.
    Element *link1 = new Element(this, tr("5V电源"), blockWidth, blockHeight, tr("spu10"));
    scene->addItem(link1);
    link1->setPos(centerPoint + QPointF(-blockWidth, 0));

    Element *link2 = new Element(this, tr("12V电源"), blockWidth, blockHeight, tr("spu10"));
    scene->addItem(link2);
    link2->setPos(centerPoint + QPointF(blockWidth, 0));

    Element *fixture10 = new Element(this, tr("fixture10"), 0.1 * blockWidth, 0.1 * blockHeight, tr("###"));
    scene->addItem(fixture10);
    fixture10->setPos(centerPoint + QPointF(0, blockHeight + verticalSpace));


    // spu11.
    Element *link3 = new Element(this, tr("本振锁定"), blockWidth, blockHeight, tr("spu11"));
    scene->addItem(link3);
    link3->setPos(centerPoint + QPointF(0, 2 * (blockHeight + verticalSpace)));

    // spu12.
    Element *link4 = new Element(this, tr("变频"), 0.7 * blockWidth, blockHeight, tr("spu12"));
    scene->addItem(link4);
    link4->setPos(centerPoint + QPointF(0, 3 * (blockHeight + verticalSpace)));


    // Join.
    Arrow *arrow1_fixture10 = new Arrow(link1, fixture10);
    scene->addItem(arrow1_fixture10);
    Arrow *arrow2_fixture10 = new Arrow(link2, fixture10);
    scene->addItem(arrow2_fixture10);
    Arrow *arrow_fixture10 = new Arrow(fixture10, link3);
    scene->addItem(arrow_fixture10);
    Arrow *arrow3To4 = new Arrow(link3, link4);
    scene->addItem(arrow3To4);
}

void Diagram::drawIf(){
    QGraphicsScene *scene = this->scene();
    QPointF centerPoint(border + 2 * blockWidth + blockWidth / 2., verticalSpace + blockHeight / 2. + 
                         5 * (blockHeight + verticalSpace));
    
    // spu13.
    Element *link1 = new Element(this, tr("电源A"), 0.7 * blockWidth, blockHeight, tr("spu13"));
    scene->addItem(link1);
    link1->setPos(centerPoint + QPointF(-blockWidth, 0));

    Element *link2 = new Element(this, tr("电源B"), 0.7 * blockWidth, blockHeight, tr("spu13"));
    scene->addItem(link2);
    link2->setPos(centerPoint + QPointF(blockWidth, 0));

    Element *fixture13 = new Element(this, tr("fixture13"), 0.1 * blockWidth, 0.1 * blockHeight, tr("###"));
    scene->addItem(fixture13);
    fixture13->setPos(centerPoint + QPointF(0, blockHeight));

    // Join.
    Arrow *arrow1_fixture13 = new Arrow(link1, fixture13);
    scene->addItem(arrow1_fixture13);
    Arrow *arrow2_fixture13 = new Arrow(link2, fixture13);
    scene->addItem(arrow2_fixture13);
}

void Diagram::drawDemodulator(){
    QGraphicsScene *scene = this->scene();
    QPointF centerPoint(border + 2 * blockWidth + blockWidth / 2., verticalSpace + blockHeight / 2.);

    // Demodulator has too many signal processing links, so it would be better to narrow the 
    // space between ajacent blocks.
    // Save original weight and height of element block and halve the weight and height.
    qreal originalBlockHeight = blockHeight, originalVerticalSpace = verticalSpace;
    blockHeight /= 2.;
    verticalSpace /= 2.;

    // spu14.
    Element *link1 = new Element(this, tr("VCC电压"), blockWidth, blockHeight, tr("spu14"));
    scene->addItem(link1);
    link1->setPos(centerPoint);

    Element *link2 = new Element(this, tr("2.5V电源"), blockWidth, blockHeight, tr("spu14"));
    scene->addItem(link2);
    link2->setPos(centerPoint + QPointF(-blockWidth, blockHeight + verticalSpace)); 

    Element *link3 = new Element(this, tr("1.2V电源"), blockWidth, blockHeight, tr("spu14"));
    scene->addItem(link3);
    link3->setPos(centerPoint + QPointF(blockWidth, blockHeight + verticalSpace));

    Element *fixture14 = new Element(this, tr("fixture14"), 0.1 * blockWidth, 0.1 * blockHeight, tr("###"));
    scene->addItem(fixture14);
    fixture14->setPos(centerPoint + QPointF(0, 2 * (blockHeight + verticalSpace)));

    // spu15.
    Element *link4 = new Element(this, tr("载波恢复"), blockWidth, blockHeight, tr("spu15"));
    scene->addItem(link4);
    link4->setPos(centerPoint + QPointF(0, 3 * (blockHeight + verticalSpace)));

    // spu16.
    Element *link5 = new Element(this, tr("混频"), 0.7 * blockWidth, blockHeight, tr("spu16"));
    scene->addItem(link5);
    link5->setPos(centerPoint + QPointF(0, 4 * (blockHeight + verticalSpace)));

    // spu17.
    Element *link6 = new Element(this, tr("低通滤波"), blockWidth, blockHeight, tr("spu17"));
    scene->addItem(link6);
    link6->setPos(centerPoint + QPointF(0, 5 * (blockHeight + verticalSpace)));

    // spu18.
    Element *link7 = new Element(this, tr("时钟恢复"), blockWidth, blockHeight, tr("spu18"));
    scene->addItem(link7);
    link7->setPos(centerPoint + QPointF(0, 6 * (blockHeight + verticalSpace)));

    // spu19.
    Element *link8 = new Element(this, tr("抽样判决"), blockWidth, blockHeight, tr("spu19"));
    scene->addItem(link8);
    link8->setPos(centerPoint + QPointF(0, 7 * (blockHeight + verticalSpace)));

    // spu20.
    Element *link9 = new Element(this, tr("维特比译码"), blockWidth, blockHeight, tr("spu20"));
    scene->addItem(link9);
    link9->setPos(centerPoint + QPointF(0, 8 * (blockHeight + verticalSpace)));

    // spu21.
    Element *link10 = new Element(this, tr("帧同步"), blockWidth, blockHeight, tr("spu21"));
    scene->addItem(link10);
    link10->setPos(centerPoint + QPointF(0, 9 * (blockHeight + verticalSpace)));

    // Fixture.
    Element *fixture21 = new Element(this, tr("fixture21"), 0.1 * blockWidth, 0.1 * blockHeight, tr("###"));
    scene->addItem(fixture21);
    fixture21->setPos(centerPoint + QPointF(0, 10 * (blockHeight + verticalSpace)));

    // spu22.
    Element *link11 = new Element(this, tr("RS译码"), blockWidth, blockHeight, tr("spu22"));
    scene->addItem(link11);
    link11->setPos(centerPoint + QPointF(-blockWidth, 11 * (blockHeight + verticalSpace)));

    Element *link12 = new Element(this, tr("LDPC译码"), blockWidth, blockHeight, tr("spu22"));
    scene->addItem(link12);
    link12->setPos(centerPoint + QPointF(0, 12 * (blockHeight + verticalSpace)));

    Element *link13 = new Element(this, tr("解扰"), 0.7 * blockWidth, blockHeight, tr("spu22"));
    scene->addItem(link13);
    link13->setPos(centerPoint + QPointF(blockWidth, 11 * (blockHeight + verticalSpace)));


    // Join.
    Arrow *arrow1_fixture14 = new Arrow(link1, fixture14);
    scene->addItem(arrow1_fixture14);
    Arrow *arrow2_fixture14 = new Arrow(link2, fixture14);
    scene->addItem(arrow2_fixture14);
    Arrow *arrow3_fixture14 = new Arrow(link3, fixture14);
    scene->addItem(arrow3_fixture14);
    Arrow *arrow_fixture14_4 = new Arrow(fixture14, link4);
    scene->addItem(arrow_fixture14_4);
    Arrow *arrow4To5 = new Arrow(link4, link5);
    scene->addItem(arrow4To5);
    Arrow *arrow5To6 = new Arrow(link5, link6);
    scene->addItem(arrow5To6);
    Arrow *arrow6To7 = new Arrow(link6, link7);
    scene->addItem(arrow6To7);
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
    blockHeight = originalBlockHeight, verticalSpace = originalVerticalSpace;
}
//! []
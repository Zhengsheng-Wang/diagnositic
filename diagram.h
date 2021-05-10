#ifndef ABSTRACT_PROFILE_H
#define ABSTRACT_PROFILE_H

#include <QGraphicsView>

/*
Diagram is used to diaplay the diagram of data receiving system and mark the fault areas.

Diagram inherits QGraphicsView.  It is a frame that displays the silouhette of data receiving system 
in the original state.  Every receiving device is represented by a rectangle.  The linkage between devices
is represented by an arrow the direction of which is indicating the flowing of signal.
    When you double click any receiving device rectangle, the inner "signal processing link-level" structure of
the corresponding device is being displayed on the right-hand side of the Diagram frame.
*/
class Diagram: public QGraphicsView{
    Q_OBJECT
public:
    /*
    We set the reference size of blocks in profile diagram.  We set the scene weight as 5 times 
    the weight of reference size, the height as the number of blocks times the height of 
    reference size plusing the vertical intervals between each two block. We set boundary 
    as the border between the popup detailed blocks area. After the size of scene being set, 
    we set the minimum size of view to the size of scene. 
    */
    Diagram(QWidget *parent=nullptr);

    // Draw background.
    //void drawBackground(QPainter *painter, const QRectF &rect) override;

    #if QT_CONFIG(wheelevent)
    /*
    Wheel event handler implementing zoom in/out function.
    */
    void wheelEvent(QWheelEvent *event) override;
    #endif

    /*
    Draw detailed sub-diagram of each element block.
    */
    void drawDetailDiagram(const QString &elementName);
public slots:
    /*
    After clicking the "重置显示区域" button at the bottom of viewer, this slot function is called.
    First it reset the view field so the top-left corner of view is the point(0, 0) of scene.
    Second it select all items to right of boundary, and remove them from the scene.
    */
    void resetSceneRect();
protected:
    /*
    In three mouse event handlers we implement the function that the users can move the perspective of 
    scene in view by holding the left button and moving the curosr.
    */
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    /*
    In draw***() function for every device we draw the inner signal processing links of a device while keeping 
    the links belong to same spu in a same height and those belong to later spu in a lower height.
    */
    void drawAmplifier();
    void drawTransmitter();
    void drawReceiver();
    void drawRadio();
    void drawDown();
    void drawIf();
    void drawDemodulator();
private:
    // Grabbing movement flag.
    bool grabbing = false;
    QPointF grabbingStartPoint;

    qreal sceneWidth, sceneHeight; // scene size.
    qreal blockWidth, blockHeight; // element block size.
    qreal verticalSpace; // the length of vertical space between two adjacent block.

    qreal border; // the x-axis border between devices diagram and detail information diagram.
};

#endif // ABSTRACT_PROFILE_H

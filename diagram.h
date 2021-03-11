#ifndef ABSTRACT_PROFILE_H
#define ABSTRACT_PROFILE_H

#include <QGraphicsView>

// [0]
class Diagram: public QGraphicsView{
    Q_OBJECT

public:
    Diagram(QWidget *parent=nullptr);

    // Draw background.
    //void drawBackground(QPainter *painter, const QRectF &rect) override;

    // Wheel event handler implementing zoom in/out function.
    #if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *event) override;
    #endif
    
public slots:
    void reset_scene_rect();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    // Grabbing movement flag.
    bool grabbing = false;
    QPointF grabbing_start_point;
    // Scene rect size.
    qreal scene_w, scene_h;
};

#endif // ABSTRACT_PROFILE_H

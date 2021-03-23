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

public:
    // Draw detailed sub-diagram of each element block.
    void draw_detailed_diagram(const QString &element_type, const QString &t);
private:
    void draw_amplifier();
    void draw_transmitter();
    void draw_receiver();
    void draw_radio();
    void draw_down();
    void draw_if();
    void draw_demodulator();
    
public slots:
    // Restore the initial view field of QGraphicsView.
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
    // Element block size.
    qreal shape_w, shape_h;
    qreal space_v;

    // The boundary of device diagram and detail information diagram.
    qreal boundary;
};

#endif // ABSTRACT_PROFILE_H

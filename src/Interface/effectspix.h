#include <QtGui>


class gpix : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
public:
    gpix(const QPixmap &pix)
        : QObject(), QGraphicsPixmapItem(pix)
    {
    }
};


class blinkpix :public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

    QPropertyAnimation *anim;
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)

public :

    blinkpix(const QPixmap &pix) : QGraphicsPixmapItem(pix)
    {
        anim = new QPropertyAnimation(this,"opacity");
        anim->setEasingCurve(QEasingCurve::Linear);
        anim->setStartValue(0.25);
        anim->setEndValue(1.0);
        anim->setDuration(1000);
    }

signals:
    void pressed();

protected:
    void hoverMoveEvent ( QGraphicsSceneHoverEvent *event )
    {
        anim->setLoopCount(-1);
        anim->start();
    }

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
    {
        anim->setLoopCount(1);
    }

    void mousePressEvent ( QGraphicsSceneMouseEvent *event )
    {
        emit pressed();
    }
};

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

class ButtonItem :public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public :
ButtonItem(const QPixmap &pic) : QGraphicsPixmapItem(pic)
{}

signals:
void pressed();

protected:
void hoverMoveEvent ( QGraphicsSceneHoverEvent *event )
{
    setCursor(Qt::PointingHandCursor);
}
void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::ArrowCursor);
}
void mousePressEvent ( QGraphicsSceneMouseEvent *event )
{
    emit pressed();
}
};

class ArrowItem :public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public :
    ArrowItem(const QPixmap &pic) : QGraphicsPixmapItem(pic)
    {}

signals:
    void pressed();

protected:
    void hoverMoveEvent ( QGraphicsSceneHoverEvent *event )
    {
        setCursor(Qt::PointingHandCursor);
    }

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
    {
        setCursor(Qt::ArrowCursor);
    }

    void mousePressEvent ( QGraphicsSceneMouseEvent *event )
    {
        emit pressed();
    }
};

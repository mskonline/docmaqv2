#ifndef CERTIFICATEVIEW_H
#define CERTIFICATEVIEW_H
#include <QtGui>


class CredentialView : public QGraphicsView
{
    Q_OBJECT
public:
    CredentialView(QWidget *parent = 0) : QGraphicsView(parent)
    {}
signals:
    void viewresized();
    void viewscrolled();
protected:
    void resizeEvent(QResizeEvent *event)
    {
       QGraphicsView::resizeEvent(event);
       emit viewresized();
    }

    void wheelEvent(QWheelEvent *event)
    {
        QGraphicsView::wheelEvent(event);
        emit viewscrolled();
    }
};

#endif // CERTIFICATEVIEW_H

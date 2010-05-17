#ifndef DATEITEM_H
#define DATEITEM_H

#include <QtGui>

class CertificateScene;

class DateItem : public QGraphicsTextItem
{
    Q_OBJECT
private:
    QWidget *Form;
    CertificateScene *scene;
    QGraphicsProxyWidget *proxy;
    QDateEdit *de;
    int id;
    QPushButton *okButton;
public:
    DateItem(CertificateScene *,int);
    void constructWidgets();
public slots:
    void done();
signals :
    void changed(int,QString);
protected:
    void mousePressEvent ( QGraphicsSceneMouseEvent * event )
    {
        constructWidgets();
    }
    void hoverMoveEvent ( QGraphicsSceneHoverEvent * event )
    {
        setDefaultTextColor (QColor("red"));
    }
    void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event )
    {
        setDefaultTextColor (QColor("black"));
    }
};

#endif // DATEITEM_H

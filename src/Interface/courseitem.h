#ifndef COURSEITEM_H
#define COURSEITEM_H

#include <QtGui>

class CertificateScene;
class Student;
class CourseItem : public QGraphicsTextItem
{
Q_OBJECT
public:
    CertificateScene *scene;
    QWidget *Form;
    QGraphicsProxyWidget *proxy;
    CourseItem(CertificateScene *, int);
    int id;
    QComboBox *cbsem,*cbyear,*cbbranch,*cbcourse;
    QPushButton *okButton;
    void constructWidgets();
public slots:
    void done();
signals:
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

#endif // COURSEITEM_H

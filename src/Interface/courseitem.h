#ifndef COURSEITEM_H
#define COURSEITEM_H

#include <QtGui>

class CredentialScene;
class Student;

//extern int var_yr,var_sem;

class CourseItem : public QGraphicsTextItem
{
Q_OBJECT
public:
    CredentialScene *scene;
    QWidget *Form;
    QGraphicsProxyWidget *proxy;
    QComboBox *cbsem,*cbyear,*cbbranch,*cbcourse;
    QPushButton *okButton;
    int id;

    CourseItem(CredentialScene *, int);
    void constructWidgets();
    void setText(QString);

public slots:
    void done();

signals:
    void itemchanged(int,QString);

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

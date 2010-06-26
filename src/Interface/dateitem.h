#ifndef DATEITEM_H
#define DATEITEM_H

#include <QtGui>

class CredentialScene;

class DateItem : public QGraphicsTextItem
{
    Q_OBJECT

public:
    DateItem(CredentialScene *,QGraphicsItem *parent,int);
    void constructWidgets();
    void itemName(QString);
    void setText(QString);

private:
    CredentialScene *scene;
    QWidget *Form;
    QString iname;
    QGraphicsProxyWidget *proxy;
    QDateEdit *de;
    QPushButton *okButton;
    int id;

signals :
    void itemchanged(int,const QString &);

public slots:
    void done();

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

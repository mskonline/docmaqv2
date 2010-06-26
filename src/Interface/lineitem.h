#ifndef LINEITEM_H
#define LINEITEM_H

#include <QtGui>

class CredentialScene;

class LineItem :public QGraphicsTextItem
{
    Q_OBJECT

public:
    int id;

    LineItem(CredentialScene *, int ,int , QGraphicsItem *parent = 0);
    void setText(QString);
    void reset();
    void itemName(QString);
    void constructWidgets();

private:
    CredentialScene *scene;
    QWidget *Form;
    QGraphicsProxyWidget *proxy;
    QString iname;
    QLineEdit *le;
    int widgetWidth, lineWidth;

signals:
    void itemchanged(int ,const QString &);

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

#endif // LINEITEM_H

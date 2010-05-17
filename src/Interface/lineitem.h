#ifndef LINEITEM_H
#define LINEITEM_H

#include <QtGui>

class CertificateScene;

class LineItem :public QGraphicsTextItem
{
    Q_OBJECT
private:
    QWidget *Form;
    CertificateScene *scene;
    QGraphicsProxyWidget *proxy;
    QString iname;
    int widgetWidth, lineWidth;
    QLineEdit *le;
public:
    int id;
    LineItem(CertificateScene *,int ,int);
    void toManual();
    void toDB();
    void itemName(QString);
    void constructWidgets();
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

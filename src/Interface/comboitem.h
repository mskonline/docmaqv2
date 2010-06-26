#ifndef COMBOITEM_H
#define COMBOITEM_H

#include <QtGui>

class CredentialScene;

class ComboItem : public QGraphicsTextItem
{
    Q_OBJECT

public:
    int id;

    ComboItem(CredentialScene *,QGraphicsItem *parent,int ,int,QStringList);
    void setText(QString);
    void constructWidgets();
    void itemName(QString);

private:
       CredentialScene *scene;
       QWidget *Form;
       QGraphicsProxyWidget *proxy;
       QComboBox *cb;
       QString iname;
       QPushButton *okButton;
       QStringList list;
       int widgetWidth,comboWidth;

signals:
    void itemchanged(int , const QString &);

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

#endif // COMBOITEM_H

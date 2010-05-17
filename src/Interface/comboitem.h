#ifndef COMBOITEM_H
#define COMBOITEM_H

#include <QtGui>

class CertificateScene;

class ComboItem : public QGraphicsTextItem
{
    Q_OBJECT
private:
       QWidget *Form;
       CertificateScene *scene;
       QGraphicsProxyWidget *proxy;
       QComboBox *cb;
       int widgetWidth,comboWidth;
       QString iname;
       QPushButton *okButton;
       QStringList list;
public:
    int id;
    ComboItem(CertificateScene *,int ,int,QStringList);
    void toManual();
    void toDB();
    void constructWidgets();
    void itemName(QString);
//signals:
  //  changed(int id,QVariant data);
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

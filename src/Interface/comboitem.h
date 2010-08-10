/* DocmaQ v2.0, Credential Publishing System
    Copyright (C) 2010 M.Sai Kumar <msk.mymails@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef COMBOITEM_H
#define COMBOITEM_H

#include <QtGui>

class ComboItem : public QGraphicsTextItem
{
    Q_OBJECT

public:
    int id;

    ComboItem(QGraphicsScene *,int ,QStringList, QGraphicsItem *parent = 0);
    void setText(QString);
    void constructWidgets();
    void itemName(QString);
    ~ComboItem();

private:
       QGraphicsScene *scene;
       QWidget *Form;
       QGraphicsProxyWidget *proxy;
       QComboBox *cb;
       QString iname;
       QPushButton *okButton;
       QStringList list;
       int comboWidth;

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

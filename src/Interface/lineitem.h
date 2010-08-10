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

#ifndef LINEITEM_H
#define LINEITEM_H

#include <QtGui>

class LineItem :public QGraphicsTextItem
{
    Q_OBJECT

public:
    int id;

    LineItem(QGraphicsScene *,int , QGraphicsItem *parent = 0);
    void setText(QString);
    void reset();
    void itemName(QString);
    void constructWidgets();

private:
    QGraphicsScene *scene;
    QWidget *Form;
    QGraphicsProxyWidget *proxy;
    QString iname;
    QLineEdit *le;
    int lineWidth;

signals:
    void itemchanged(int ,const QString &);

public slots:
    void done();
    void cancel();

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

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

#ifndef EFFECTSPIX_H
#define EFFECTSPIX_H

#include <QtGui>

class HoverItem :public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public :
HoverItem(const QPixmap &pic) : QGraphicsPixmapItem(pic)
{}

void enable(bool e)
{
    setAcceptHoverEvents(e);
    this->e = e;
}

private :
    bool e;

signals:
void pressed();

protected:
void hoverMoveEvent ( QGraphicsSceneHoverEvent *event )
{
    setCursor(Qt::PointingHandCursor);
}
void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::ArrowCursor);
}
void mousePressEvent ( QGraphicsSceneMouseEvent *event )
{
    if(e)
        emit pressed();
}
};

#endif

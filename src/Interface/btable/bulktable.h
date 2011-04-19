/*  DocmaQ v2.1, Credential Publishing System
    Copyright (C) 2011 M.Sai Kumar <msk.mymails@gmail.com>

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

#ifndef BULKTABLE_H
#define BULKTABLE_H

#include <QTableWidget>
#include <QKeyEvent>
#include <QAction>
#include <QMenu>

class BulkTable : public QTableWidget
{
    Q_OBJECT

public:
    QFont ft;
    QAction *EditAct, *RAct , *DAct, *CAct;
    int rc,st_ptr;
    BulkTable(QWidget *parent = 0);
    void add_roll(QString,QString);

signals :
    void removeSt(int);
    void b_clearRow();

public slots:
    void editAct();
    void refreshAct();
    void clearTable();
    void clearRow();

private:
    void keyPressEvent(QKeyEvent *e);
    void contextMenuEvent(QContextMenuEvent *e);
};

#endif // BULKTABLE_H

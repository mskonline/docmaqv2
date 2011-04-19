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

#include "bulktable.h"
#include <QKeyEvent>

/*
 * Constructor
 */
BulkTable::BulkTable(QWidget *parent):QTableWidget(parent)
{
    rc = 0;
    st_ptr = -1;
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setEditTriggers(QAbstractItemView::DoubleClicked);
    setFocusPolicy(Qt::StrongFocus);

    EditAct = new QAction("Edit Roll Number",this);
    EditAct->setIcon(QIcon(":/Images/edit.png"));
    RAct = new QAction("Revert Details",this);
    RAct->setIcon(QIcon(":/Images/refresh.png"));
    DAct = new QAction("Delete Roll Number",this);
    DAct->setIcon(QIcon(":/Images/delete.png"));
    CAct = new QAction("Clear All",this);
    CAct->setIcon(QIcon(":/Images/clear.png"));

    connect(this->EditAct, SIGNAL(triggered()),this,SLOT(editAct()));
    connect(RAct,SIGNAL(triggered()),this,SLOT(refreshAct()));
    connect(DAct,SIGNAL(triggered()),this,SLOT(clearRow()));
    connect(CAct,SIGNAL(triggered()),this,SLOT(clearTable()));
}

/* add_roll(QString,QString)
 * Called : By AppManager::onRollEntry()
 * Performs : Adds Roll in the btable
 */
void BulkTable::add_roll(QString roll,QString t)
{
    // Set Column 0 (Roll)
    insertRow(rc);
    setItem(rc,0,new QTableWidgetItem(roll));

    // Set Column 1 (Certificate Type)
    QTableWidgetItem *item = new QTableWidgetItem(t);
    item->setFlags(Qt::NoItemFlags);
    item->setBackgroundColor(QColor(197,239,253));
    item->setTextAlignment(Qt::AlignCenter);

    setItem(rc,1,item);
    setRowHeight(rc,30);
    ++rc;
}

/*
 * Edit Action
 */
void BulkTable::editAct()
{
    openPersistentEditor(item(st_ptr,0));
}

/*
 * Refresh Action
 */
void BulkTable::refreshAct()
{
    emit cellChanged(st_ptr,0);
}

/*
 * Clearing Table
 */
void BulkTable::clearTable()
{
    if(rc == 0)
        return;

    while(rc)
    {
        this->removeRow(rc);
        --rc;
    }

    removeRow(rc);
    emit removeSt(-1);
}

/*
 * Clearing Row
 */
void BulkTable::clearRow()
{
    this->removeRow(st_ptr);
    rc = rowCount();
    clearSelection();
    emit removeSt(st_ptr);
}

/*
 * Key Press Event
 */
void BulkTable::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_Delete :
        emit b_clearRow();
    }
}

/*
 * Context Menu Event
 */
void BulkTable::contextMenuEvent(QContextMenuEvent *event)
{
    if(st_ptr != -1 )
    {
        QMenu menu;
        menu.addAction(EditAct);
        menu.addAction(DAct);
        menu.addAction(RAct);
        menu.addAction(CAct);
        menu.exec(event->globalPos());
    }
}

#include "bulktable.h"
#include <QKeyEvent>

BulkTable::BulkTable(QWidget *parent):QTableWidget(parent)
{
    rc = 0;
    ft.setBold(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
}


void BulkTable::add_roll(QString roll,QString t)
{
   insertRow(rc);
   setItem(rc,0,new QTableWidgetItem(roll));
   QTableWidgetItem *item = new QTableWidgetItem(t);
   item->setFlags(Qt::NoItemFlags);
   item->setBackgroundColor(QColor(255,237,132));
   item->setFont(ft);
   setItem(rc,1,item);
   ++rc;
}

void BulkTable::clearTable()
{
    if(rc == 0)
        return;
    this->clearContents();
    rc = 0;
}


void BulkTable::KeyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_Delete : this->removeRow(st_ptr);

    }
}

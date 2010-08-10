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

#include "comboitem.h"

ComboItem::ComboItem(QGraphicsScene *scene,int comboWidth,QStringList list, QGraphicsItem *parent):QGraphicsTextItem(parent)
{
    this->scene = scene;
    this->comboWidth = comboWidth;
    this->list = list;
}

void ComboItem::itemName(QString name)
{
    this->iname = name;
}

void ComboItem::setText(QString t)
{
    if(t.isEmpty())
        this->setPlainText(iname);
    else
        this->setPlainText(t);
}

void ComboItem::constructWidgets()
{
    // Report an item being changed
    emit itemchanged(-id,"");

    Form = new QWidget;
    Form->resize(comboWidth + 71,40);

    cb = new QComboBox(Form);
    cb->setGeometry(QRect(10, 10, comboWidth, 20));
    cb->addItems(list);
    cb->setCurrentIndex(cb->findText(this->toPlainText()));

    okButton =  new QPushButton(Form);
    okButton->setIcon(QIcon(":/Images/ok.png"));
    okButton->show();
    okButton->setGeometry(QRect(20 + comboWidth,10,41,21));
    connect(okButton,SIGNAL(pressed()),this,SLOT(done()));

    proxy = scene->addWidget(Form);
    proxy->setZValue(this->zValue() + 1);
    proxy->setPos(this->pos());
}

void ComboItem::done()
{
    this->setPlainText(cb->currentText());
    scene->removeItem(proxy);
    emit itemchanged(id,this->toPlainText());
}

ComboItem::~ComboItem()
{
    list.clear();
}

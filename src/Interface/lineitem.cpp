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

#include "lineitem.h"

LineItem::LineItem(QGraphicsScene *scene,int lineWidth, QGraphicsItem *parent) : QGraphicsTextItem(parent)
{
    this->scene = scene;
    this->lineWidth = lineWidth;
}

void LineItem::itemName(QString name)
{
    this->iname = name;
}

void LineItem::setText(QString t)
{
    if(t.isEmpty())
        this->setPlainText(iname);
    else
        this->setPlainText(t);
}

void LineItem::reset()
{
    this->setPlainText(iname);
}

void LineItem::constructWidgets()
{
    // Report an item is being changed
    emit itemchanged(-id,"");

    Form = new QWidget;
    Form->setWindowFlags(Qt::Widget);
    Form->resize(lineWidth + 122,40);
    //Form->setWindowOpacity(0.75);

    le = new QLineEdit(Form);
    le->setGeometry(QRect(10, 10, lineWidth, 20));
    le->setText(this->toPlainText());
    le->setFocus();
    le->setCursorPosition(0);

    QPushButton *okButton =  new QPushButton(Form);
    okButton->setToolTip("Save");
    okButton->setIcon(QIcon(":/Images/ok.png"));
    okButton->setGeometry(QRect(20 + lineWidth,10,41,21));
    okButton->show();
    connect(okButton,SIGNAL(pressed()),this,SLOT(done()));
    connect(le,SIGNAL(returnPressed()),this,SLOT(done()));

    QPushButton *cancel = new QPushButton(Form);
    cancel->setToolTip("Cancel");
    cancel->setIcon(QIcon(":/Images/delete.png"));
    cancel->setGeometry(QRect(71 + lineWidth,10,41,21));
    cancel->show();
    connect(cancel,SIGNAL(pressed()),this,SLOT(cancel()));

    proxy = scene->addWidget(Form);
    proxy->setZValue(this->zValue() + 1);
    proxy->setPos(this->pos());
}

void LineItem::done()
{
    if( !le->text().compare(iname) or le->text().isEmpty())
    {
       this->setPlainText(iname);
       scene->removeItem(proxy);
       emit itemchanged(id,"");
       return;
    }
    else
       this->setPlainText(le->text());
    scene->removeItem(proxy);
    emit itemchanged(id,this->toPlainText());
}

void LineItem::cancel()
{
    scene->removeItem(proxy);
    emit itemchanged(id,this->toPlainText());
}

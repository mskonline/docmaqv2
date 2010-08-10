#include "dateitem.h"

DateItem::DateItem(QGraphicsScene *scene,QGraphicsItem *parent,int id):QGraphicsTextItem(parent)
{
    this->scene = scene;
    this->id  = id;
}
void DateItem::itemName(QString name)
{
    this->iname = name;
}

void DateItem::setText(QString t)
{
    if(t.isEmpty())
        this->setPlainText(iname);
    else
        this->setPlainText(t);
}

void DateItem::constructWidgets()
{
    // Report an item being changed
    emit itemchanged(-id,"");

    Form = new QWidget;
    Form->resize(170,40);
    Form->setWindowOpacity(0.75);

    de = new QDateEdit(Form);
    de->setGeometry(QRect(10, 10,110, 22));
   // de->setMinimumDate(QDate(1996, 1, 1));
    de->setCalendarPopup(true);
    de->setDisplayFormat("dd.MMM.yyyy");
    de->setDate(QDate().fromString(this->toPlainText(),"dd.MMM.yyyy"));
    okButton =  new QPushButton(Form);
    okButton->setIcon(QIcon(":/Images/ok.png"));
    okButton->show();
    okButton->setGeometry(QRect(130,10,31,21));
    connect(okButton,SIGNAL(pressed()),this,SLOT(done()));

    proxy = scene->addWidget(Form);
    proxy->setZValue(this->zValue() + 1);
    proxy->setPos(this->pos());
}

void DateItem::done()
{
	QString t;
        t  = de->text();
	this->setPlainText(t);

        scene->removeItem(proxy);
        emit itemchanged(id,t);
}

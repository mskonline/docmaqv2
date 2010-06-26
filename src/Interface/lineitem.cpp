#include "lineitem.h"
#include "credentialscene.h"

LineItem::LineItem(CredentialScene *scene, int widgetWidth,int lineWidth, QGraphicsItem *parent) : QGraphicsTextItem(parent)
{
    this->scene = scene;
    this->widgetWidth = widgetWidth;
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
    Form->resize(widgetWidth,40);
    Form->setWindowOpacity(0.75);

    le = new QLineEdit(Form);
    le->setGeometry(QRect(10, 10, lineWidth, 20));
    le->setText(this->toPlainText());

    QPushButton *okButton =  new QPushButton(Form);
    okButton->setText("ok");
    okButton->show();
    okButton->setGeometry(QRect(20 + lineWidth,10,41,21));
    connect(okButton,SIGNAL(pressed()),this,SLOT(done()));
    connect(le,SIGNAL(returnPressed()),this,SLOT(done()));

    proxy = scene->addWidget(Form);
    proxy->setZValue(this->zValue() + 1);
    proxy->setPos(this->pos());
}

void LineItem::done()
{
    if(le->text().isEmpty() or le->text().contains(iname))
    {
       this->setPlainText(iname);
       scene->removeItem(proxy);
       return;
    }
    else
       this->setPlainText(le->text());
    scene->removeItem(proxy);
    emit itemchanged(id,this->toPlainText());
}

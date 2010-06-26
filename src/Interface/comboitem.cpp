#include "comboitem.h"
#include "credentialscene.h"

ComboItem::ComboItem(CredentialScene *scene,QGraphicsItem *parent,int widgetWidth,int comboWidth,QStringList list):QGraphicsTextItem(parent)
{
    this->scene = scene;
    this->widgetWidth = widgetWidth;
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
    Form->resize(widgetWidth,40);
    Form->setWindowOpacity(0.75);

    cb = new QComboBox(Form);
    cb->setGeometry(QRect(10, 10, comboWidth, 20));
    this->cb->addItems(list);

    okButton =  new QPushButton(Form);
    okButton->setText("ok");
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
    emit itemchanged(id,cb->currentText());
}

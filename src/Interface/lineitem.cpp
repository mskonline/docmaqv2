#include "lineitem.h"
#include "certificatescene.h"

LineItem::LineItem(CertificateScene *scene,int widgetWidth,int lineWidth)
{
    this->scene = scene;
    this->widgetWidth = widgetWidth;
    this->lineWidth = lineWidth;
}

void LineItem::itemName(QString name)
{
    this->iname = name;
}

void LineItem::toManual()
{

}

void LineItem::toDB()
{

}

void LineItem::constructWidgets()
{
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


    proxy = scene->addWidget(Form);
    proxy->setZValue(this->zValue() + 1);
    proxy->setPos(this->pos());
}

void LineItem::done()
{
    if(le->text().isEmpty())
       this->setPlainText(iname);
    else
       this->setPlainText(le->text());
    scene->removeItem(proxy);
    emit itemchanged(id,this->toPlainText());
}

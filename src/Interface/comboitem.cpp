#include "comboitem.h"
#include "certificatescene.h"

ComboItem::ComboItem(CertificateScene *scene,int widgetWidth,int comboWidth,QStringList list)
{
    this->scene = scene;
    this->widgetWidth = widgetWidth;
    this->comboWidth = comboWidth;
    this->list = list;
}

void ComboItem::itemName(QString name)
{
    this->iname = name.toUpper();
}

void ComboItem::toManual()
{
    this->setPlainText(iname);
}

void ComboItem::toDB()
{

}

void ComboItem::constructWidgets()
{
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
    if(!cb->currentIndex())
    {
        this->setPlainText(iname);
//        emit changed(id,"");
    }
    else
    {
        this->setPlainText(cb->currentText());
//        emit changed(id,cb->currentText());
    }
}

#include "courseitem.h"
#include "certificatescene.h"

CourseItem::CourseItem(CertificateScene *scene, int id)
{
    this->scene = scene;
    this->setPlainText("STUDENT COURSE DETAILS");
    this->id = id;
}

void CourseItem::constructWidgets()
{

    Form = new QWidget;
    Form->resize(450, 42);
    Form->setWindowOpacity(0.75);
    cbsem = new QComboBox(Form);
    cbsem->setGeometry(QRect(270, 10, 111, 21));
    cbyear = new QComboBox(Form);
    cbyear->setGeometry(QRect(190, 10, 71, 21));
    cbbranch = new QComboBox(Form);
    cbbranch->setGeometry(QRect(100, 10, 81, 21));
    cbbranch->setMaxVisibleItems(8);
    cbcourse = new QComboBox(Form);
    cbcourse->setGeometry(QRect(10, 10, 81, 21));
    okButton = new QPushButton(Form);
    okButton->setText("ok");
    okButton->show();
    okButton->setGeometry(QRect(400, 10, 41, 21));

    cbsem->insertItems(0, QStringList()
             << QString()
             << "I Semister"
             << "II Semister"
            );

            cbyear->insertItems(0, QStringList()
             << QString()
             << "I Year"
             << "II Year"
             << "III Year"
             << "IV Year"
            );

            cbbranch->insertItems(0, QStringList()
             << QString()
             << "AM"
             << "Bio-Tech"
             << "CSE"
             << "ECE"
             << "ECM"
             << "EEE"
             << "EET"
             << "IT"
             << "Mech"
             << "BSCE"
             << "CAD-CAM"
             << "ECE"
             << "SE"
             << "S & H"
             << "VLSI"
            );

            cbcourse->insertItems(0, QStringList()
             << QString()
             << "B.Tech"
             << "M.Tech"
             << "MBA"
             << "MCA"
            );

       connect(okButton,SIGNAL(pressed()),this,SLOT(done()));

       proxy = scene->addWidget(Form);
       proxy->setZValue(this->zValue() + 1);
       proxy->setPos(this->pos());
}

void CourseItem::done()
{
    QString t = cbcourse->currentText() + " - " + cbbranch->currentText() + " , " + cbyear->currentText() +
                        " , " + cbsem->currentText();

    this->setPlainText(t);
    scene->removeItem(proxy);
    emit changed(id,t);
}

#include "interface.h"
#include "comboitem.h"
#include "courseitem.h"
#include "dateitem.h"
#include <QDebug>
#include "../Data_Structures/student.h"


Interface::Interface()
{
    setupUi(this);

    createItems();
    bg = new QButtonGroup;

    QGraphicsScene *scene = new QGraphicsScene(0,0,560,75);
    QFont tfont("Bitstream Vera Serif",30);
    tfont.setBold(true);

    ctext = new QGraphicsTextItem(" BONAFIDE ");
    ctext->setPos(156,22);
    ctext->setFont(tfont);
    ctext->setDefaultTextColor("black");

    print= new blinkpix(QPixmap(":/Images/Images/printer.png"));
    print->setAcceptHoverEvents(true);
    print->setPos(12,28);


    pdf = new blinkpix(QPixmap(":/Images/Images/pdf.png"));
    pdf->setAcceptHoverEvents(true);
    pdf->setPos(60,28);


    left = new blinkpix(QPixmap(":/Images/Images/left.png"));
    left->setAcceptHoverEvents(true);
    left->setPos(108,28);

    right = new blinkpix(QPixmap(":/Images/Images/right.png"));
    right->setOffset(404,28);
    right->setAcceptHoverEvents(true);


    settings= new blinkpix(QPixmap(":/Images/Images/settings.png"));
    settings->setOffset(452,28);
    settings->setAcceptHoverEvents(true);


    log = new blinkpix(QPixmap(":/Images/Images/log.png"));
    log->setAcceptHoverEvents(true);
    log->setPos(500,28);


    scene->addItem(ctext);
    scene->addItem(left);
    scene->addItem(right);
    scene->addItem(print);
    scene->addItem(pdf);
    scene->addItem(log);
    scene->addItem(settings);


    anim = new QPropertyAnimation(pic[0],"opacity");
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InExpo);
    anim->setDuration(300);

    this->panelView->setScene(scene);
    this->showMaximized();
}


void Interface::createItems()
{

    QRectF bounds(0,0,619,438);

    cscene = new CertificateScene(this);
    cscene->setSceneRect(bounds);

    QString t = QDate::currentDate().toString("dd.MM.yyyy");
    cdate = new DateItem(cscene, 5);
    cdate->setPlainText(t);
    cdate->setPos(QPointF(512,132));
    qDebug() << cdate->toPlainText();

    stname = new LineItem(cscene,328,261);
    stname->id = 0;
    stname->itemName("STUDENT NAME");
    stname->setPos(QPointF(230,191));
    stname->setPlainText("STUDENT NAME");

    co = new ComboItem(cscene,140,69,QStringList() << "S/o" << "D/o");
    co->id = 1;

    fname = new LineItem(cscene,328,261);
    fname->id = 2;
    fname->itemName("FATHER NAME");
    fname->setPos(QPointF(115,216));
    fname->setPlainText("FATHER NAME");

    roll = new QGraphicsTextItem("ROLL NUMBER");
    roll->setPos(QPointF(206,244));

    cdetails = new CourseItem(cscene,3);
    cdetails->setPos(273,271);

    fyear = new QGraphicsTextItem();
    fyear->setPos(QPointF(192,300));

    tyear = new QGraphicsTextItem();
    tyear->setPos(QPointF(294,300));

    purposetext = new QGraphicsTextItem("This Certificate is issued for the purpose of:");
    purposetext->setPos(QPointF(30,339));

    purpose = new LineItem(cscene,150,92);
    purpose->id = 4;
    purpose->itemName("PURPOSE");
    purpose->setPos(QPointF(365,339));
    purpose->setPlainText("PURPOSE");

    acyear =  new QGraphicsTextItem();

    admno = new QGraphicsTextItem();

    doa = new DateItem(cscene,7);
    dob = new DateItem(cscene,6);
    dol = new DateItem(cscene,10);

    course = new ComboItem(cscene,164,102, QStringList()
    << QString()
    << "B.Tech"
    << "M.Tech"
    << "MBA"
    << "MCA"
    );
    course->id = 8;
    course->itemName("COURSE");
    course->setVisible(false);
    course->setPlainText("COURSE");

    branch = new ComboItem(cscene,164,102, QStringList()
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
    << "VLSI" );
    branch->id = 9;
    branch->itemName("BRANCH");
    branch->setVisible(false);
    branch->setPlainText("BRANCH");

    qlf = new LineItem(cscene,328,261);
    qlf->id = 11;
    qlf->itemName("YES / NO");
    qlf->setPos(QPointF(230,191));
    qlf->setPlainText("YES / NO");

    dues = new LineItem(cscene,328,261);
    dues->id = 12;
    dues->itemName("DUES");
    dues->setPos(QPointF(230,191));
    dues->setPlainText("DUES");

    dis = new LineItem(cscene,328,261);
    dis->id = 13;
    dis->itemName("DISCIPILINE");
    dis->setPos(QPointF(230,191));
    dis->setPlainText("DISCIPILINE");

    conduct = new LineItem(cscene,328,261);
    conduct->id = 14;
    conduct->itemName("CONDUCT");
    conduct->setPos(QPointF(230,191));
    conduct->setPlainText("CONDUCT");

    community = new LineItem(cscene,328,261);
    community->id = 15;
    community->itemName("COMMUNITY");
    community->setPos(QPointF(230,191));
    community->setPlainText("COMMUNITY");

    pic[0] = new gpix(QPixmap("./bpicrz.png"));
    pic[1] = new gpix(QPixmap("./conduct.png"));
    pic[2] = new gpix(QPixmap("./tc.png"));

    cscene->addItem(pic[0]);
    cscene->addItem(pic[1]);
    cscene->addItem(pic[2]);
    cscene->addItem(cdate);
    cscene->addItem(stname);
    cscene->addItem(fname);
    cscene->addItem(roll);
    cscene->addItem(cdetails);
    cscene->addItem(fyear);
    cscene->addItem(tyear);
    cscene->addItem(purpose);
    cscene->addItem(purposetext);
    cscene->addItem(doa);
    cscene->addItem(dob);
    cscene->addItem(dol);
    cscene->addItem(qlf);
    cscene->addItem(conduct);
    cscene->addItem(community);
    cscene->addItem(dues);
    cscene->addItem(dis);
    cscene->addItem(course);
    cscene->addItem(branch);
    cscene->addItem(acyear);
    CertificateView->setScene(cscene);
    CertificateView->setBackgroundBrush(QBrush(QColor("black")));

    showTc(false);
    pic[1]->setOpacity(0.0);
    pic[2]->setOpacity(0.0);
}


void Interface::set_ct(Student *student)
{
    // Sets the certificates
   stname->setPlainText(student->name);
   fname->setPlainText(student->father_name);
   roll->setPlainText(student->roll);
   co->setPlainText(student->co);

   if(student->ctype[1])
       acyear->setPlainText("2009-2010");

   if(student->ctype[2])
   {
       //is TC
       admno->setPlainText(student->roll);
       dob->setPlainText(student->std_ex->dob);
       qDebug() << "DOB : " + student->std_ex->dob;
       doa->setPlainText(student->std_ex->doa);
       course->setPlainText(student->std_ex->course);
       branch->setPlainText(student->std_ex->branch);
       dol->setPlainText(student->std_ex->dol);
       qlf->setPlainText(student->std_ex->qualified);
       dues->setPlainText(student->std_ex->dues);
       dis->setPlainText(student->std_ex->disp_m);
       conduct->setPlainText(student->std_ex->conduct);
       community->setPlainText(student->std_ex->community);

   }
   else
   {
        cdetails->setPlainText(student->cdetails);
        purpose->setPlainText("Bus Pass");
   }

   int i;
   for(i=0; i<3; ++i)
       if(student->ctype[i])
        break;

   student->vpic = i;
   show_ct(student->ctype[2],i);
}

void Interface::show_ct(int ctype,int vpic)
{
    switch(vpic)
    {
    case 0: // Bonafide
               pic[0]->setOpacity(1.0);
               pic[1]->setOpacity(0.0);
               pic[2]->setOpacity(0.0);

               ctext->setPlainText(" BONAFIDE ");
               cscene->setSceneRect(QRectF(0,0,619,438));
               stname->setPos(230,191);
               fname->setPos(115,216);
               roll->setPos(206,244);
               cdate->setPos(515,132);
               co->setPos(505,191);

               cdetails->setPos(273,271);
               cdetails->show();

               fyear->setPos(192,300);
               fyear->show();

               tyear->setPos(294,300);
               tyear->show();

               purpose->show();
               purposetext->show();

               showTc(false);
               acyear->hide();
               break;
    case 1: // Conduct
                pic[0]->setOpacity(0.0);
                pic[1]->setOpacity(1.0);
                pic[2]->setOpacity(0.0);

                ctext->setPlainText(" CONDUCT ");
                cscene->setSceneRect(QRectF(0,0,619,438));
                cdate->setPos(520,59);
                stname->setPos(164,143);
                co->setPos(502,143);
                fname->setPos(57,175);
                roll->setPos(152,205);

                cdetails->setPos(227,235);
                cdetails->show();

                acyear->setPos(222,264);
                acyear->show();

                showTc(false);
                tyear->hide();
                fyear->hide();
                purpose->hide();
                purposetext->hide();
                break;
    case 2: // TC
                pic[0]->setOpacity(0.0);
                pic[1]->setOpacity(0.0);
                pic[2]->setOpacity(1.0);

                ctext->setPlainText("       TC");

                cscene->setSceneRect(QRectF(0,0,619,877));
                cdate->setPos(542,149);
                stname->setPos(214,186);
                fname->setPos(202,228);
                roll->setPos(82,149);
                cdate->setPos(542,149);
                admno->setPos(384,149);
                dob->setPos(190,269);
                doa->setPos(319,314);
                course->setPos(239,352);
                branch->setPos(254,398);
                dol->setPos(285,440);
                qlf->setPos(402,480);
                dues->setPos(136,524);
                dis->setPos(321,567);
                conduct->setPos(172,610);
                community->setPos(389,654);

                showTc(true);
                cdetails->hide();
                tyear->hide();
                fyear->hide();
                acyear->hide();
                purpose->hide();
                purposetext->hide();
    }


 }

void Interface::showTc(bool var)
{
    admno->setVisible(var);
    dob->setVisible(var);
    doa->setVisible(var);
    course->setVisible(var);
    branch->setVisible(var);
    dol->setVisible(var);
    qlf->setVisible(var);
    dues->setVisible(var);
    dis->setVisible(var);
    conduct->setVisible(var);
    community->setVisible(var);
}

void Interface::printDialog()
{
    printd = new Ui::PrintDialog;
    bg->addButton(printd->next1,0);
    bg->addButton(printd->next2,1);

}

void Interface::closeEvent(QCloseEvent *)
{
    qApp->quit();
}





#ifndef INTERFACE_H
#define INTERFACE_H
#include <QtGui>
#include "certificatescene.h"
#include "ui_interface.h"
#include "ui_print.h"
#include "effectspix.h"
#include "lineitem.h"

class Student;
class LineItem;
class CourseItem;
class ComboItem;
class DateItem;

class Interface:  public QMainWindow, public Ui::Interface
{
    Q_OBJECT
public:
    Interface();

    blinkpix  *print,*pdf,*left,*right,*settings,*log;
    gpix *pic[3];
    //int ctype;
    LineItem *stname,*fname,*purpose;
    LineItem *qlf,*conduct,*community;
    LineItem *dues,*dis;
    Ui::PrintDialog *printd;
    DateItem *dob, *doa, *dol, *cdate;
    QGraphicsTextItem *tyear, *fyear, *acyear;
    QButtonGroup *bg;
    CertificateScene *cscene;

    Student *student;

    int vpic;
    void showTc(bool);
    void show_ct(int,int);
    void set_ct(Student *student);
    void printDialog();

private:

    QPropertyAnimation *anim;
    QGraphicsTextItem *purposetext,*roll, *admno,*ctext;
    QGraphicsTextItem *bcount,*ccount;
    ComboItem *course,*branch,*co;
    CourseItem *cdetails;
    void createItems();
protected:
    void closeEvent(QCloseEvent *);
};

#endif // INTERFACE_H

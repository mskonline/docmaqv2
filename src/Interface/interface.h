#ifndef INTERFACE_H
#define INTERFACE_H
#include <QtGui>
#include "credentialscene.h"
#include "about.h"
#include "ui_interface.h"
#include "ui_DIssue.h"
#include "ui_print-single.h"
#include "effectspix.h"
#include "lineitem.h"
#include "comboitem.h"
#include "courseitem.h"
#include "dateitem.h"
#include "../Data_Structures/student.h"

class SPrintDialog : public QWidget, public Ui::sprint
{
    Q_OBJECT
public:
    SPrintDialog(QWidget *parent = 0): QWidget(parent, Qt::Window)
    {
        setupUi(this);
        this->setWindowFlags(Qt::Window
                             | Qt::WindowCloseButtonHint);
        this->nbutton->setDefault(true);
    }
};

class DIssueDialog: public QWidget, public Ui::DIssue
{
    Q_OBJECT

public :
       Student *student;
       DIssueDialog(Student *student)
       {
            setupUi(this);
            this->setWindowFlags(Qt::Window
                                 | Qt::WindowCloseButtonHint);
            show();

           QDate d;
           d.fromString(student->std_ex->dissue->at(1), "yyyy-MM-dd");
           this->student = student;

           this->roll->setText(student->roll);
           this->nsl->setText(student->std_ex->dissue->at(0));
           this->dos->setText(d.toString("dd-MM-yyyy"));
           this->isby->setText(student->std_ex->dissue->at(2));
           this->remarks->setText(student->std_ex->dissue->at(3));

           connect(this->bnext,SIGNAL(released()),this,SLOT(next()));
           connect(this->bback,SIGNAL(released()),this,SLOT(back()));
           connect(this->bprint,SIGNAL(released()),this,SLOT(done()));
        }

signals:
       void print();

public slots:

    void next() { this->stack->setCurrentIndex(1);}
    void back() { this->stack->setCurrentIndex(0);}
    void done()
    {
        int i  = this->nsl->text().toInt() + 1;
        this->student->std_ex->dissue->replace(3, this->remarks->toPlainText());
        student->std_ex->dissue->replace(0, QString().setNum(i));

        emit print();
        this->close();
    }
};

class Interface:  public QMainWindow, public Ui::Interface
{
    Q_OBJECT
public:
    Interface();
    gpix *pic[3];
    ButtonItem *menu,*session;
    ArrowItem *larrow,*rarrow, *uarrow, *darrow;
    LineItem *stname,*fname,*purpose;
    LineItem *qlf,*conduct,*community;
    LineItem *dues,*dis , *cdetails;
    ComboItem *course,*branch;
    SPrintDialog *prints;
    DateItem *dob, *doa, *dol;
    CredentialScene *cscene;
    Student *student;
    ComboItem *co;
    QGraphicsProxyWidget *lproxy,*rproxy,*sproxy;
    QGraphicsTextItem *tyear, *fyear, *roll,*acyear, *sno ,*cdate, *tc_type;
    QToolButton *settingsb, *tcb,*fscreenb;
    QToolButton *logb, *helpb,*aboutb;
    QShortcut *Bsc, *Csc, *Tsc, *help;
    QLabel *mode;
    int vpic;

    void set_TC_Items(int);
    void set_items(QStringList *);
    void showTc(bool);
    void show_ct(int);
    void resetView(int);
    void showHome(bool);
    void setStatusBar(QString, int);
    void set_ct(Student *);
    void set_tc(Student *);
    void show_dissue();

private:
    QWidget *lframe, *rframe;
    QPropertyAnimation *anim;
    QStateMachine *machine;
    QState *s1, *s2;
    QGraphicsTextItem *purposetext, *admno,*ctext;
    QGraphicsTextItem *bcount,*ccount;
    QString temp;
    qreal z;

    void create_NItems();
    void createPanels();
    void createPrintDialog();
    void createShortCuts();


signals:
    void quit();

public slots:
    void viewresized();
    void fnscreen();
    void createsession(QStringList );
    void closesessionpage();
    void report(int,QString);
    void about_docmaq();

protected:
    void closeEvent(QCloseEvent *);
};

#endif // INTERFACE_H

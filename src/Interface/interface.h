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

#ifndef INTERFACE_H
#define INTERFACE_H

#include <QtGui>
#include "about.h"
#include "ui_interface.h"
#include "ui_DIssue.h"
#include "ui_print-single.h"
#include "effectspix.h"
#include "lineitem.h"
#include "comboitem.h"
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
        this->nprint->setDefault(true);
        QPalette palette;
        palette.setBrush(this->backgroundRole(), QBrush(QImage(":/Images/printb.png")));
        this->setPalette(palette);
    }
};

class DIssueDialog: public QWidget, public Ui::DIssue
{
    Q_OBJECT

public :
       Student *student;
       DIssueDialog(QWidget *parent,Student *student):QWidget(parent,Qt::Window)
       {
           setupUi(this);
           setWindowFlags(Qt::Window
                                 | Qt::WindowCloseButtonHint);
           QWidget::setAttribute(Qt::WA_DeleteOnClose);
           bnext->setEnabled(false);


           QDate d,d2,d3;
           d2 = d.fromString(student->std_ex->dissue->at(1), "yyyy-MM-dd");
           d3 = d.fromString(student->std_ex->dissue->at(3), "yyyy-MM-dd");
           this->student = student;

           roll->setText(student->roll);
           oio->setText(d2.toString("dd.MMM.yyyy"));
           oib->setText(student->std_ex->dissue->at(2));
           ndi->setText(student->std_ex->dissue->at(0));

           if(student->std_ex->dissue->at(0).toInt())
           {
               dio->setText(d3.toString("dd.MMM.yyyy"));
               isby->setText(student->std_ex->dissue->at(4));
               remarks->setText(student->std_ex->dissue->at(5));
           }
           else
           {
               dio->setText("NOT GIVEN YET");
               isby->setText("NOT GIVEN YET");
               remarks->setText("");
           }

           connect(bnext,SIGNAL(released()),this,SLOT(next()));
           connect(bback,SIGNAL(released()),this,SLOT(back()));
           connect(bprint,SIGNAL(released()),this,SLOT(done()));
           connect(nremarks,SIGNAL(textChanged()),this,SLOT(enable()));
        }

signals:
       void print();

public slots:

    void next() { stack->setCurrentIndex(1);}
    void back() { stack->setCurrentIndex(0);}
    void done()
    {
        student->std_ex->dissue->replace(5, nremarks->toPlainText());
        int i  = ndi->text().toInt() + 1;
        student->std_ex->dissue->replace(0, QString().setNum(i));

        emit print();
        close();
    }

    void enable()
    {
        if(nremarks->toPlainText().isEmpty())
            bnext->setEnabled(false);
        else
            bnext->setEnabled(true);
    }
};

class Interface:  public QMainWindow, public Ui::Interface
{
    Q_OBJECT

public:
    HoverItem *menu,*session;
    HoverItem *larrow,*rarrow, *uarrow, *darrow;
    LineItem *stname,*fname,*purpose;
    LineItem  *cdetails,*acyear;
    ComboItem *course,*branch,*co;
    SPrintDialog *prints;
    Student *student;
    DIssueDialog *dissue;

    QGraphicsScene *cscene;
    QGraphicsPixmapItem *pic[3];
    QGraphicsProxyWidget *lproxy,*rproxy,*sproxy,*tpanel;
    QList <QGraphicsTextItem *> *tcitems;
    QGraphicsTextItem *tyear, *fyear, *roll, *sno ,*cdate, *tc_type;
    QGraphicsTextItem *admno,*tc_sno;
    QToolButton *settingsb, *tcb,*fscreenb;
    QToolButton *logb, *pdfb,*aboutb;
    QShortcut *Bsc, *Csc;
    QLabel *mode,*ulb,*timelb,*dq;
    QCheckBox *typecb;
    QSpinBox *sersb;
    QPushButton *tprint;
    QLineEdit *troll;

    Interface();
    void set_TC_Items(int);
    void set_items(QStringList &);
    void showTc(bool);
    void show_ct(int);
    void resetView(int);
    void setStatusBar();
    void setSBText(QString, bool);
    void set_ct(Student *);
    void set_tc(Student *);
    void show_dissue();
    void setenabled(bool);

private:
    QWidget *lframe, *rframe;
    QPropertyAnimation *anim;
    QStateMachine *machine;
    QState *s1, *s2;
    QGraphicsTextItem *purposetext;
    QGraphicsTextItem *bcount,*ccount;
    QString temp;
    qreal z;
    bool tpl;

    void create_NItems();
    void createPanels();
    void create_TC_Panel();
    void createPrintDialog();
    void createShortCuts();

signals:
    void quit();

public slots:
    void viewresized();
    void fnscreen();
    void showsessionpage(QStringList &);
    void closesessionpage();
    void report(int,QString);
    void about_docmaq();

protected:
    void closeEvent(QCloseEvent *);
};

#endif // INTERFACE_H

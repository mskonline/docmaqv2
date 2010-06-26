#ifndef CPRINTER_H
#define CPRINTER_H

#include <QPrinter>
#include <QPainter>
#include <QTimer>

class Student;

class CPrinter:public QObject
{
    Q_OBJECT

public:
    int st_ptr,p_type;
    Student *tc_student;

    CPrinter(QList <int> &, QStringList &);

    QStringList availablePrinters();
    void updateDList(QStringList *);
    void pcancel();
    void pList(QList <Student* > *);
    void updatePositionList(QList <int>, int type);
    void printB();
    void printC();
    void printTC();
    void stagecomplete();

private:
    QList <int> blist,clist,tlist,&c_sno;
    QList <Student *> *st_list;
    QString temp;
    QPrinter *printer;
    QPainter *painter;
    QPointF pt;
    QFont paintfont,snofont;
    QStringList &dlist;
    int stno;
    float valw,valh;

signals:
    void pcomplete(int);

public slots:
    void printc();
};

#endif // CPRINTER_H

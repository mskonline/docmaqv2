#ifndef CPRINTER_H
#define CPRINTER_H

#include <QPrinter>
#include <QPainter>
class Student;

class CPrinter:public QObject
{
    Q_OBJECT
private:
    QList <int> blist;
    QPrinter *printer;
    QPainter *painter;
public:
    CPrinter();
    QStringList availablePrinters();
    int printB(QList <Student *>,int, QStringList);
    void printC(QList <Student *>);
    void printTC(QList <Student *>);

public slots:
    void updateList(QList<int> ,int);
};

#endif // CPRINTER_H

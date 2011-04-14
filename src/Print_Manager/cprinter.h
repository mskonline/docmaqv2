/*  DocmaQ v2.1, Credential Publishing System
    Copyright (C) 2011 M.Sai Kumar <msk.mymails@gmail.com>

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

#ifndef CPRINTER_H
#define CPRINTER_H

#include <QPrinter>
#include <QPainter>

class Student;

class CPrinter:public QObject
{
    Q_OBJECT

public:
    int st_ptr,p_type,pdf,admin,t,count;
    Student *tc_student;
    QList <int> blist,clist,tlist;
    QList <Student *> *st_list;
    QString path,p;

    CPrinter(QList <int> &,QStringList &);
    void pdfprint(int,int);
    void printB();
    void printC();
    void processTC();
    void printTC();
    void setPrinter();
    void setTCprint(bool);

private:
    QList <int> &c_sno;
    QString temp;
    QPrinter *printer,*pdf_printer,*tcprinter;
    QPainter *painter;
    QPointF pt;
    QFont paintfont,snofont,cfont;
    QStringList &dlist,type;
    int stno;
    float valw,valh,tvalh,tvalw;

signals:
    void writeLog(QString &, const short &);
    void pcomplete(int);
    void tc_pcomplete();

public slots:
    void printc();
};

#endif // CPRINTER_H

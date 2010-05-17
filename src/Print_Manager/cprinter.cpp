#include "cprinter.h"
#include "../Data_Structures/student.h"

CPrinter::CPrinter()
{
    printer = new QPrinter(QPrinter::HighResolution);
    printer->setPaperSize(QPrinter::A5);
    printer->setOrientation(QPrinter::Landscape);
    printer->setFullPage(true);
}

void CPrinter::updateList(QList <int> lt, int type)
{


}

QStringList CPrinter::availablePrinters()
{

    /*QPrinter print;
    QPrinterInfo pinfo(print);
    QList <QPrinterInfo> pl;
    pl = pinfo.availablePrinters();

    for(int p=0;p < pl.size();++p)
    ui.cbpl->insertItem(p,pl.at(p).printerName());*/

}

int CPrinter::printB(QList <Student *> st_list, int sno, QStringList dlist)
{
    QPointF pt;
    float valw,valh;
    QFont paintfont("Bitstream Vera Serif",11);
    QFont snofont("Bitstream Vera Serif",18);
    QString temp;
    painter = new QPainter(printer);
    painter->setFont(snofont);

    valw = (printer->width()/printer->widthMM());
    valh = (printer->height()/printer->heightMM());

    for(int i = 0; i <= st_list.count(); ++i)
    {
        pt.setX(blist.at(0) * valw);
        pt.setY(blist.at(1) * valh);
        painter->drawText (pt,temp.sprintf("%04d",sno));

        painter->setFont(paintfont);
        pt.setX(blist.at(2) * valw);
        pt.setY(blist.at(3) * valh);
        painter->drawText ( pt, dlist.at(0) );

        pt.setX(blist.at(4) * valw);
        pt.setY(blist.at(5) * valh);
        painter->drawText ( pt,st_list.at(i)->name);

        pt.setX(blist.at(6) * valw);
        pt.setY(blist.at(7) * valh);
        painter->drawText (pt,st_list.at(i)->co);

        pt.setX(blist.at(8) * valw);
        pt.setY(blist.at(9) * valh);
        painter->drawText (pt,st_list.at(i)->father_name);

        pt.setX(blist.at(10)* valw);
        pt.setY(blist.at(11)* valh);
        painter->drawText (pt,st_list.at(i)->roll);

        pt.setX(blist.at(12) * valw);
        pt.setY(blist.at(13) * valh);
        painter->drawText (pt,st_list.at(i)->cdetails);

        // Academic years
        pt.setX(blist.at(14) * valw);
        pt.setY(blist.at(15) * valh);
        painter->drawText (pt,dlist.at(1));


        pt.setX(blist.at(16) * valw);
        pt.setY(blist.at(17) * valh);
        painter->drawText ( pt,dlist.at(2) );

        if( !st_list.at(i)->purpose.isEmpty())
        {
            pt.setX(blist.at(18) * valw);
            pt.setY(blist.at(19) * valh);
            painter->drawText (pt,"This Certificate is issued for the Purpose of : ");

            pt.setX(blist.at(20) * valw);
            pt.setY(blist.at(21) * valh) ;
            painter->drawText (pt,st_list.at(i)->purpose);
        }

        ++sno;
        painter->end();
        delete painter;
    }
    return sno;
}

void CPrinter::printC(QList <Student *> list)
{

}

void CPrinter::printTC(QList <Student *> list)
{

}

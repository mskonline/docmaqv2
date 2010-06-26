#include "cprinter.h"
#include "../Data_Structures/student.h"

CPrinter::CPrinter(QList <int> &c_sno, QStringList &dlist):c_sno(c_sno),dlist(dlist)
{
    printer = new QPrinter(QPrinter::HighResolution);
    printer->setPaperSize(QPrinter::A5);
    printer->setOrientation(QPrinter::Landscape);
    printer->setFullPage(true);

    valw = (printer->width()/printer->widthMM());
    valh = (printer->height()/printer->heightMM());

    paintfont.setFamily("Bitstream Vera Serif");
    paintfont.setPointSize(11);

    snofont.setFamily("Bitstream Vera Serif");
    snofont.setPointSize(18);

    p_type = 0;

}

void CPrinter::printc()
{
    switch(p_type)
    {
    case 0 : // Printing Bonafide
        printB();
        break;
    case 1 : // Prining Conduct
        printC();
        break;
    case 2: // Printing TC
        printTC();//Tsno
    }
}

void CPrinter::pcancel()
{

}

/* printB()
 * Prints the Bonafide Certificate
 */
void CPrinter::printB()
{
    st_ptr = 0;
    for(; st_ptr < stno ; ++st_ptr)
    {
        if(st_list->at(st_ptr)->c_type[0])
        {
            printer->setOutputFormat(QPrinter::PdfFormat);
            printer->setDocName(st_list->at(st_ptr)->roll);

            painter = new QPainter(printer);

            painter->setFont(snofont);

            // Sno
            pt.setX(blist.at(0) * valw);
            pt.setY(blist.at(1) * valh);
            painter->drawText (pt,temp.sprintf("%04d",c_sno.at(0)));

            // Date
            painter->setFont(paintfont);
            pt.setX(blist.at(2) * valw);
            pt.setY(blist.at(3) * valh);
            painter->drawText ( pt, dlist.at(0) );

            // Student Name
            pt.setX(blist.at(4) * valw);
            pt.setY(blist.at(5) * valh);
            painter->drawText ( pt,st_list->at(st_ptr)->name);

            // C/o
            pt.setX(blist.at(6) * valw);
            pt.setY(blist.at(7) * valh);
            painter->drawText (pt,st_list->at(st_ptr)->co);

            // Father's Name
            pt.setX(blist.at(8) * valw);
            pt.setY(blist.at(9) * valh);
            painter->drawText (pt,st_list->at(st_ptr)->father_name);

            // Roll Number
            pt.setX(blist.at(10)* valw);
            pt.setY(blist.at(11)* valh);
            painter->drawText (pt,st_list->at(st_ptr)->roll);

            // Course Details
            pt.setX(blist.at(12) * valw);
            pt.setY(blist.at(13) * valh);
            painter->drawText (pt,st_list->at(st_ptr)->cdetails);

            // Academic years
            pt.setX(blist.at(14) * valw);
            pt.setY(blist.at(15) * valh);
            painter->drawText (pt,dlist.at(1));


            pt.setX(blist.at(16) * valw);
            pt.setY(blist.at(17) * valh);
            painter->drawText ( pt,dlist.at(2) );

            // Purpose
            if( !st_list->at(st_ptr)->purpose.isEmpty())
            {
                pt.setX(blist.at(18) * valw);
                pt.setY(blist.at(19) * valh);
                painter->drawText (pt,"This Certificate is issued for the Purpose of : ");

                pt.setX(blist.at(20) * valw);
                pt.setY(blist.at(21) * valh) ;
                painter->drawText (pt,st_list->at(st_ptr)->purpose);
            }


            c_sno[0] = c_sno[0] + 1;

            if(c_sno[0] == 9999)
                c_sno[0] = 1;

            ++st_ptr;
            painter->end();
            delete painter;
        }
    }
    st_ptr = 0;
    emit pcomplete(0);
}

/* printC()
 * Prints the Conduct Certificate
 */
void CPrinter::printC()
{
    st_ptr = 0;
    for(; st_ptr < stno ; ++st_ptr)
    {
        // Check if Conduct is checked
        if(st_list->at(st_ptr)->c_type[1])
        {
            printer->setDocName(st_list->at(st_ptr)->roll);
            painter = new QPainter(printer);
            painter->setFont(snofont);

            // Sno
            pt.setX(clist.at(0) * valw);
            pt.setY(clist.at(1) * valh);
            painter->drawText (pt,temp.sprintf("%04d",c_sno.at(1)));

            // Date
            painter->setFont(paintfont);
            pt.setX(clist.at(2) * valw);
            pt.setY(clist.at(3) * valh);
            painter->drawText ( pt, dlist.at(0) );

            // Student Name
            pt.setX(clist.at(4) * valw);
            pt.setY(clist.at(5) * valh);
            painter->drawText ( pt,st_list->at(st_ptr)->name);

            // C/o
            pt.setX(clist.at(6) * valw);
            pt.setY(clist.at(7) * valh);
            painter->drawText (pt,st_list->at(st_ptr)->co);

            // Father's Name
            pt.setX(clist.at(8) * valw);
            pt.setY(clist.at(9) * valh);
            painter->drawText (pt,st_list->at(st_ptr)->father_name);

            // Roll Number
            pt.setX(clist.at(10)* valw);
            pt.setY(clist.at(11)* valh);
            painter->drawText (pt,st_list->at(st_ptr)->roll);

            // Course Details
            pt.setX(clist.at(12) * valw);
            pt.setY(clist.at(13) * valh);
            painter->drawText (pt,st_list->at(st_ptr)->cdetails);

            // Academic Years
            pt.setX(blist.at(14) * valw);
            pt.setY(blist.at(15) * valh);
            painter->drawText (pt,dlist.at(1) + " - " + dlist.at(2));

            //Increment Serial Number
            c_sno[1] = c_sno[1] + 1;

            // Log the Print


            c_sno[1] = c_sno[1] + 1;

            if(c_sno[1] == 9999)
                c_sno[1] = 1;

            painter->end();
            delete painter;
         }
    }

    st_ptr = 0;
    emit pcomplete(0);
}

void CPrinter::printTC()
{
   int i = 0, count = 0;
   QStringList type;

   if(tc_student->std_ex->is)
   {
       count = 1;
       type << "Duplicate";
   }
   else
   {
       type << "Original" << "Duplicate";
       count = 2;
   }

   // Two Copies of TC are to printed (if this is first issue)
   // Original and Duplicate
   for(; i < count ; ++i)
   {
       printer->setDocName(tc_student->roll);
       painter = new QPainter(printer);

       // Type
       pt.setX(tlist.at(0)* valw);
       pt.setY(tlist.at(1)* valh);
       painter->drawText ( pt, type.at(i));

       // Roll Number
       pt.setX(tlist.at(2) * valw);
       pt.setY(tlist.at(3) * valh);
       painter->drawText (pt,tc_student->roll);

       // Admission Number
       painter->setFont(paintfont);
       pt.setX(tlist.at(4) * valw);
       pt.setY(tlist.at(5) * valh);
       painter->drawText ( pt, tc_student->roll);

       // Date
       pt.setX(tlist.at(6)* valw);
       pt.setY(tlist.at(7)* valh);
       painter->drawText ( pt,"");

       // Student Name
       pt.setX(tlist.at(8) * valw);
       pt.setY(tlist.at(9) * valh);
       painter->drawText ( pt,tc_student->name);

       // Father's Name
       pt.setX(tlist.at(10) * valw);
       pt.setY(tlist.at(11) * valh);
       painter->drawText (pt,tc_student->father_name);

       // Dob
       pt.setX(tlist.at(12) * valw);
       pt.setY(tlist.at(13) * valh);
       painter->drawText ( pt,tc_student->std_ex->dob);

       // Doa
       pt.setX(tlist.at(14)* valw);
       pt.setY(tlist.at(15)* valh);
       painter->drawText ( pt,tc_student->std_ex->doa);

       // Course
       pt.setX(tlist.at(16) * valw);
       pt.setY(tlist.at(17) * valh);
       painter->drawText (pt,tc_student->course);

       // Branch
       pt.setX(blist.at(18) * valw);
       pt.setY(blist.at(19) * valh);
       painter->drawText (pt,tc_student->branch);

       // Dol
       pt.setX(tlist.at(20)* valw);
       pt.setY(tlist.at(21)* valh);
       painter->drawText ( pt,tc_student->std_ex->dol);

       // Qualified
       pt.setX(tlist.at(22)* valw);
       pt.setY(tlist.at(23)* valh);
       painter->drawText ( pt,tc_student->std_ex->qualified);

       // Dues
       pt.setX(tlist.at(24)* valw);
       pt.setY(tlist.at(25)* valh);
       painter->drawText ( pt,tc_student->std_ex->dues);

       // Disciplinary Measures
       pt.setX(tlist.at(26)* valw);
       pt.setY(tlist.at(27)* valh);
       painter->drawText ( pt,tc_student->std_ex->disp_m);

       // Conduct
       pt.setX(tlist.at(28)* valw);
       pt.setY(tlist.at(29)* valh);
       painter->drawText ( pt,tc_student->std_ex->conduct);

       // Community
       pt.setX(tlist.at(30)* valw);
       pt.setY(tlist.at(31)* valh);
       painter->drawText ( pt,tc_student->std_ex->community);

       // Remarks
       pt.setX(tlist.at(32)* valw);
       pt.setY(tlist.at(33)* valh);
       painter->drawText ( pt,tc_student->std_ex->remarks);

       painter->end();
       delete painter;
   }

   emit pcomplete(2);
}

void CPrinter::updatePositionList(QList <int> lt, int type)
{


}

void CPrinter::pList(QList <Student* > *st_list)
{
    this->st_list = st_list;
    stno = st_list->count();
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

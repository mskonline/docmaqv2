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

#include "../Data_Structures/student.h"
#include "cprinter.h"
#include <QDate>
#include <QFileDialog>

/*
 * Constructor
 */
CPrinter::CPrinter(QList <int> &c_sno, QStringList &dlist )
    :c_sno(c_sno),dlist(dlist)
{
    printer = 0;

    paintfont.setFamily("Bitstream Vera Serif");
    paintfont.setPointSize(11);

    snofont.setFamily("Bitstream Vera Serif");
    snofont.setPointSize(18);

    cfont.setFamily("Bitstream Vera Serif");
    cfont.setPointSize(11);
    cfont.setItalic(true);

    p_type = 0;
    pdf = 0;
    count = 2;
    admin = 0;
}

/* setPrinter()
 * Called : By AppManager::load_final_modules()
 * Performs : Sets the Printer
 */
void CPrinter::setPrinter()
{
    if(printer)
        delete printer;

    printer = new QPrinter(QPrinter::HighResolution);
    printer->setPrinterName(p);
    printer->setPaperSize(QPrinter::A5);
    printer->setOrientation(QPrinter::Landscape);
    printer->setFullPage(true);

    valw = (printer->width()/printer->widthMM());
    valh = (printer->height()/printer->heightMM());
}

/* printc()
 * Called : By AppManager::init_print()
 * Performs : As a Switch function for
 * Bonafide & Conduct Printing
 */
void CPrinter::printc()
{
    stno = st_list->count();
    switch(p_type)
    {
    case 0 : // Printing Bonafide
        printB();
        break;
    case 1 : // Prining Conduct
        printC();
    }
}

/* pdfprint(int,int)
 * Called : By AppManager::pdfprint()
 * Performs : Pdf printing of Bonafide and Conduct Certificates
 */
void CPrinter::pdfprint(int bc,int cc)
{
    path = QFileDialog::getExistingDirectory(0, tr("Select a Directory to Save the PDF file(s)"),"\\",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);

    if(path.isEmpty())
        return;

    stno = st_list->count();
    pdf_printer = new QPrinter(QPrinter::HighResolution);
    pdf_printer->setPaperSize(QPrinter::A5);

    pdf_printer->setOrientation(QPrinter::Landscape);
    pdf_printer->setFullPage(true);
    pdf_printer->setOutputFormat(QPrinter::PdfFormat);
    
    valw = (pdf_printer->width()/pdf_printer->widthMM());
    valh = (pdf_printer->height()/pdf_printer->heightMM());
    
    // Pdf printing start
    pdf = 1;

    if(bc)
        printB();

    stno = st_list->count();

    if(cc)
        printC();


    // Pdf printing End
    pdf = 0;
    delete pdf_printer;

    path ="";
    // Reset Ratio back to Normal
    valw = (printer->width()/printer->widthMM());
    valh = (printer->height()/printer->heightMM());
}

/* printB()
 * Called : By printc()
 * Performs : Printing of Bonafide Certificate
 */
void CPrinter::printB()
{
    st_ptr = 0;

    for(; st_ptr < stno ; ++st_ptr)
    {
        if(st_list->at(st_ptr)->c_type[0])
        {
            if(pdf)
            {
                pdf_printer->setOutputFileName(path + "/" + st_list->at(st_ptr)->roll + "-bonafide.pdf");
                painter = new QPainter(pdf_printer);
            }
            else
            {
                printer->setDocName(st_list->at(st_ptr)->roll);
                painter = new QPainter(printer);
            }
                
            painter->setFont(snofont);

            // Sno
            pt.setX(blist.at(0) * valw);
            pt.setY(blist.at(1) * valh);
            painter->drawText (pt,temp.sprintf("%04d",st_list->at(st_ptr)->c_sno[0]));

            // Date
            painter->setFont(paintfont);
            pt.setX(blist.at(2) * valw);
            pt.setY(blist.at(3) * valh);
            painter->drawText ( pt, dlist.at(0));

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
            painter->drawText ( pt,dlist.at(2));

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

            c_sno[0] = c_sno[0] % 9999 + 1;

            painter->end();
            delete painter;
        }
    }

    st_ptr = 0;
    emit pcomplete(0);
}

/* printC()
 * Called : By printc()
 * Performs : Printing of Conduct Certificate
 */
void CPrinter::printC()
{
    st_ptr = 0;

    for(; st_ptr < stno ; ++st_ptr)
    {
        // Check if Conduct is checked
        if(st_list->at(st_ptr)->c_type[1])
        {
            if(pdf)
            {
                pdf_printer->setOutputFileName(path + "/" + st_list->at(st_ptr)->roll + "-conduct.pdf");
                painter = new QPainter(pdf_printer);
            }
            else
            {
                printer->setDocName(st_list->at(st_ptr)->roll);
                painter = new QPainter(printer);
            }

            painter->setFont(snofont);

            // Sno
            pt.setX(clist.at(0) * valw);
            pt.setY(clist.at(1) * valh);
            painter->drawText (pt,temp.sprintf("%04d",st_list->at(st_ptr)->c_sno[1]));

            // Date
            painter->setFont(paintfont);
            pt.setX(clist.at(2) * valw);
            pt.setY(clist.at(3) * valh);
            painter->drawText ( pt, dlist.at(0));

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
            pt.setX(clist.at(14) * valw);
            pt.setY(clist.at(15) * valh);
            painter->drawText (pt,st_list->at(st_ptr)->acyear);

            // Incremant Count
            c_sno[1] = c_sno[1] % 9999 + 1;

            painter->end();
            delete painter;
         }
    }

    st_ptr = 0;
    emit pcomplete(1);
}

/* setTCprint(bool)
 * Called : By AppManager::TC_mode()
 * Performs : Initialisation for TC printing
 */
void CPrinter::setTCprint(bool v)
{
    if(v)
    {
        tcprinter = new QPrinter(QPrinter::HighResolution);
        tcprinter->setPrinterName(p);
        tcprinter->setPaperSize(QPrinter::A4);
        tcprinter->setOrientation(QPrinter::Portrait);
        tcprinter->setFullPage(true);

        tvalw = (tcprinter->width()/tcprinter->widthMM());
        tvalh = (tcprinter->height()/tcprinter->heightMM());

        type << "ORIGINAL" << "DUPLICATE" << "DUPLICATE";

        // Admin can issue only one certificate
        if(admin == 1)
            count = 1;   
    }
    else
    {
        tlist.clear();
        type.clear();
        delete tcprinter;
    }
}

/* processTC()
 * Called : By AppManager::init_print()
 * Performs : Checks for Admn Mode
 */
void CPrinter::processTC()
{
    if(admin == 0)
    {
        if(tc_student->std_ex->is)
            t = 1; // duplicate
        else
            t = 0; // Original
    }

    // Everything is set.Lets go for Printing
    printTC();

    emit tc_pcomplete();
}

/* printTC()
 * Called : By processTC()
 * Performs : Printing of TC
 */
void CPrinter::printTC()
{
    // Two Copies of TC are to printed (if this is first issue)
   // Original and Duplicate
   for(int i = 0; i < count ; ++i)
   {
       if(i)
            tcprinter->setDocName(tc_student->roll + "-2");
       else
            tcprinter->setDocName(tc_student->roll + "-1");
       painter = new QPainter(tcprinter);

       painter->setFont(snofont);
       // sno
       pt.setX(tlist.at(0) * tvalw);
       pt.setY(tlist.at(1) * tvalh);
       painter->drawText(pt, temp.sprintf("%05d",tc_student->c_sno[0]));

       // Type
       pt.setX(tlist.at(2)* tvalw);
       pt.setY(tlist.at(3)* tvalh);
       painter->drawText ( pt, type.at(t));

       painter->setFont(paintfont);
       // Roll Number
       pt.setX(tlist.at(4) * tvalw);
       pt.setY(tlist.at(5) * tvalh);
       painter->drawText (pt,tc_student->roll);

       // Admission Number
       painter->setFont(paintfont);
       pt.setX(tlist.at(6) * tvalw);
       pt.setY(tlist.at(7) * tvalh);
       painter->drawText ( pt, tc_student->roll);

       // Date
       pt.setX(tlist.at(8)* tvalw);
       pt.setY(tlist.at(9)* tvalh);
       painter->drawText ( pt,QDate::currentDate().toString("dd.MMM.yyyy"));

       // Student Name
       pt.setX(tlist.at(10) * tvalw);
       pt.setY(tlist.at(11) * tvalh);
       painter->drawText ( pt,tc_student->name);

       // Father's Name
       pt.setX(tlist.at(12) * tvalw);
       pt.setY(tlist.at(13) * tvalh);
       painter->drawText (pt,tc_student->father_name);

       // Dob
       pt.setX(tlist.at(14) * tvalw);
       pt.setY(tlist.at(15) * tvalh);
       painter->drawText ( pt,tc_student->std_ex->dob);

       // Doa
       pt.setX(tlist.at(16)* tvalw);
       pt.setY(tlist.at(17)* tvalh);
       painter->drawText ( pt,tc_student->std_ex->doa);

       // Course
       pt.setX(tlist.at(18) * tvalw);
       pt.setY(tlist.at(19) * tvalh);
       painter->drawText (pt,tc_student->course);

       // Branch
       pt.setX(tlist.at(20) * tvalw);
       pt.setY(tlist.at(21) * tvalh);
       painter->drawText (pt,tc_student->branch);

       // Dol
       pt.setX(tlist.at(22)* tvalw);
       pt.setY(tlist.at(23)* tvalh);
       painter->drawText ( pt,tc_student->std_ex->dol);

       // Qualified
       pt.setX(tlist.at(24)* tvalw);
       pt.setY(tlist.at(25)* tvalh);
       painter->drawText ( pt,tc_student->std_ex->qualified);

       // Dues
       pt.setX(tlist.at(26)* tvalw);
       pt.setY(tlist.at(27)* tvalh);
       painter->drawText ( pt,tc_student->std_ex->dues);

       // Disciplinary Measures
       pt.setX(tlist.at(28)* tvalw);
       pt.setY(tlist.at(29)* tvalh);
       painter->drawText ( pt,tc_student->std_ex->disp_m);

       painter->setFont(cfont);

       // Conduct
       pt.setX(tlist.at(30)* tvalw);
       pt.setY(tlist.at(31)* tvalh);
       painter->drawText ( pt,tc_student->std_ex->conduct);

       painter->setFont(paintfont);
       // Community
       pt.setX(tlist.at(32)* tvalw);
       pt.setY(tlist.at(33)* tvalh);
       painter->drawText ( pt,tc_student->std_ex->community);

       // Remarks
       pt.setX(tlist.at(34)* tvalw);
       pt.setY(tlist.at(35)* tvalh);
       painter->drawText ( pt,tc_student->std_ex->remarks);

       ++t;
       painter->end();
       delete painter;
   }
}

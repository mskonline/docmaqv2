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

#ifndef STUDENT_H
#define STUDENT_H

#include <QString>

#define ERROR 0
#define WARNING 1
#define INFO 2

class Student_ex;

class Student
{
public :
        QString name, id,
        roll,
        father_name,
        co,purpose;
        QString cdetails,branch,course,acyear;
        int bid;
        int st_filled,stx_filled;
        int c_type[2], c_sno[2];
        int vpic;
        Student_ex *std_ex;
Student()
{
    st_filled = 0;
    stx_filled = 0;
    c_sno[0] = 0;
    c_sno[1] = 0;
    c_sno[2] = 0;
    c_type[0] = 0;
    c_type[1] = 0;
    vpic = 0;
    co = "S/o";
    purpose = "Bus Pass";
}
};

class Student_ex
{
public :
        int is;
        QString dob,doa,dol,type;
        QString qualified,dues,disp_m;
        QString conduct,remarks,community;
        QStringList *dissue;
        Student_ex(){is = 0; dissue = 0;}
};

#endif // STUDENT_H

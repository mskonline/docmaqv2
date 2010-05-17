#ifndef STUDENT_H
#define STUDENT_H

#include <QString>

class Student_ex;

class Student
{
public :
        QString name,
        roll,
        father_name,
        co,purpose;
        QString cdetails,branch,course;
        int bid;
        int st_filled,stx_filled;
        int ctype[3];
        int vpic;
        Student_ex *std_ex;
Student()
{
    st_filled = 0;
    stx_filled = 0;
    vpic = 0;
}
};

class Student_ex : public Student
{
public :
        QString dob,doa,dol;
        QString qualified,dues,disp_m;
        QString conduct,remarks,community;
};

#endif // STUDENT_H


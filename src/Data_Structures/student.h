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
        QString cdetails,branch,course;
        int bid;
        int st_filled,stx_filled;
        int c_type[3], c_sno[2];
        int vpic;
        Student_ex *std_ex;
Student()
{
    st_filled = 0;
    stx_filled = 0;
    vpic = 0;
    co = "S/o";
    purpose = "Bus Pass";
}
};

class Student_ex : public Student
{
public :
        int is;
        QString dob,doa,dol,type;
        QString qualified,dues,disp_m;
        QString conduct,remarks,community;
        QStringList *dissue;
        Student_ex(){is = 0;}
};

#endif // STUDENT_H


#include "cdatabase.h"
#include "QDebug"
#include <QString>

CDatabase::CDatabase()
{
    qp1 = "select STUDENT_ID from student_admission where STUDENT_ROLLNO=";
    qp2 = "select student_name,father_name,gender from student_info where student_id=" ;
    qp3 = "select course,branch_id,current_sem,current_year from student_academic where student_id=" ;
    qp4 = "select date_of_birth,caste from student_info where student_id=";
    qp5 = "select date_of_adm from student_admission where student_id=" ;
}

bool CDatabase::connect() //QStringList dbdetails)
{
        db = QSqlDatabase::addDatabase("QMYSQL");
        db.setDatabaseName("sreenidhi");
        db.setPort(3306);
        db.setHostName("localhost");
        db.setUserName("root");
        db.setPassword("123");

        return db.open();
}

bool CDatabase::reconnect(QStringList dbdetails)
{
    db.close();
    //return connect(dbdetails);
    return true;
}


bool CDatabase::fetch(int isTC,Student *student)
{
    QString q,id;
    QSqlQuery query(QSqlDatabase::database());

    if(!student->st_filled)
    {
        // first query,fetches students unique ID
        q = qp1 + "'" + student->roll  + "'" ;
        query.exec(q);
        query.next();
        id = query.value(0).toString();

        if( id.isEmpty() )
        {
            //error("No Data Exists for :" + student->roll);
            return false;
        }

        // second query,fetches student name,fathers name, gender
        q = qp2 + id;
        query.exec(q);
        query.next();
        student->name = query.value(0).toString().toUpper();
        student->father_name = query.value(1).toString().toUpper();
        gender = query.value(2).toString();

        if ( student->name.isEmpty() && student->father_name.isEmpty() && gender.isEmpty() )
        {
               //error("Few Empty Fields in Database for : " + student->roll);
                return false;
        }

        // third query,fetches course details
        q = qp3 + id;
        query.exec(q);
        query.next();
        student->course = query.value(0).toString();
        student->bid = query.value(1).toInt();
        sem = query.value(2).toString();
        year = query.value(3).toString();


        if ( student->course.isEmpty() &&  student->bid == 0 && year.isEmpty() && sem.isEmpty())
        {
            //error("Few Empty Fields in Database for : " + student->stroll);
            return false;
        }

        format_course(student);
        student->st_filled = 1;
    }

    if(isTC==1 and !student->stx_filled) // is TC
    {
        student->std_ex = new Student_ex;
        QString c;

        q = qp4 + id;
        query.exec(q);
        query.next();

        d_b = query.value(0).toDate();
        c = query.value(1).toString();

        q = qp5 + id;
        query.exec(q);
        query.next();

        d_a = query.value(0).toDate();

        student->std_ex->dob = d_b.toString("dd.MM.yyyy");
        student->std_ex->doa = d_a.toString("dd.MM.yyyy");

        student->std_ex->dol = QDate::currentDate().toString("dd.MM.yyyy");
        student->std_ex->qualified = "Yes";
        student->std_ex->dues = "No";
        student->std_ex->disp_m = "None";
        student->std_ex->conduct = "Satisfactory";

        if(c != "OC")
        student->std_ex->community = c;
        student->stx_filled = 1;
     }
    return true;
}


void CDatabase::get_user_password(QString userid)
{
    QString q1,q2,q3,passwd;
    QSqlQuery query(QSqlDatabase::database());
    QSqlQuery query2(QSqlDatabase::database());

    q1 = "select role from login_info where user_name='" + userid + "'";


    query.exec(q1);
    query.next();

     for(int i = 0 ; i <= query.size() ;++i)
    {
        if(query.value(0).toString() == "academic")
        {
            q2 = "select passwd from login_info where user_name='" + userid + "'" ;
            query2.exec(q2);
            query2.next();
            passwd = query2.value(0).toString();

            q3 = "select teacher_name from teacher_info where teacher_code='" + userid + "'";
            query2.exec(q3);
            query2.next();

            username = query2.value(0).toString();
            break;
        }
        query.next();
    }

    emit user_passwd(passwd);
}


void CDatabase::format_course(Student *student)
{
    int  var_yr = year.toInt();

        // Year
        switch ( var_yr )
        {
        case 1: year = ", I year";
            break;
        case 2: year = ", II year";
            break;
        case 3: year = ", III year";
            break;
        case 4: year = ", IV year";
        }

   int  var_sem = sem.toInt();

       // B'tech 1st year don't have semester system
        if (student->course == "B.Tech" and var_yr == 1)
       {
           sem = "";
           var_sem = 0;
        }
       else
       {
            switch ( var_sem )
            {
                case 1 : sem = ", I semester";
                break;
                case 2 : sem = ", II semester";
            }
       }

       // Branch
       switch ( student->bid )
        {
        case 1 : student->branch ="EEE";
            break;
        case 2 : student->branch ="CSE";
            break;
        case 3 : student->branch ="ECE";
            break;
        case 4 : student->branch ="IT";
            break;
        case 5 : student->branch ="Mech";
            break;
        case 6 : student->branch ="Bio-Tech";
            break;
        case 9 : student->branch ="ECM";
            break;
        case 10 : student->branch ="S & H";
            break;
        case 12 : student->branch ="";//MBA
            break;
        case 13 : student->branch ="AM";
            break;
        case 14 : student->branch ="EET";
            break;
        case 15 : student->branch ="ECE";
            break;
        case 16 : student->branch ="BSCE";
            break;
        case 17 : student->branch ="VLSI";
            break;
        case 19 : student->branch ="SE";
            break;
        case 20 : student->branch ="CAD-CAM";
            break;
        case 21 : student->branch ="Bio-Tech";
            break;
        case 22 : student->branch ="";//MCA
            break;
        default : student->branch = "";
    }

        QString temp2 = student->branch;
        student->branch = !student->branch.isEmpty() ? " - " + student->branch : "";

        // constructing course details.
        student->cdetails = student->course + student->branch + year + sem;

        if(gender == "Male")
        {
            student->name = "Mr. " + student->name;
            student->co = "S/o";
        }
        else
        {
            student->name = "Ms. " + student->name;
            student->co = "D/o";
        }

        qDebug() << "fomat course";
}

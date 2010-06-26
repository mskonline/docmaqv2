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
    qpc = "select status from tc_log where student_id=";
}

bool CDatabase::connect() //QStringList dbdetails)
{
        db = QSqlDatabase::addDatabase("QMYSQL");
        db.setDatabaseName("sreenidhi");
        db.setPort(3306);
        db.setHostName("localhost");
        db.setUserName("root");
        db.setPassword("123");

        is_connected = db.open();

        return is_connected;
}

bool CDatabase::reconnect(QStringList dbdetails)
{
    db.close();
    //return connect(dbdetails);
    return true;
}

/* bool fetch(Student *)
 * fetches details for Bonafide & Conduct
 */
bool CDatabase::fetch(Student *student)
{
    QString q;
    QSqlQuery query(QSqlDatabase::database());

    if(!student->st_filled)
    {
        // first query,fetches students unique ID
        q = qp1 + "'" + student->roll  + "'" ;
        query.exec(q);
        query.next();
        student->id = query.value(0).toString();

        if( student->id.isEmpty() )
        {
            emit report(ERROR,"No Details Found for : " + student->roll + " . Check whether you have entered the Roll Number Correctly or Whether the Database is Configured Correctly. ");
            return false;
        }

        // second query,fetches student name,fathers name, gender
        q = qp2 + student->id;
        query.exec(q);
        query.next();
        student->name = query.value(0).toString().toUpper();
        student->father_name = query.value(1).toString().toUpper();
        gender = query.value(2).toString();

        if ( student->name.isEmpty() && student->father_name.isEmpty() && gender.isEmpty() )
        {
            emit report(WARNING , "Few Empty Details in Database for : " + student->roll + ". Please Fill them Manually.");
            student->st_filled = 0;
        }
        else
            student->st_filled  = 1;

        // third query,fetches course details
        q = qp3 + student->id;
        query.exec(q);
        query.next();
        student->course = query.value(0).toString();
        student->bid = query.value(1).toInt();
        sem = query.value(2).toString();
        year = query.value(3).toString();


        if ( student->course.isEmpty() &&  student->bid == 0 && year.isEmpty() && sem.isEmpty())
        {
            emit report(WARNING , "Few Empty Details in Database for : " + student->roll + ". Please Fill them Manually.");
            student->st_filled = 0;
        }
        else
            student->st_filled = 1;

        format_course(student);
    }

    return true;
}


bool CDatabase::fetch_ex(Student *student)
{
     QSqlQuery query(QSqlDatabase::database());

    if(fetch(student))
    {
        student->std_ex = new Student_ex;
        QString c,q;


        if(check_duplicate_issue(student))
        {
            emit report(ERROR,"An Error occurred while checking for any previous issues of TC for : " + student->roll + " . Please check if the Database is properly configured");
            return false;
        }

        if(student->std_ex->is)
            student->std_ex->type = "Duplicate";
        else
            student->std_ex->type = "Original";

        q = qp4 + student->id;
        query.exec(q);
        query.next();

        d_b = query.value(0).toDate();
        c = query.value(1).toString();

        q = qp5 + student->id;
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
        student->std_ex->remarks = "";

        if(c != "OC")
            student->std_ex->community = c;
        else
            student->std_ex->community = "None";

        student->stx_filled = 1;
     }
    return true;
}


int CDatabase::check_duplicate_issue(Student *student)
{
    QSqlQuery query(QSqlDatabase::database());

    qpc = qpc + student->id;
    query.exec(qpc);

    if(query.lastError().type() != QSqlError::NoError)
           return 1;


    query.next();

    if(!query.value(0).toString().isEmpty())
    {
        student->std_ex->dissue =  new QStringList;
        QString q = "Select status,date_of_issue,issued_by, remarks from tc_log where student_id=" + student->id;
        query.exec(q);
        query.next();

        student->std_ex->dissue->append(query.value(0).toString()); // Status
        student->std_ex->dissue->append(query.value(1).toString()); // Date_of_issue
        student->std_ex->dissue->append(query.value(2).toString()); // Issued_by
        student->std_ex->dissue->append(query.value(3).toString()); // Remarks

        student->std_ex->is = 1;
    }

    return 0;
}


int CDatabase::update_dissue(Student *student)
{
    QString q ;
    QSqlQuery query(QSqlDatabase::database());

    QString d,i,r ;

    d = "'" + student->std_ex->dissue->at(1) + "'";
    i = "'" + student->std_ex->dissue->at(2) + "'";
    r = "'" + student->std_ex->dissue->at(3) + "'";


    q = "update tc_log set status=" + student->std_ex->dissue->at(0) + "," +tr(" date_of_issue=") +  d +  "," + tr(" issued_by=") +
          i  +  "," + tr(" remarks=") + r + " where student_id=" + student->id;

    query.exec(q);
    if(query.lastError().type() != QSqlError::NoError)
    {
        emit report(ERROR,"An Error occurred while updating Remarks of TC for : " + student->roll + " . Please check if the Database is properly configured");
        return -1;
    }

    return 0;
}

/* void log_tc(QStringList)
 * Called after Printing is completed & is logged in the TC Log
 * Record is created when TC is issued for the first time
 */
int CDatabase::log_tc(Student *student)
{
    QString q;
    QStringList *lt = student->std_ex->dissue;
    QSqlQuery query(QSqlDatabase::database());

    QString d,i,r ;

    d = "'" + lt->at(1) + "'";
    i = "'" + lt->at(2) + "'";
    r = "'" + lt->at(3) + "'";

    q = "insert into tc_log(student_id,status,date_of_issue,issued_by,remarks) values(" + student->id + "," +
        lt->at(0) + "," +  d +  "," + i + "," + r + ")";

    query.exec(q);

    if(query.lastError().type() != QSqlError::NoError)
    {
           report (ERROR,"An Error occurred while Recording an issue of TC for : " + student->roll + " . Please check if the Database is properly configured") ;
           return -1;
    }
    return 0;
}

/* void get_user_password(QString)
 * Called by AuthenDialog to obtain User's Password
 */
void CDatabase::get_user_password(QString userid)
{
    QString q1,q2,passwd;
    QSqlQuery query(QSqlDatabase::database());
    QSqlQuery query2(QSqlDatabase::database());

    q1 = "select passwd from login_info where user_name='" + userid + "' and role='academic'" ;

    query.exec(q1);
    query.next();

    passwd = query.value(0).toString();

    if(!passwd.isEmpty())
    {
        q2 = "select teacher_name from teacher_info where teacher_code='" + userid + "'";
        query2.exec(q2);
        query2.next();

        username = query2.value(0).toString();
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
}
